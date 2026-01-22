#ifndef HW_INFO_H
#define	HW_INFO_H

#include "common.h"
#include "hardware.h"


typedef struct {
	s16 add;  // value = value + add
	s16 mult; // value = value * (32768 + mult)/32768
} hw_info_calibration_t;

typedef struct {

/* 0x7F00 */ u64   addr;
/* 0x7F08 */ u16   hw_version;     // compatibility version
/* 0x7F0A */ u16   device_id;      // device type 
/* 0x7F0C */ ascii hw_name[16];    //

             u8    res[256-(0x1C)];  // padding to 256B

} hw_info_t;

extern hw_info_t HW_INFO;

#endif // ! HW_INFO_H
