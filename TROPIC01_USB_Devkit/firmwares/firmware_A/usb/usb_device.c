#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

#include "stm32f1xx_ll_rcc.h"

#include "main.h"
#include "gpio.h"
#include "sys.h"


extern PCD_HandleTypeDef hpcd_USB_FS;

// USB Device Core handle declaration.
USBD_HandleTypeDef hUsbDeviceFS;

void usb_clock_init(void)
{
    sys_usb_clock_config();
}

void usb_device_init(void)
{
    // do USB host driver reset by pulling DP to low
    GPIO_BIT_CLR(HW_USB_DP_PORT, HW_USB_DP_BIT);
    GPIO_PIN_INIT(HW_USB_DP_PORT, HW_USB_DP_BIT, GPIO_MODE_OUTPUT);
    OS_DELAY(5);

    usb_clock_init();

    // Init Device Library, add supported class and start the library.
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
    {
        Error_Handler();
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
    {
        Error_Handler();
    }
    if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
    {
        Error_Handler();
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
        Error_Handler();
    }
}

bool usb_device_connected(void)
{
    if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
        return (false);

    if (hUsbDeviceFS.pClassData == NULL)
        return (false);

    return (true);
}

void USB_LP_CAN1_RX0_IRQHandler(void) // == USB_LP_IRQHandler
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

