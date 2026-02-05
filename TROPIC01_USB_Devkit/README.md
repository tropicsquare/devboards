# TROPIC01 USB Devkit

This repository contains **hardware** design files for the TROPIC01 USB Devkit.

> [!TIP]
> **Looking for code examples, ready to get started?** Head over to [libtropic](https://github.com/tropicsquare/libtropic) and pick a tutorial that matches your interests. Whether you're new to secure elements or building production systems, you'll find step-by-step guides to get you up and running quickly.

## PCB Revisions + BOMs

Following folders contain schematics, layouts, BOMs and gerber files.

- **PCB [TS1301](./PCB/TS1301/)**
- **PCB [TS1302](./PCB/TS1302/)**
- **PCB [TS1303](./PCB/TS1303/)**

## Product Versions

| Product version | PCB Tracking  | PCB Revision  | MCU        | TROPIC01 Part Number | BOM               | Firmware                          | Picture                      | Availability |
|:----:|:------:|:----------:|:---------------:|:-------------:|:-------------------------------:|:---------------------------------:|:-------------------------------------:|:-----:|
| 3.1  | none   | **TS1303** | STM32U5 (512kB) | TR01-C2P-T103 | [BOM](./PCB/TS1303/bom/bom.csv) | [FW B](./firmwares/firmware_B/)   | [Top View](./PCB/TS1303/img/top.png)  | [BUY HERE](https://www.tropicsquare.com/order-devkit)  |
| 3.0  | none   | **TS1303** | STM32U5 (512kB) | TR01-C2P-T202 | [BOM](./PCB/TS1303/bom/bom.csv) | [FW B](./firmwares/firmware_B/)   | [Top View](./PCB/TS1303/img/top.png)  | DEPRECATED  |
| 2.0  | none   | **TS1302** | STM32U5 (256kB) | TR01-C2P-T101 | [BOM](./PCB/TS1302/bom/bom.csv) | [FW B](./firmwares/firmware_B/)   | [Top View](./PCB/TS1302/img/top.png)  | DEPRECATED  |
| 1.0  | none   | **TS1301** | STM32F1 (256kB) | TR01-B2S-T005 | [BOM](./PCB/TS1301/bom/bom.csv) | [FW A](./firmwares/firmware_A/)   | [Top View](./PCB/TS1301/img/top.png)  | DEPRECATED  |
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

## About TROPIC01

For more information about the TROPIC01 secure element, please see the official documentation:

*   [TROPIC01 Development resources](https://github.com/tropicsquare/tropic01/tree/main/)
*   [TROPIC01 Website](https://www.tropicsquare.com/tropic01)

## Support & Warranty

This devkit is provided as-is for evaluation and prototyping. No warranties are provided, and no production certifications (e.g., CE/FCC) are implied. Community-level support only; for examples and tutorials, visit [libtropic](https://github.com/tropicsquare/libtropic).
