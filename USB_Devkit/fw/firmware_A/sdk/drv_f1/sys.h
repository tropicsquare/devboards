#ifndef SYS_H
#define	SYS_H

#include "common.h"

void sys_init(void);
void sys_clock_config(void);
void sys_usb_clock_config(void);
u32  sys_flash_size(void);


#endif // ! SYS_H

