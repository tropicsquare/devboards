#include "platform_setup.h"
#include "uart.h"
#include "irq.h"
#include "gpio.h"
#include "os.h"

#include "stm32f1xx_ll_usart.h"
// #include "stm32f1xx_ll_bus.h"

u32 uart_brr_value(u32 clk, u32 baudrate) // PCLK == 32M
{	// return BRR register value for selected baudrate
/*__STATIC_INLINE void LL_USART_SetBaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = (uint16_t)(__LL_USART_DIV_SAMPLING16(PeriphClk, BaudRate));
}*/

	return (__LL_USART_DIV_SAMPLING16(clk, baudrate));
}

__WEAK void main_wakeup(void)
{
}

#if HW_UART1_ON

char		 	u1_rx_buf[UART1_RX_BUF_DEPTH];
volatile u32	u1_rx_wr_index;
volatile u32	u1_rx_wr_cnt;
u32 			u1_rx_rd_index;
u32 			u1_rx_rd_cnt;
volatile int 	u1_rx_run;

static void _u1_rx(const char c)
{
	if ((u32)(u1_rx_wr_cnt-u1_rx_rd_cnt) >= UART1_RX_BUF_DEPTH)
	{	//buffer full
		return;
	}

	u1_rx_buf[u1_rx_wr_index]=c;

	if (++u1_rx_wr_index >= UART1_RX_BUF_DEPTH)
		u1_rx_wr_index=0;

	u1_rx_wr_cnt++;

#if HW_RS485_UART1
	if (c == 0xFF) // APR_EOP
	{	// release RS485 as soon as possible
		HW_RS485_DE_LO;
		main_wakeup();
	}
#endif // HW_RS485_UART1
}

int uart1_getchar(void)
{
	int c;

	if(u1_rx_rd_cnt == u1_rx_wr_cnt)
	{	//buffer empty
		return (-1);
	}

	c=u1_rx_buf[u1_rx_rd_index];

	if (++u1_rx_rd_index >= UART1_RX_BUF_DEPTH)
		u1_rx_rd_index=0;

	u1_rx_rd_cnt++;
	return (c);
}

char 	u1_tx_buf[UART1_TX_BUF_DEPTH];
volatile u32 u1_tx_wr_index;
volatile u32 u1_tx_wr_cnt;
volatile u32 u1_tx_rd_index;
volatile u32 u1_tx_rd_cnt;
volatile int u1_tx_run;

int uart1_putchar(const char c)
{
	if ((u32)(u1_tx_wr_cnt-u1_tx_rd_cnt) >= UART1_TX_BUF_DEPTH)
	{	//buffer full
		return (false);
	}
	u1_tx_buf[u1_tx_wr_index] = c;

	if (++u1_tx_wr_index >= UART1_TX_BUF_DEPTH)
		u1_tx_wr_index = 0;
	u1_tx_wr_cnt++;

	if (u1_tx_run == false)
	{
		u1_tx_run = true;
		USART1->CR1 |= USART_CR1_TXEIE; 
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
	USART1->DR = c;
}

#if UART1_IRQ_USED
void USART1_IRQHandler(void)
{
	u16 status;

	status=USART1->SR;

	if((status & USART_SR_TXE) && (USART1->CR1 & USART_CR1_TXEIE))
	{
		if(u1_tx_wr_cnt == u1_tx_rd_cnt)//buffer empty
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;
			u1_tx_run=false;
		}
		else
		{
			u1_tx();
		}
	}
	if(status & USART_SR_RXNE)
	{
		_u1_rx(USART1->DR);
	}
}
#endif // UART1_IRQ_USED

