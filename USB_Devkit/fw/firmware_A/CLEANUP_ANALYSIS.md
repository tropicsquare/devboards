# Firmware_A SDK Cleanup Analysis

## Current Situation

Firmware_A uses **STM32F103xB** (Cortex-M3) microcontroller and only compiles with STM32F1xx HAL drivers.

## What's Actually Used (from sdk_stm32f103xb.mk)

**Used HAL Driver:**
- `sdk/stm32/STM32F1xx_HAL_Driver/` - STM32F1 series HAL (USED ✓)

**Used USB Library:**
- `sdk/stm32/STM32_USB_Device_Library/` - STM32 USB Device Library (USED ✓)

**Used HAL Source Files:**
```
stm32f1xx_ll_adc.c
stm32f1xx_ll_dma.c
stm32f1xx_ll_exti.c
stm32f1xx_ll_gpio.c
stm32f1xx_ll_pwr.c
stm32f1xx_ll_rcc.c
stm32f1xx_ll_rtc.c
stm32f1xx_ll_spi.c
stm32f1xx_ll_tim.c
stm32f1xx_ll_usb.c
stm32f1xx_ll_utils.c
stm32f1xx_hal_cortex.c
stm32f1xx_hal_pcd.c
stm32f1xx_hal_pcd_ex.c
```

## UNUSED Components (Can Be REMOVED)

### 1. STM32L4xx HAL Driver - UNUSED ❌
- **Path:** `sdk/stm32/STM32L4xx_HAL_Driver/`
- **Reason:** Firmware uses STM32F103xB, not STM32L4 series
- **Size:** ~50+ MB with all sources

### 2. STM32U5xx HAL Driver - UNUSED ❌
- **Path:** `sdk/stm32/STM32U5xx_HAL_Driver/`
- **Reason:** Firmware uses STM32F103xB, not STM32U5 series
- **Size:** ~50+ MB with all sources

### 3. STM32_USBX_Library - UNUSED ❌
- **Path:** `sdk/stm32/STM32_USBX_Library/`
- **Reason:** Firmware uses STM32_USB_Device_Library, not USBX (Azure RTOS)
- **Size:** Several MB

## Cleanup Commands

```bash
cd /home/ppolach/Desktop/AAA_devkits/tropic01-devboards/USB_Devkit/legacy_fw/firmware_A/sdk/stm32

# Remove unused STM32L4 HAL driver
rm -rf STM32L4xx_HAL_Driver/

# Remove unused STM32U5 HAL driver
rm -rf STM32U5xx_HAL_Driver/

# Remove unused Azure RTOS USBX library
rm -rf STM32_USBX_Library/
```

## What Will Remain

After cleanup, `sdk/stm32/` will contain only:
- `CMSIS/` - Core ARM CMSIS files (needed)
- `STM32F1xx_HAL_Driver/` - Used HAL driver
- `STM32_USB_Device_Library/` - Used USB library
- `platform_setup.h` - Configuration file

## Impact

- **Space saved:** ~100+ MB
- **Compilation:** No change, firmware will build normally
- **Functionality:** No change, only unused code removed

## Verification

After cleanup, test that firmware still builds:
```bash
cd app
make clean
make
```

Ready to execute cleanup?
