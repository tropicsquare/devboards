#include "common.h"
#include "hardware.h"

#include "spi.h"
#include "gpio.h"
#include "dma.h"

#include <stm32f1xx_ll_spi.h>
#include <stm32f1xx_ll_bus.h>

#if SPI1_ON

static bool _spi1_cs_state = SPI_CS_IDLE; // true == active == LOW

static void _spi1_pin_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    // SPI1 GPIO Configuration
    // PA5   ------> SPI1_SCK
    // PA6   ------> SPI1_MISO
    // PA7   ------> SPI1_MOSI
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    
    HW_SPI_SW_CS_INIT;
    HW_SPI_SW_CS_UP;

    GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT; // LL_GPIO_MODE_FLOATING;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
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
    SPI1->CR1 |= SPI_CR1_SPE;

    while ((SPI1->SR & SPI_SR_TXE) == 0)
        ;
}

void spi1_data_transfer(char *rx, char *tx, size_t len)
{
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

