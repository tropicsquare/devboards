#include "os.h"
#include "adc.h"
#include "irq.h"
#include "log.h"

LOG_DEF("ADC");

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_rcc.h"

#define ADC_TASK_PERIOD (50 * OS_TIMER_MS)

static int adc_channel_ptr = 0;
static u16 adc_data[ADC_CHANNELS];
static u32 adc_values[ADC_CHANNELS];
static bool adc_running = false;

static const u32 adc_channel_map[ADC_CHANNELS]= ADC_CHANNEL_MAP;

void adc_pin_init(gpio_port_t *port, int pin)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gpio_ll_map[pin];
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(port, &GPIO_InitStruct);
}

static void _adc_channel_setup(u8 rank, u8 channel)
{
    LL_ADC_SetChannelSamplingTime(ADC1, channel, LL_ADC_SAMPLINGTIME_12CYCLES_5);
    LL_ADC_SetChannelSingleDiff(ADC1, channel, LL_ADC_SINGLE_ENDED);
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
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);

    // Peripheral clock enable 
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);


    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);

    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

    ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV16;
    ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);

    LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);

    // set sampling time
    ADC1->SMPR1 = ADC_SMPR1_SMP1_2 |  ADC_SMPR1_SMP2_1 |  ADC_SMPR1_SMP3_1 |  ADC_SMPR1_SMP4_1
                | ADC_SMPR1_SMP5_1 |  ADC_SMPR1_SMP6_1 |  ADC_SMPR1_SMP7_1 |  ADC_SMPR1_SMP8_1 |  ADC_SMPR1_SMP9_1;

    ADC1->SMPR2 = ADC_SMPR2_SMP10_1 | ADC_SMPR2_SMP11_1 | ADC_SMPR2_SMP12_1 | ADC_SMPR2_SMP13_1
                | ADC_SMPR2_SMP14_1 | ADC_SMPR2_SMP15_1 | ADC_SMPR2_SMP16_1 | ADC_SMPR2_SMP17_1 | ADC_SMPR2_SMP18_1;

    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_PATH_INTERNAL_TEMPSENSOR | LL_ADC_PATH_INTERNAL_VBAT);
    ADC1->SQR1 = 0;

    // Disable ADC deep power down (enabled by default after reset state)
    LL_ADC_DisableDeepPowerDown(ADC1);
    // Enable ADC internal voltage regulator
    LL_ADC_EnableInternalRegulator(ADC1);

    uint32_t wait_loop_index;
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
        wait_loop_index--;
    }
    // Configure Regular Channel
    /* LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_2CYCLES_5);
    LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SINGLE_ENDED); */


    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
    OS_DELAY_US(1000);

    ADC1->SQR1 = 0;

    for (i=0; i<ADC_CHANNELS; i++)
    {
        _adc_channel_setup(i, adc_channel_map[i]);
        adc_values[i] = ADC_VALUE_NONE;
    }

    // enable IRQ (EOC == End Of Conversion)
    LL_ADC_EnableIT_EOC(ADC1);
    irq_enable(ADC1_2_IRQn, ADC_ISR_PRIO);
    // SET_BIT(ADC1->IER, ADC_IER_EOCIE); // ADC EOC interrupt enable
    
    LL_ADC_Enable(ADC1);
}

static void _adc_start(void)
{
    adc_running = true;
    adc_channel_ptr = 0;
    _adc_channel_select(adc_channel_ptr);
    LL_ADC_REG_StartConversion(ADC1);
}


void ADC1_2_IRQHandler(void)
{
    u32 status = ADC1->ISR;

    if ((status & ADC_ISR_EOC) == 0)
        return;

    // end of single conversion

    if (adc_channel_ptr < ADC_CHANNELS)
    {
        adc_data[adc_channel_ptr] = ADC1->DR; // clears ADC_ISR_EOC
        adc_channel_ptr++;

        if (adc_channel_ptr < ADC_CHANNELS)
        {   // switch to next channel and start conversion
            _adc_channel_select(adc_channel_ptr);
            LL_ADC_REG_StartConversion(ADC1);
        }
        else
        {   // all channels done, wait for manual start
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
            // TODO: calibration
            adc_values[i] = tmp;
        }
    }
    else
    {
        LOG_DEBUG("task fail");
    }

    _adc_start();
}

void adc_test(void)
{
    if (adc_channel_ptr == ADC_CHANNELS)
    {
        int i;

        for (i=0; i<ADC_CHANNELS; i++)
        {
            s32 tmp = adc_values[i];
            OS_PRINTF("ADC%d: %ld" NL, i, tmp);
        }
    }
    else
    {
        OS_PRINTF("ADC err" NL);
        _adc_start();
    }
}
