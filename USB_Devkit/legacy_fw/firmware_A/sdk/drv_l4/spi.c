#include "common.h"
#include "hardware.h"

#include "spi.h"
#include "gpio.h"
#include "dma.h"
#include "sys.h"

#include <stm32l4xx_ll_spi.h>
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_rcc.h>

#if SPI1_ON

#define PRESCALER_APB2_MAX 16
#define PRESCALER_SPI_MAX 256


static bool _spi1_cs_state = SPI_CS_IDLE; // true == active == LOW

static void _spi1_pin_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    HW_SPI_SW_CS_INIT;
    HW_SPI_SW_CS_UP;

    // SPI1 GPIO Configuration
    // PA1   ------> SPI1_SCK
    // PA6   ------> SPI1_MISO
    // PA7   ------> SPI1_MOSI
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // switch ON pull-up on MISO to have defined state when sending request or SPI inactive
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);
}

static u32 _apb2_get_prescaler(void)
{
    switch (LL_RCC_GetAPB2Prescaler())
    {
    case LL_RCC_APB2_DIV_1: return(1);
    case LL_RCC_APB2_DIV_2: return(2);
    case LL_RCC_APB2_DIV_4: return(4);
    case LL_RCC_APB2_DIV_8: return(8);
    case LL_RCC_APB2_DIV_16: return(16);
    default:
        break;
    }
    return (1);
}

static void _apb2_set_prescaler(u32 div)
{
    switch (div)
    {
    case 1: LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1); return;
    case 2: LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2); return;
    case 4: LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_4); return;
    case 8: LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_8); return;
    case 16: LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_16); return;
    default:
        break;
    }
}

u32 spi1_get_frequency(void)
{   // HCLK -> APB2 -> PCLK2
    u32 clk = sys_get_hclk() / _apb2_get_prescaler();
    return (clk / spi1_get_prescaler());
}

bool spi1_set_frequency(u32 freq)
{
    // setting exact SPI frequency is a little bit tricky
    // For SPI we have only prescaler 'power of 2' so we have to change HCLK and APB2 prescaler
    // it has impact to all HCLK timed peripheries ! 
    // Luckily USB on L4P has its own 48MHz oscillator.
#define FREQ_MIN (SYS_HCLK_MIN/PRESCALER_SPI_MAX/PRESCALER_APB2_MAX)

    u32 prescaler = 2;

    if (freq < FREQ_MIN)
        return (false);

    if (freq > SYS_HCLK_MAX/2)
        return (false);

    while ((freq * prescaler) < SYS_HCLK_MIN)
        prescaler *= 2;

    if (! sys_set_hclk(freq * prescaler))
    {
        // OS_PRINTF("err: F=%ld, D=%ld" NL, freq * prescaler, prescaler);
        return (false);
    }

    if (prescaler > PRESCALER_SPI_MAX)
    {
        // OS_PRINTF("F=%ld, spi=%d, apb=%ld" NL, freq * prescaler, PRESCALER_SPI_MAX,  prescaler / PRESCALER_SPI_MAX);
        spi1_set_prescaler(PRESCALER_SPI_MAX);
        prescaler /= PRESCALER_SPI_MAX;
        _apb2_set_prescaler(prescaler);
    }   
    else
    {
        // OS_PRINTF("F=%ld, spi=%ld, apb=%d" NL, freq * prescaler, prescaler,  1);
        _apb2_set_prescaler(1);
        spi1_set_prescaler(prescaler);
    }

    return (true);
}

u32 spi1_get_prescaler(void)
{
    switch (SPI1->CR1 & SPI_CR1_BR)
    {
    case LL_SPI_BAUDRATEPRESCALER_DIV2: return (2);
    case LL_SPI_BAUDRATEPRESCALER_DIV4: return (4);
    case LL_SPI_BAUDRATEPRESCALER_DIV8: return (8);
    case LL_SPI_BAUDRATEPRESCALER_DIV16: return (16);
    case LL_SPI_BAUDRATEPRESCALER_DIV32: return (32);
    case LL_SPI_BAUDRATEPRESCALER_DIV64: return (64);
    case LL_SPI_BAUDRATEPRESCALER_DIV128: return (128);
    case LL_SPI_BAUDRATEPRESCALER_DIV256: return (256);
    default:
        break;
    }
    return (0); // impossible value
}

