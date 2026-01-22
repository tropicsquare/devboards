#ifndef ADC_H
#define	ADC_H

#include "common.h"
#include "gpio.h"

#define	ADC_VREF       (3300) // Vref+ == 3.3V
#define	ADC_RESOLUTION (4096) 

typedef enum {
	ADC_CHANNEL_IN1 = 0,
	ADC_CHANNEL_IN2,
	ADC_CHANNEL_IN3,
	ADC_CHANNEL_IN4,
	ADC_CHANNEL_IN5,
	ADC_CHANNEL_IN6,
	ADC_CHANNEL_IN7,
	ADC_CHANNEL_IN8,
	ADC_CHANNEL_IN9,
	ADC_CHANNEL_IN10,

	ADC_CHANNEL_REF,

	ADC_CHANNEL_SIZE
} adc_channel_e;

#define	ADC_CHANNEL_NONE ADC_CHANNEL_SIZE

#define	ADC_CHANNELS (HW_ADC_CHANNELS + 2)
#define	ADC_CHANNEL_MAP {HW_AD_CHANNEL_MAP, 16, 17}
// temp sensor ch16, Vref ch17

#define	ADC_VALUE_NONE (0xFFFFFFFF)


void adc_init(void);
void adc_pin_init(gpio_port_t *port, int pin);
u32  adc_value(adc_channel_e channel);
void adc_task(void);

#endif // ! ADC_H

