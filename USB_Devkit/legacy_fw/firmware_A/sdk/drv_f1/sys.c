#include "sys.h"

#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"


#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007)
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006)
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005)
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004)
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003)
#endif // ! NVIC_PRIORITYGROUP_0

void sys_init(void)
{
    u32 tmp;

    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE); // Enable the FLASH prefetch buffer

    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); // Power interface clock enable
    // SET_BIT(RCC->APB1ENR, RCC_APB1ENR_BKPEN); // Backup interface clock enable

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); // STM32L4
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); // Alternate function I/O clock enable

    // disable JTAG (we need these pins as gpio)
    tmp = AFIO->MAPR;
    tmp &= ~AFIO_MAPR_SWJ_CFG_Msk;
    tmp |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
    AFIO->MAPR = tmp;
}

void sys_clock_config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
    {
    }

#if HW_HSE_ENABLED
    LL_RCC_HSE_Enable();

    while(LL_RCC_HSE_IsReady() != 1)
        ;

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
#else // HW_HSE_ENABLED
  #error "unconfigured clock"
#endif // not HW_HSE_ENABLED
    LL_RCC_PLL_Enable();
    
#if HW_LSE_ENABLED
    LL_RCC_LSE_Enable();

    // Wait till LSE is ready
    while(LL_RCC_LSE_IsReady() != 1)
        ;
    
    if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
    }
    LL_RCC_EnableRTC();
#endif // HW_LSE_ENABLED

    LL_PWR_EnableBkUpAccess();

    // Wait till PLL is ready
    while(LL_RCC_PLL_IsReady() != 1)
        ;
    
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    // Wait till System clock is ready
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        ;
    
    LL_SetSystemCoreClock(SYSCLK);

    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
}

void sys_usb_clock_config(void)
{
    LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL_DIV_1_5); // core 72MHz
    // LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL); // core 48MHz
}

u32 sys_flash_size(void)
{
    return (LL_GetFlashSize());
}

