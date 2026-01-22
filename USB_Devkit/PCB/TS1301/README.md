# TS1301 – USB Devkit (Revision 1.0)

KiCad-based hardware project providing a development platform for the TROPIC01 chip via USB interface.

## Features

- **USB-C interface** with STM32F1 microcontroller (256kB)
- **Firmware**: [Firmware A](./../../legacy_fw/firmware_A/README.md) acts as USB-to-SPI converter for host communication with TROPIC01
- **PCB Marking**: none - See [how to identify](../../../../IDENTIFY_DEVBOARD.md)

## Getting Started

Ready to use this devkit? Follow tutorials in [libtropic](https://github.com/tropicsquare/libtropic) to get started with TROPIC01.

## Project Structure

| File/Folder        | Description                                            |
|:--------------     |:-------------------------------------------------------|
| `*.kicad_pro`      | KiCad project file                                     |
| `*.kicad_sch`      | Schematic file                                         |
| `*.kicad_pcb`      | PCB layout file                                        |
| `*_schematics.pdf` | Exported schematic diagrams                            |
| `./out/`           | Gerber files for PCB manufacturing                     |
| `./bom/`           | Bill of materials with order codes and interactive BOM |
| `./img/`           | PCB renders and images                                 |

## Manufacturing

### Assembly Notes

Standard SMD components (0805 size):
- Resistor tolerance: 5% (unless specified)
- Capacitor tolerance: 20% (unless specified)

### PCB Specifications

| Parameter          | Value                                                  |
|:-------------------|:-------------------------------------------------------|
| Layers             | 2-layer                                                |
| Thickness          | 1.0 mm                                                 |
| Material           | FR4                                                    |
| Size               | 20 × 60 mm                                             |
| Solder Mask        | Blue                                                   |
| Silkscreen         | Yes (top and bottom)                                   |


