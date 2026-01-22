#include "common.h"
#include "hw_info.h"
#include "hardware.h"

#if defined BOOTLOADER

#ifndef HW_ADDR
  #warning "HW_ADDR not defined, using default"
  #define HW_ADDR     1000001 // 0xFFFFFFFF
#endif

hw_info_t HW_INFO __attribute__((section(".HW_INFO"))) = {
        .addr           = HW_ADDR,
        .hw_version     = HW_VERSION_CUR,
        .device_id      = HW_DEVICE_ID,
        .hw_name        = HW_NAME,
        .calibration    = { // for future use, possible HW calibration
			HW_CALIBRATIONS_DEFAULT
		}
};

#else // BOOTLOADER

hw_info_t HW_INFO __attribute__((section(".HW_INFO"))) __attribute__((used)) = {0xFF};

#endif // ! BOOTLOADER

