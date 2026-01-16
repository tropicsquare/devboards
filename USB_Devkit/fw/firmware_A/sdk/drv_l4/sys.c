#include "sys.h"

#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"


#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007)
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006)
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005)
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004)
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003)
#endif // ! NVIC_PRIORITYGROUP_0

static void _change_pll_frequency(u32 freq) // [Hz]
{
    if (freq < SYS_HCLK_MIN)
        freq = SYS_HCLK_MIN;
    if (freq > SYS_HCLK_MAX)
        freq = SYS_HCLK_MAX;

    freq /= 1000000;

    // Switch the system clock source to HSI (or MSI) temporarily
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI); // Switch to HSI (16 MHz)
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
        ; // Wait for switch
    
    // Disable the PLL
    LL_RCC_PLL_Disable();
    while (LL_RCC_PLL_IsReady() != 0)
        ;  // Wait until PLL is disabled
    
    // Configure the PLL to the new frequency ('freq' MHz)
    // PLL input source: HSE, PLLM divider: 2, PLLN multiplier: freq, PLLR divider: 4
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, freq, LL_RCC_PLLR_DIV_4);
    
    // Re-enable the PLL
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1)
        ;  // Wait until PLL is ready
    
    // Enable the PLL output to the system clock
    LL_RCC_PLL_EnableDomain_SYS();
    
    // Switch the system clock source back to the PLL
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        ;  // Wait for switch

    LL_SetSystemCoreClock(freq * 1000000);
}

void sys_init(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); // 
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_PWREN); // Power interface clock enable

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

void sys_clock_config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
        ;

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();

    // Wait till HSE is ready
    while(LL_RCC_HSE_IsReady() != 1)
        ;
    LL_RCC_HSI_Enable();

    // Wait till HSI is ready
    while(LL_RCC_HSI_IsReady() != 1)
        ;
    LL_RCC_HSI_SetCalibTrimming(64);
    LL_RCC_HSI48_Enable();

    // Wait till HSI48 is ready
    while(LL_RCC_HSI48_IsReady() != 1)
        ;
    LL_PWR_EnableBkUpAccess();

    _change_pll_frequency(SYS_HCLK_MAX);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

}

void sys_usb_clock_config(void)
{
    LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_HSI48);
}

u32 sys_get_hclk(void)
{
    return (SystemCoreClock); // [Hz]
}

bool sys_set_hclk(u32 freq) // [Hz]
{
    if ((freq < SYS_HCLK_MIN) || (freq > SYS_HCLK_MAX))
        return (false);

    _change_pll_frequency(freq);
    return (true);
}

u32 sys_flash_size(void)
{
    return (LL_GetFlashSize());
}


// ST HAL overlay
void HAL_Delay(uint32_t delay)
{
    OS_DELAY(delay);
}

void HAL_PWREx_EnableVddUSB(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_USV);
}