void uart1_init(u32 baudrate)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= UART1_GPIO_EN;

	/* USART3 GPIO Configuration
	PD5   ------> USART1_TX
	PD6   ------> USART1_RX
	*/
	GPIO_InitStruct.Pin = gpio_ll_map[UART1_TX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(UART1_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = gpio_ll_map[UART1_RX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	LL_GPIO_Init(UART1_RX_PORT, &GPIO_InitStruct);

	LL_GPIO_AF_EnableRemap_USART1();

 	USART1->BRR = uart_brr_value(UART1CLK, baudrate);

#if UART1_IRQ_USED
  	irq_enable(USART1_IRQn, UART1_ISR_PRIO);
#endif

 	USART1->CR1 = USART_CR1_UE | USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE;
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

char		 	u2_rx_buf[UART2_RX_BUF_DEPTH];
volatile u32	u2_rx_wr_index;
volatile u32	u2_rx_wr_cnt;
u32 			u2_rx_rd_index;
u32 			u2_rx_rd_cnt;
volatile int 	u2_rx_run;

static void _u2_rx(const char c)
{
	if ((u32)(u2_rx_wr_cnt-u2_rx_rd_cnt) >= UART2_RX_BUF_DEPTH)
	{	//buffer full
		return;
	}

	u2_rx_buf[u2_rx_wr_index]=c;

	if (++u2_rx_wr_index >= UART2_RX_BUF_DEPTH)
		u2_rx_wr_index=0;

	u2_rx_wr_cnt++;

#if HW_RS485_UART2
	if (c == 0xFF) // APR_EOP
	{	// release RS485 as soon as possible
		HW_RS485_DE_LO;
		main_wakeup();
	}
#endif // HW_RS485_UART2
}

int uart2_getchar(void)
{
	int c;

	if(u2_rx_rd_cnt == u2_rx_wr_cnt)
	{	//buffer empty
		return (-1);
	}

	c=u2_rx_buf[u2_rx_rd_index];

	if (++u2_rx_rd_index >= UART2_RX_BUF_DEPTH)
		u2_rx_rd_index=0;

	u2_rx_rd_cnt++;
	return (c);
}

char 	u2_tx_buf[UART2_TX_BUF_DEPTH];
volatile u32 u2_tx_wr_index;
volatile u32 u2_tx_wr_cnt;
volatile u32 u2_tx_rd_index;
volatile u32 u2_tx_rd_cnt;
volatile int u2_tx_run;

int uart2_putchar(const char c)
{
	if ((u32)(u2_tx_wr_cnt-u2_tx_rd_cnt) >= UART2_TX_BUF_DEPTH)
	{	//buffer full
		return (false);
	}
	u2_tx_buf[u2_tx_wr_index] = c;

	if (++u2_tx_wr_index >= UART2_TX_BUF_DEPTH)
		u2_tx_wr_index = 0;
	u2_tx_wr_cnt++;

	if (u2_tx_run == false)
	{
		u2_tx_run = true;
		USART2->CR1 |= USART_CR1_TXEIE; 
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
	USART2->DR = c;
}

#if UART2_IRQ_USED
void USART2_IRQHandler(void)
{
	u16 status;

	status=USART2->SR;

	if((status & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE))
	{
		if(u2_tx_wr_cnt == u2_tx_rd_cnt)//buffer empty
		{
			USART2->CR1 &= ~USART_CR1_TXEIE;
			u2_tx_run=false;
		}
		else
		{
			u2_tx();
		}
	}
	if(status & USART_SR_RXNE)
	{
		_u2_rx(USART2->DR);
	}
}
#endif // UART2_IRQ_USED

void uart2_init(u32 baudrate)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= UART2_GPIO_EN;

	GPIO_InitStruct.Pin = gpio_ll_map[UART2_TX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(UART2_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = gpio_ll_map[UART2_RX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	LL_GPIO_Init(UART2_RX_PORT, &GPIO_InitStruct);

//	LL_GPIO_AF_EnableRemap_USART2();

 	USART2->BRR = uart_brr_value(UART2CLK, baudrate);

#if UART2_IRQ_USED
  	irq_enable(USART2_IRQn,UART2_ISR_PRIO);
#endif

	USART2->CR1 = USART_CR1_UE | USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE;
}

void uart2_set_state(bool enabled)
{
	if (enabled)
	{
	  	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else
	{
	  	RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
	}

}

#endif // HW_UART2_ON

#if HW_UART3_ON

char		 	u3_rx_buf[UART3_RX_BUF_DEPTH];
volatile u32	u3_rx_wr_index;
volatile u32	u3_rx_wr_cnt;
u32 			u3_rx_rd_index;
u32 			u3_rx_rd_cnt;
volatile int 	u3_rx_run;

static void _u3_rx(const char c)
{
	if ((u32)(u3_rx_wr_cnt-u3_rx_rd_cnt) >= UART3_RX_BUF_DEPTH)
	{	//buffer full
		return;
	}

	u3_rx_buf[u3_rx_wr_index]=c;

	if (++u3_rx_wr_index >= UART3_RX_BUF_DEPTH)
		u3_rx_wr_index=0;

	u3_rx_wr_cnt++;
}

int uart3_getchar(void)
{
	int c;

	if(u3_rx_rd_cnt == u3_rx_wr_cnt)
	{	//buffer empty
		return (-1);
	}

	c=u3_rx_buf[u3_rx_rd_index];

	if (++u3_rx_rd_index >= UART3_RX_BUF_DEPTH)
		u3_rx_rd_index=0;

	u3_rx_rd_cnt++;
	return (c);
}

char 	u3_tx_buf[UART3_TX_BUF_DEPTH];
volatile u32 u3_tx_wr_index;
volatile u32 u3_tx_wr_cnt;
volatile u32 u3_tx_rd_index;
volatile u32 u3_tx_rd_cnt;
volatile int u3_tx_run;

int uart3_putchar(const char c)
{
	if ((u32)(u3_tx_wr_cnt-u3_tx_rd_cnt) >= UART3_TX_BUF_DEPTH)
	{	//buffer full
		return (false);
	}
	u3_tx_buf[u3_tx_wr_index] = c;

	if (++u3_tx_wr_index >= UART3_TX_BUF_DEPTH)
		u3_tx_wr_index = 0;
	u3_tx_wr_cnt++;

	if (u3_tx_run == false)
	{
		u3_tx_run = true;
		USART3->CR1 |= USART_CR1_TXEIE; 
	}
	return (true);
}

void u3_tx(void)
{
	char c;
	c=u3_tx_buf[u3_tx_rd_index];

	if (++u3_tx_rd_index >= UART3_TX_BUF_DEPTH)
		u3_tx_rd_index=0;

	u3_tx_rd_cnt++;
	USART3->DR = c;
}

#if UART3_IRQ_USED
void USART3_IRQHandler(void)
{
	u16 status;

	status=USART3->SR;

	if((status & USART_SR_TXE) && (USART3->CR1 & USART_CR1_TXEIE))
	{
		if(u3_tx_wr_cnt == u3_tx_rd_cnt)//buffer empty
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;
			u3_tx_run=false;
		}
		else
		{
			u3_tx();
		}
	}
	if(status & USART_SR_RXNE)
	{
		_u3_rx(USART3->DR);	
	}
}
#endif // UART3_IRQ_USED

void uart3_init(u32 baudrate)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	// Peripheral clock enable
  	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->APB2ENR |= UART3_GPIO_EN;

	GPIO_InitStruct.Pin = gpio_ll_map[UART3_TX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(UART3_TX_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = gpio_ll_map[UART3_RX_BIT];
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	LL_GPIO_Init(UART3_RX_PORT, &GPIO_InitStruct);

	LL_GPIO_AF_RemapPartial_USART3();

#if UART3_IRQ_USED
	irq_enable(USART3_IRQn, UART3_ISR_PRIO);
#endif

 	USART3->BRR = uart_brr_value(UART3CLK, baudrate);

 	USART3->CR1 = USART_CR1_UE | USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE;
}

#endif // HW_UART3_ON

