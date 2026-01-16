#ifndef TPDI_H
#define TPDI_H

#include "common.h"

void tpdi_hw_init(void);
void tpdi_hw_connect(void);
void tpdi_hw_disconnect(void);
bool tpdi_hw_ready(void);
bool tpdi_data_send(u32 addr, u32 *src, size_t len);
bool tpdi_data_read(u32 *dest, u32 addr, size_t len);
bool tpdi_delay_set(u32 value);
u32  tpdi_delay_get(void);

#endif // ! TPDI_H

