# About

TS15 – TROPIC01 Raspberry Pi Shield
A KiCad-based hardware project designed to connect the TROPIC01 device to a Raspberry Pi via the standard 40-pin GPIO header.

This shield enables straightforward integration of TROPIC01 into Raspberry Pi-based development and testing environments.
It is intended for development, evaluation, and prototyping use.

# Project structure

* `*.kicad_pro` - KiCad project file
* `*.kicad_sch` - Schematic file
* `*.kicad_pcb` - PCB layout file
* `./out/` -  Generated Gerber files for PCB manufacturing
* `./bom/` - Bill of materials, including order codes and interactive BOM
* `./img/` - Images (e.g. PCB renders)
* `*_schematics.pdf` - Exported schematic diagrams (latest version)

# Manufacturing instructions:

## Assembly

IC1/CON1: Chose one variant, assembly IC1 or CON1 (not both on one board).
Chose `bom/bom_variant_*`.

J1: Because the Raspberry Pi has male GPIO pins, the shield's socket must be assembled on the bottom side 
to connect properly when placed on top of the Pi. \

J2: Jumper place to position CS2

There are no any special requirements for C and R components.
Where not specified the R tolerance is 5% and C 20%.

## PCB

* Number of cu layers: 2
* Board Thickness: 1.6
* Core: FR4
* Size: 56 x 65mm
* Mask: Blue
* Silkscreen: Yes (TOP only)
