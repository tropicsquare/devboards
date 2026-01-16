# TROPIC01 Development Boards

This repository collects all resources for development boards which are available for the TROPIC01 chip.

| Development Board        | Description                           | Resources                       |
|:------------------------:|:-------------------------------------:|:-------------------------------:|
| **USB Devkit**           | TROPIC01 evaluation over USB port     | [📁 View](./USB_Devkit/)        |
| **Raspberry Pi Shield**  | Shield for Raspberry Pi form factor   | [📁 View](./Raspberry_Pi_Shield/) |
| **Mini Board**           | Compact standalone board              | [📁 View](./Mini_Board/)        |
| **Arduino Shield**       | Shield for Arduino form factor        | [📁 View](./Arduino_Shield/)    |

## About TROPIC01

For more information about the TROPIC01 secure element, please see the official documentation:

*   [TROPIC01 Development resources](https://github.com/tropicsquare/tropic01/tree/main/)
*   [TROPIC01 Website](https://www.tropicsquare.com/tropic01)

## License

This repository uses a **multi-license structure** to accommodate different types of content:

### Hardware Designs 🔧
**License:** CERN Open Hardware Licence Version 2 - Permissive (CERN-OHL-P-2.0)  
**Applies to:** PCB designs, schematics, mechanical designs, and related documentation  
**License File:** [LICENSE.txt](LICENSE.txt)

All hardware designs in this repository (including PCB layouts in Arduino_Shield/, Raspberry_Pi_Shield/, Mini_Board/, and USB_Devkit/pcb/) are licensed under CERN-OHL-P-2.0, a permissive open hardware license that allows free use, modification, and distribution.

### Firmware & Software 💻
**License:** BSD-3-Clause-Clear (Clear BSD License)  
**Applies to:** Firmware and software developed by Tropic Square  
**License File:** [LICENSES/FIRMWARE-LICENSE.txt](LICENSES/FIRMWARE-LICENSE.txt)

Firmware code developed by Tropic Square is licensed under the Clear BSD License, a permissive open-source license.

### Third-Party Components 📦
The firmware includes third-party components with their own licenses:
- **STM32 HAL Drivers** (BSD-3-Clause) - STMicroelectronics
- **Microsoft Azure RTOS / USBX** (Microsoft Proprietary) - ⚠️ Has hardware restrictions

⚠️ **Important:** The Microsoft Azure RTOS/USBX component is licensed only for use with STM32U5 and other specifically licensed hardware. See [LICENSES/NOTICE.txt](LICENSES/NOTICE.txt) for complete details about third-party components and their licensing restrictions.

### Summary

| Component Type | License | File |
|:--------------|:--------|:-----|
| Hardware Designs | CERN-OHL-P-2.0 | [LICENSE.txt](LICENSE.txt) |
| Tropic Square Firmware | BSD-3-Clause-Clear | [LICENSES/FIRMWARE-LICENSE.txt](LICENSES/FIRMWARE-LICENSE.txt) |
| Third-Party Components | Various (See details) | [LICENSES/NOTICE.txt](LICENSES/NOTICE.txt) |

For complete license information, compliance requirements, and details about third-party components, please review:
- [LICENSE.txt](LICENSE.txt) - Hardware license
- [LICENSES/FIRMWARE-LICENSE.txt](LICENSES/FIRMWARE-LICENSE.txt) - Firmware license  
- [LICENSES/NOTICE.txt](LICENSES/NOTICE.txt) - Third-party components and compliance requirements
