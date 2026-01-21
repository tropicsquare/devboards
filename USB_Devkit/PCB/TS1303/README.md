# About

TS13 – TROPIC01 USB Development Kit
A KiCad-based hardware project providing a minimal development platform for the TROPIC01 chip.

The TS13 features a USB-C interface and an STM32 microcontroller acting as a USB-to-SPI converter,
enabling straightforward host communication with TROPIC01.

Devkit was distributed with [this](./../../legacy_fw/firmware_B/README.md) firmware.

# Project structure

`*.kicad_pro` - KiCad project file \
`*.kicad_sch` - Schematic file \
`*.kicad_pcb` - PCB layout file \
`./out/` - Generated gerber data for PCB manufacturing \
`./bom/` - Bill of materials, including order codes and interactive BOM \
`./img/` - Images (PCB renders) \
`*_schematics.pdf` - Exported schematic diagrams (latest version) \

# Manufacturing instructions

## Assembly

There are no any special requirements for C and R components.
Where not specified the R tolerance is 5% and C 20%.

## PCB

  Number of cu layers: 2 \
  Board Thickness: 1.0 \
  Core: FR4 \
  Size: 20 x 46 mm \
  Mask: Blue \
  Silkscreen: Yes (TOP and BOTTOM)


