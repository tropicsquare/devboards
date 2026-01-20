# TROPIC01 Development Boards

This repository collects all resources for development boards which are available for the TROPIC01 secure element.

| Development Board        | Description                           | PCB resources                  |
|:------------------------:|:-------------------------------------:|:------------------------------:|
| **USB Devkit**           | TROPIC01 evaluation over USB port     | [View](./USB_Devkit/)          |
| **Raspberry Pi Shield**  | Shield for Raspberry Pi form factor   | [View](./Raspberry_Pi_Shield/) |
| **Mini Board**           | Compact standalone board              | [View](./Mini_Board/)          |
| **Arduino Shield**       | Shield for Arduino form factor        | [View](./Arduino_Shield/)      |


## I got a devboard, now what?

Section here.

## I want to design PCB

Symbol, 3D model and Tropic Square logos for silkscreen for **KiCAD** are available in `KiCAD-lib/` folder.

## About TROPIC01

For more information about the TROPIC01 secure element, please see the official documentation:

*   [TROPIC01 Development resources](https://github.com/tropicsquare/tropic01/tree/main/)
*   [TROPIC01 Website](https://www.tropicsquare.com/tropic01)

## License

This repository contains not only PCB rsources, but in same cases also HW which comes with devkit. Each part is under different license.

| Component | License | Applies To |
|:----------|:--------|:-----------|
| **PCB Hardware** | [CERN-OHL-P-2.0](LICENSE.txt) | Arduino_Shield/, Raspberry_Pi_Shield/, Mini_Board/, USB_Devkit/pcb/ |
| **Firmware** | [BSD-3-Clause-Clear](LICENSES/FIRMWARE-LICENSE.txt) | USB_Devkit/legacy_fw/ |
| **Third-party USB library** | Specific | USB_Devkit/legacy_fw/firmware_B/sdk/ - See [LICENSES/NOTICE.txt](LICENSES/NOTICE.txt) |
