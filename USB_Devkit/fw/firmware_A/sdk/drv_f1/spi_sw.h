#ifndef SPI_SW_H
#define SPI_SW_H

#include "type.h"

void spi_sw_init(void);

u8 spi_sw_byte(u8 b);

void spi_sw_cs(bool state);

#endif // !SPI_SW_H

