#include "hardware.h"
#include "gpio.h"
#include "spi_sw.h"


#ifndef HW_SPI_SW_CPOL
#define HW_SPI_SW_CPOL 0
#endif

#ifndef HW_SPI_SW_CPHA
#define HW_SPI_SW_CPHA 0
#endif

#if (HW_SPI_SW_CPOL == 0)
  #define SPI_CLK_IDLE HW_SPI_SW_SCK_DOWN
  #define SPI_CLK_ACT  HW_SPI_SW_SCK_UP
#else // (HW_SPI_SW_CPOL == 0)
  #define SPI_CLK_IDLE HW_SPI_SW_SCK_UP
  #define SPI_CLK_ACT  HW_SPI_SW_SCK_DOWN
#endif // (HW_SPI_SW_CPOL == 1)

static void _dly(void)
{
	//OS_DELAY_US(100);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}

void spi_sw_init(void)
{
	HW_SPI_SW_CS_INIT;
	HW_SPI_SW_SCK_INIT;
	HW_SPI_SW_MISO_INIT;
	HW_SPI_SW_MOSI_INIT;
	HW_SPI_SW_CS_UP;
	SPI_CLK_IDLE;
	HW_SPI_SW_MOSI_DOWN;
}

u8 spi_sw_byte(u8 b)
{
	int i;
	u8 result = 0;

	for (i=0; i<8; i++)
	{
		if (b & 0x80)
			HW_SPI_SW_MOSI_UP;
		else
			HW_SPI_SW_MOSI_DOWN;
		b <<=1;
		result <<=1;

	  #if (HW_SPI_SW_CPHA == 0)
		_dly();
		if (HW_SPI_SW_MISO_IN)
			result |= 1;
		SPI_CLK_ACT;
		_dly();
		SPI_CLK_IDLE;

	  #else // (HW_SPI_SW_CPHA == 0)
		SPI_CLK_ACT;
		_dly();
		if (HW_SPI_SW_MISO_IN)
			result |= 1;
		SPI_CLK_IDLE;
		_dly();
	  #endif // (HW_SPI_SW_CPHA == 1)
	}
	return (result);
}

void spi_sw_cs(bool state) // true == active (LOW)
{
	static bool cs_state = false;

	if (cs_state == state)
		return;

	if (state)
	{
		HW_SPI_SW_CS_DOWN;
		_dly();
	}
	else
	{
		_dly();
		HW_SPI_SW_CS_UP;
		SPI_CLK_IDLE;
		HW_SPI_SW_MOSI_DOWN;

	}
	cs_state = state;
}

