#include "common.h"
#include "hardware.h"
#include "irq.h"
#include "dma.h"

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_dmamux.h"

static bool _dma_spi_tx_done = false;
static bool _dma_spi_rx_done = false;

bool dma_init(void)
{
    // DMA controller clock enable 
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    return (true);
}

void dma_init_spi_rx(void)
{   // DMA1 channel 2
    DMA1_Channel2->CCR = 0; // ensure OFF
   
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_SPI1_RX);

    DMA1_Channel2->CCR = DMA_CCR_MINC;
    DMA1_Channel2->CCR |= DMA_CCR_TCIE;
    DMA1_Channel2->CPAR = (u32)&(SPI1->DR);

    // DMA1_Channel2_IRQn interrupt configuration
    irq_enable(DMA1_Channel2_IRQn, 0); // SPI RX
}

void dma_init_spi_tx(void)
{   // DMA1 channel 3
    DMA1_Channel3->CCR = 0; // ensure OFF
    
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMAMUX_REQ_SPI1_TX);

    DMA1_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_DIR;
    DMA1_Channel3->CCR |= DMA_CCR_TCIE;
    DMA1_Channel3->CPAR = (u32)&(SPI1->DR);

    // DMA1_Channel3_IRQn interrupt configuration
    irq_enable(DMA1_Channel3_IRQn, 0); // SPI TX
}

void dma_spi_rx_start(char *dest, size_t len)
{
    _dma_spi_rx_done = false;

    DMA1->IFCR |= DMA_IFCR_CGIF2 | DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CTEIF2;
    DMA1_Channel2->CMAR = (u32)dest;
    DMA1_Channel2->CNDTR = len;

    DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void dma_spi_tx_start(char *src, size_t len)
{
    _dma_spi_tx_done = false;

    DMA1->IFCR |= DMA_IFCR_CGIF3 | DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CTEIF3;
    DMA1_Channel3->CMAR = (u32)src;
    DMA1_Channel3->CNDTR = len;

    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

bool dma_spi_rx_done(void)
{
    return (_dma_spi_rx_done);
}

bool dma_spi_tx_done(void)
{
    return (_dma_spi_tx_done);
}

void DMA1_Channel2_IRQHandler(void)
{
    u32 isr = DMA1->ISR;
    u32 ccr = DMA1_Channel2->CCR;

    // Half Transfer Complete Interrupt management 
    if ((isr & DMA_ISR_HTIF2) && (ccr & DMA_CCR_HTIE))
    {
        DMA1->IFCR |= DMA_IFCR_CHTIF2;
    }
    // Transfer Complete Interrupt management
    if ((isr & DMA_ISR_TCIF2) && (ccr & DMA_CCR_TCIE))
    {
        DMA1_Channel2->CCR &= ~DMA_CCR_EN;
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        _dma_spi_rx_done = true;
    }
    // Transfer Error Interrupt management
    if ((isr & DMA_ISR_TEIF2) && (ccr & DMA_CCR_TEIE))
    {
        DMA1->IFCR |= DMA_IFCR_CTEIF2;
    }
}

void DMA1_Channel3_IRQHandler(void)
{
    u32 isr = DMA1->ISR;
    u32 ccr = DMA1_Channel3->CCR;

    // Half Transfer Complete Interrupt management 
    if ((isr & DMA_ISR_HTIF3) && (ccr & DMA_CCR_HTIE))
    {
        DMA1->IFCR |= DMA_IFCR_CHTIF3;
    }
    // Transfer Complete Interrupt management
    if ((isr & DMA_ISR_TCIF3) && (ccr & DMA_CCR_TCIE))
    {
        DMA1_Channel3->CCR &= ~DMA_CCR_EN;
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
        _dma_spi_tx_done = true;
    }
    // Transfer Error Interrupt management
    if ((isr & DMA_ISR_TEIF3) && (ccr & DMA_CCR_TEIE))
    {
        DMA1->IFCR |= DMA_IFCR_CTEIF3;
    }
}