bool spi1_set_prescaler(u32 value)
{
    u32 prescaler;

    switch (value)
    {
    case   2: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV2;   break;
    case   4: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV4;   break;
    case   8: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV8;   break;
    case  16: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV16;  break;
    case  32: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV32;  break;
    case  64: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV64;  break;
    case 128: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV128; break;
    case 256: prescaler = LL_SPI_BAUDRATEPRESCALER_DIV256; break;
    default:
        return (false);
    }
    SPI1->CR1 = (SPI1->CR1 & ~SPI_CR1_BR) | prescaler;
    return (true);
}

void spi1_init(void)
{
    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    // Peripheral clock enable
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    dma_init();

    _spi1_pin_init();

    // SPI1 parameter configuration
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV16;
    SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly           = 7;
    LL_SPI_Init(SPI1, &SPI_InitStruct);

    dma_init_spi_rx();
    dma_init_spi_tx();

    // enable SPI1
    LL_SPI_Enable(SPI1);

    while ((SPI1->SR & SPI_SR_TXE) == 0)
        ;
}

void spi1_data_transfer(char *rx, char *tx, size_t len)
{
/*
    while (len--)
    {
        *rx = spi1_transfer(*tx);
        rx++;
        tx++;
    }
#warning "DMA not used"
    return;
*/

    SPI1->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

    dma_spi_rx_start(rx, len);
    dma_spi_tx_start(tx, len);

    while (! dma_spi_rx_done())
        ;

    SPI1->CR2 &= ~(SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN);
}

void spi1_flush(void)
{
    if (SPI1->SR & SPI_SR_RXNE)
    {
        u8 tmp = SPI1->DR;
        (void)tmp;
    }
}

u8 spi1_transfer(u8 c)
{
    while((SPI1->SR & SPI_SR_TXE) == 0)
        ;

    *((__IO u8 *)&SPI1->DR) = c;

    while((SPI1->SR & SPI_SR_RXNE) == 0)
        ;

    return SPI1->DR;
}

bool spi1_cs_state(void)
{
    return(_spi1_cs_state ? true : false);
}

void spi1_cs(bool state)
{
    if (state)
    {
        HW_SPI_SW_CS_DOWN;
        _spi1_cs_state = SPI_CS_ACTIVE;
    }
    else
    {
        HW_SPI_SW_CS_UP;
        _spi1_cs_state = SPI_CS_IDLE;
    }
}

#endif // SPI1_ON


#if SPI2_ON

static void _spi2_pin_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    // SPI2 GPIO Configuration
    // PB10     ------> SPI2_SCK
    // PB14     ------> SPI2_MISO
    // PB15     ------> SPI2_MOSI
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void spi2_init(void)
{
    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    // Peripheral clock enable
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

    _spi2_pin_init();

    // SPI2 parameter configuration
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV256;
    SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly           = 7;
    LL_SPI_Init(SPI2, &SPI_InitStruct);

    LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_EnableNSSPulseMgt(SPI2);

    // enable SPI2
    LL_SPI_Enable(SPI2);

    while ((SPI2->SR & SPI_SR_TXE) == 0)
        ;
}

void spi2_flush(void)
{
    if (SPI2->SR & SPI_SR_RXNE)
    {
        u8 tmp = SPI2->DR;
        (void)tmp;
    }
}

u8 spi2_transfer(u8 c)
{
    while((SPI2->SR & SPI_SR_TXE) == 0)
        ;

    *((__IO u8 *)&SPI2->DR) = c;

    while((SPI2->SR & SPI_SR_RXNE) == 0)
        ;

    return SPI2->DR;
}

#endif // SPI2_ON

