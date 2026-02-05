# How to Identify Your TROPIC01 Development Board

Each Dev Board has a identification present on PCB in silkscreen or exposed copper as a text.

## PCB marking meaning:
   - **PCB Revision** - Defines the PCB revision.
      - **TS1701** - Mini Board PCB revision 1
      - **TS1303** - USB Devkit PCB revision 3
   - **PCB Tracing** - Links PCB revision with a bom and production.
      - **none** - Some PCBs do not have any additional Tracking data present. :(
      - **E254630 SL1** - Tracking present on Arduino Shield PCB.

Some early devboards were distributed without proper marking and may contain TROPIC01 engineering samples.

## Identification Process

1. **Check PCB marking** on the board's text (top/bottom, edges, or corners)
 
2. **Compare with tables** in each devboard folder:
   - [TROPIC01_USB_Devkit/README.md](./TROPIC01_USB_Devkit/README.md)
   - [TROPIC01_Arduino_Shield/README.md](./TROPIC01_Arduino_Shield/README.md)
   - [TROPIC01_Raspberry_Pi_Shield/README.md](./TROPIC01_Raspberry_Pi_Shield/README.md)
   - [TROPIC01_Mini_Board/README.md](./TROPIC01_Mini_Board/README.md)
3. **If you still cannot identify your board**, you probably have one of early engineering devices
   - Use "identify chip" tutorial in [libtropic](https://github.com/tropicsquare/libtropic)
   - The TROPIC01 part number will appear in the debug output

## Troubleshooting

**No marking found?** Check the board's physical shape and form factor, then use the libtropic tutorial to identify the chip.

**Custom or pre-production board?** Contact Tropic Square support with a photo of your board.