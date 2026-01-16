#include "platform_setup.h"
#include "uart.h"
#include "irq.h"
#include "gpio.h"
#include "os.h"

#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"

u32 uart_brr_value(u32 clk, u32 baudrate) // PCLK == 32M
{   // return BRR register value for selected baudrate
    return (__LL_USART_DIV_SAMPLING16(clk, LL_USART_PRESCALER_DIV1, baudrate));
}

#if HW_UART1_ON

char            u1_rx_buf[UART1_RX_BUF_DEPTH];
volatile u32    u1_rx_wr_index;
volatile u32    u1_rx_wr_cnt;
u32             u1_rx_rd_index;
u32             u1_rx_rd_cnt;
volatile int    u1_rx_run;

static void _u1_rx(const char c)
{
    if ((u32)(u1_rx_wr_cnt-u1_rx_rd_cnt) >= UART1_RX_BUF_DEPTH)
    {   //buffer full
        return;
    }

    u1_rx_buf[u1_rx_wr_index]=c;

    if (++u1_rx_wr_index >= UART1_RX_BUF_DEPTH)
        u1_rx_wr_index=0;

    u1_rx_wr_cnt++;
}

int uart1_getchar(void)
{
    int c;

    if(u1_rx_rd_cnt == u1_rx_wr_cnt)
    {   //buffer empty
        return (-1);
    }

    c=u1_rx_buf[u1_rx_rd_index];

    if (++u1_rx_rd_index >= UART1_RX_BUF_DEPTH)
        u1_rx_rd_index=0;

    u1_rx_rd_cnt++;
    return (c);
}

char    u1_tx_buf[UART1_TX_BUF_DEPTH];
volatile u32 u1_tx_wr_index;
volatile u32 u1_tx_wr_cnt;
volatile u32 u1_tx_rd_index;
volatile u32 u1_tx_rd_cnt;
volatile int u1_tx_run;

int uart1_putchar(const char c)
{
    if ((u32)(u1_tx_wr_cnt-u1_tx_rd_cnt) >= UART1_TX_BUF_DEPTH)
    {   //buffer full
        return (false);
    }
    u1_tx_buf[u1_tx_wr_index] = c;

    if (++u1_tx_wr_index >= UART1_TX_BUF_DEPTH)
        u1_tx_wr_index = 0;
    u1_tx_wr_cnt++;

    if (u1_tx_run == false)
    {
        u1_tx_run = true;
        USART1->CR1 |= USART_CR1_TXEIE_TXFNFIE; 
    }
    return (true);
}

void u1_tx(void)
{
    char c;
    c=u1_tx_buf[u1_tx_rd_index];

    if (++u1_tx_rd_index >= UART1_TX_BUF_DEPTH)
        u1_tx_rd_index=0;

    u1_tx_rd_cnt++;
    USART1->TDR = c;
}

#if UART1_IRQ_USED
void USART1_IRQHandler(void)
{
    u16 status;

    status=USART1->ISR;

    if((status & USART_ISR_TXE_TXFNF) && (USART1->CR1 & USART_CR1_TXEIE_TXFNFIE))
    {
        if(u1_tx_wr_cnt == u1_tx_rd_cnt)//buffer empty
        {
            USART1->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
            u1_tx_run=false;
        }
        else
        {
            u1_tx();
        }
    }
    if(status & USART_ISR_RXNE_RXFNE)
    {
        _u1_rx(USART1->RDR);
    }
}
#endif // UART1_IRQ_USED

void uart1_init(u32 baudrate)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_HSI);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    // USART1 GPIO Configuration
    // PA10  ------> USART1_RX
    // PB6   ------> USART1_TX
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  LL_GPIO_AF_EnableRemap_USART1();

    USART1->BRR = uart_brr_value(UART1CLK, baudrate);

#if UART1_IRQ_USED
    irq_enable(USART1_IRQn, UART1_ISR_PRIO);
#endif

    USART1->CR1 = USART_CR1_UE | USART_CR1_TXEIE_TXFNFIE | USART_CR1_RXNEIE_RXFNEIE | USART_CR1_TE | USART_CR1_RE;
}

