#include "os.h"
#include "adc.h"
#include "irq.h"

#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"

#define	ADC_TASK_PERIOD (50 * OS_TIMER_MS)

static int adc_channel_ptr = 0;
static u16 adc_data[ADC_CHANNELS];
static u32 adc_values[ADC_CHANNELS];
static bool adc_running = false;

static const u8 adc_channel_map[ADC_CHANNELS]= ADC_CHANNEL_MAP;

void adc_pin_init(gpio_port_t *port, int pin)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = gpio_ll_map[pin];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	LL_GPIO_Init(port, &GPIO_InitStruct);
}

static void _adc_channel_setup(u8 rank, u8 channel)
{
	LL_ADC_SetChannelSamplingTime(ADC1, channel, LL_ADC_SAMPLINGTIME_1CYCLE_5);
}

static void _adc_channel_select(u8 channel)
{
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, adc_channel_map[channel]);
	// ADC1->SQR3 = adc_channel_map[channel] << ADC_SQR3_SQ1_Pos;
}

void adc_init(void)
{
	uint32_t i;

	LL_ADC_InitTypeDef ADC_InitStruct = {0};
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

	// 
 	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_4);

	// Peripheral clock enable 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

/*	_adc_pin_init(HW_AD_IN1_PORT, HW_AD_IN1_BIT); // CH2
	_adc_pin_init(HW_AD_IN2_PORT, HW_AD_IN2_BIT); // CH3
	_adc_pin_init(HW_AD_T1_PORT, HW_AD_T1_BIT);   // CH11
	_adc_pin_init(HW_AD_T2_PORT, HW_AD_T2_BIT);   // CH10
	_adc_pin_init(HW_AD_T3_PORT, HW_AD_T3_BIT);   // CH1
	_adc_pin_init(HW_AD_T4_PORT, HW_AD_T4_BIT);   // CH6
	_adc_pin_init(HW_AD_TM1_PORT, HW_AD_TM1_BIT); // CH13
	_adc_pin_init(HW_AD_TM2_PORT, HW_AD_TM2_BIT); // CH12

	_adc_pin_init(HW_AD_24V1_PORT, HW_AD_24V1_BIT); // CH4 e_fuse 24V out
	_adc_pin_init(HW_AD_24V2_PORT, HW_AD_24V2_BIT); // CH5 power 24V in
*/

	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	OS_DELAY_US(100);
	LL_ADC_StartCalibration(ADC1); // dulezite, bez toho ADC dost keca
	OS_DELAY_US(1000);

	ADC1->SQR1 = 0;

	for (i=0; i<ADC_CHANNELS; i++)
	{
		_adc_channel_setup(i, adc_channel_map[i]);
		adc_values[i] = ADC_VALUE_NONE;
	}

	// enable IRQ (EOC == End Of Conversion)
  	irq_enable(ADC1_IRQn, ADC_ISR_PRIO);
	SET_BIT(ADC1->CR1, ADC_CR1_EOCIE); // ADC EOC interrupt enable

    LL_ADC_Enable(ADC1);
}

static void _adc_start(void)
{
	adc_running = true;
	adc_channel_ptr = 0;
	_adc_channel_select(adc_channel_ptr);
	LL_ADC_REG_StartConversionSWStart(ADC1);
}

void adc_test(void)
{
	int i;
	
	// ADC_TypeDef
	if (adc_channel_ptr == ADC_CHANNELS)
	{	// conversion DONE
		OS_PRINTF("ADC: ");
		
		for (i=0; i<ADC_CHANNELS; i++)
		{
			OS_PRINTF("%d ", adc_data[i]);
		}
		OS_PRINTF(NL);
	
		_adc_start();
		return;
	}

// 	OS_PRINTF("ADC error, ISR=0x%lx, CR=0x%lx" NL, ADC1->ISR, ADC1->CR);
	
	_adc_start();
}

void ADC1_IRQHandler(void)
{
	u32 status = ADC1->SR;

	if ((status & ADC_SR_EOC) == 0)
		return;

	// end of single conversion

	if (adc_channel_ptr < ADC_CHANNELS)
	{
		adc_data[adc_channel_ptr] = ADC1->DR; // clears ADC_SR_EOC
		adc_channel_ptr++;

		if (adc_channel_ptr < ADC_CHANNELS)
		{	// switch to next channel and start conversion
			_adc_channel_select(adc_channel_ptr);
			LL_ADC_REG_StartConversionSWStart(ADC1);
		}
		else
		{	// all channels done, wait for manual start
			adc_running = false;
		}
	}
}

u32 adc_value(adc_channel_e channel)
{
	if (channel >= ADC_CHANNEL_SIZE)
		return (ADC_VALUE_NONE);

	return (adc_values[channel]);
}

void adc_task(void)
{
	static os_timer_t task_timer = 0;

	os_timer_t now = os_timer_get_time();

	if (adc_running)
	{
		return;
	}

	if (now < task_timer)
		return;

	task_timer = now + ADC_TASK_PERIOD;

	if (adc_channel_ptr == ADC_CHANNELS)
	{
		int i;

		for (i=0; i<ADC_CHANNELS; i++)
		{
			s32 tmp = adc_data[i];
			/*
			// raw value calibration stored in bootloader (maybe unused)
			hw_info_calibration_t *c = &HW_INFO.calibration[HW_CALIBRATION_ADC_OFFSET + i];
			tmp += c->add;
			tmp = (tmp*(32768+c->mult))/32768;
			*/
			adc_values[i] = tmp;
			// additional user calibration in calibration.c module
		}
	}

	_adc_start();
}

