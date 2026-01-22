# TROPIC01 USB Devkit

This repository contains design files for the TROPIC01 USB Devkit.

## PCB Revisions + BOMs

All hardware revisions with their design files:

- **TS1301/** - PCB revision 1.0 hardware files (schematics, layouts, BOMs, gerbers)
- **TS1302/** - PCB revision 2.0 hardware files (schematics, layouts, BOMs, gerbers)
- **TS1303/** - PCB revision 3.0 hardware files (schematics, layouts, BOMs, gerbers)

Each folder contains complete KiCad project files, schematics, PCB layouts, BOMs, and Gerber manufacturing files.

## Product Versions

| Product version | PCB Marking | PCB Revision | MCU             | TROPIC01 Part Number | BOM                             | Firmware                          | Picture                                 | Order |
|:-------:|:-----------:|:------------:|:---------------:|:--------------------:|:-------------------------------:|:---------------------------------:|:---------------------------------------:|:-----:|
| 1.0     | none *([More info..](../IDENTIFY_DEVBOARD.md))*       | **TS1301**     | STM32F1 (256kB) | TR01-C2P-T103        | [BOM](./PCB/TS1301/bom/bom.csv) | [FW A](./legacy_fw/firmware_A/)   | [Top View](./PCB/TS1301/img/top.png)  | link  |
| 2.0     | none *([More info..](../IDENTIFY_DEVBOARD.md))*        | **TS1302**     | STM32U5 (256kB) | TR01-C2P-T103        | [BOM](./PCB/TS1302/bom/bom.csv) | [FW B](./legacy_fw/firmware_B/)   | [Top View](./PCB/TS1302/img/top.png)  | link  |
| 3.0     | 25057-XXXX<br>25058-XXXX  | **TS1303**     | STM32U5 (512kB) | TR01-C2P-T301        | [BOM](./PCB/TS1303/bom/bom.csv) | [FW B](./legacy_fw/firmware_B/)   | [Top View](./PCB/TS1303/img/top.png)  | link  |

## PCB Specifications

Common specifications for all revisions:
- **Layers**: 2-layer
- **Thickness**: 1.0 mm
- **Material**: FR4
- **Solder Mask**: Blue
- **Silkscreen**: Yes (top and bottom)

### Assembly Notes

Standard SMD components (0805 size):
- Resistor tolerance: 5% (unless specified)
- Capacitor tolerance: 20% (unless specified)

## Getting Started

Ready to explore TROPIC01? Head over to [libtropic](https://github.com/tropicsquare/libtropic) and pick a tutorial that matches your interests. Whether you're new to secure elements or building production systems, you'll find step-by-step guides to get you up and running quickly.

## About TROPIC01

For more information about the TROPIC01 secure element, please see the official documentation:

*   [TROPIC01 Development resources](https://github.com/tropicsquare/tropic01/tree/main/)
*   [TROPIC01 Website](https://www.tropicsquare.com/tropic01)
