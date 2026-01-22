#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"
#include "usbd_cdc_if.h"


void usb_device_init(void);
bool usb_device_connected(void);

// compatibility between STM32_USB_Device_Library and STM32_USBX_Library
#define USB_RESULT_BUSY USBD_BUSY
#define usb_cdc_tx_busy usbd_cdc_tx_busy
#define usb_cdc_tx      usbd_cdc_tx
#define usb_cdc_rx_init usbd_cdc_rx_init

#ifdef __cplusplus
}
#endif

#endif // ! USB_DEVICE_H