void uart1_set_state(bool enabled)
{
    if (enabled)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else
    {
        RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
    }

}

#endif // HW_UART1_ON

#if HW_UART2_ON

char            u2_rx_buf[UART2_RX_BUF_DEPTH];
volatile u32    u2_rx_wr_index;
volatile u32    u2_rx_wr_cnt;
u32             u2_rx_rd_index;
u32             u2_rx_rd_cnt;
volatile int    u2_rx_run;

static void _u2_rx(const char c)
{
    if ((u32)(u2_rx_wr_cnt-u2_rx_rd_cnt) >= UART2_RX_BUF_DEPTH)
    {   //buffer full
        return;
    }

    u2_rx_buf[u2_rx_wr_index]=c;

    if (++u2_rx_wr_index >= UART2_RX_BUF_DEPTH)
        u2_rx_wr_index=0;

    u2_rx_wr_cnt++;
}

int uart2_getchar(void)
{
    int c;

    if(u2_rx_rd_cnt == u2_rx_wr_cnt)
    {   //buffer empty
        return (-1);
    }

    c=u2_rx_buf[u2_rx_rd_index];

    if (++u2_rx_rd_index >= UART2_RX_BUF_DEPTH)
        u2_rx_rd_index=0;

    u2_rx_rd_cnt++;
    return (c);
}

char    u2_tx_buf[UART2_TX_BUF_DEPTH];
volatile u32 u2_tx_wr_index;
volatile u32 u2_tx_wr_cnt;
volatile u32 u2_tx_rd_index;
volatile u32 u2_tx_rd_cnt;
volatile int u2_tx_run;

int uart2_putchar(const char c)
{
    if ((u32)(u2_tx_wr_cnt-u2_tx_rd_cnt) >= UART2_TX_BUF_DEPTH)
    {   //buffer full
        return (false);
    }
    u2_tx_buf[u2_tx_wr_index] = c;

    if (++u2_tx_wr_index >= UART2_TX_BUF_DEPTH)
        u2_tx_wr_index = 0;
    u2_tx_wr_cnt++;

    if (u2_tx_run == false)
    {
        u2_tx_run = true;
        USART2->CR1 |= USART_CR1_TXEIE_TXFNFIE; 
    }
    return (true);
}

void u2_tx(void)
{
    char c;
    c=u2_tx_buf[u2_tx_rd_index];

    if (++u2_tx_rd_index >= UART2_TX_BUF_DEPTH)
        u2_tx_rd_index=0;

    u2_tx_rd_cnt++;
    USART2->TDR = c;
}

#if UART2_IRQ_USED
void USART2_IRQHandler(void)
{
    u16 status;

    status=USART2->ISR;

    if((status & USART_ISR_TXE_TXFNF) && (USART2->CR1 & USART_CR1_TXEIE_TXFNFIE))
    {
        if(u2_tx_wr_cnt == u2_tx_rd_cnt)//buffer empty
        {
            USART2->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
            u2_tx_run=false;
        }
        else
        {
            u2_tx();
        }
    }
    if(status & USART_ISR_RXNE_RXFNE)
    {
        _u2_rx(USART2->RDR);
    }
}
#endif // UART2_IRQ_USED

void uart2_init(u32 baudrate)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_HSI);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    // USART2 GPIO Configuration
    // PA2     ------> USART2_TX
    // PA3     ------> USART2_RX
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*LL_USART_InitTypeDef USART_InitStruct = {0};

    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART2, &USART_InitStruct);*/

    USART2->BRR = uart_brr_value(UART2CLK, baudrate);

#if UART2_IRQ_USED
    irq_enable(USART2_IRQn,UART2_ISR_PRIO);
#endif

    USART2->CR1 = USART_CR1_UE | USART_CR1_TXEIE_TXFNFIE | USART_CR1_RXNEIE_RXFNEIE | USART_CR1_TE | USART_CR1_RE;
}

void uart2_set_state(bool enabled)
{
    if (enabled)
    {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    }
    else
    {
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
    }

}

#endif // HW_UART2_ON
