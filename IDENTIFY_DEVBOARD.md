# How to Identify Your TROPIC01 Development Board

Some early devboards were distributed without proper markings and may contain TROPIC01 engineering samples.

## Identification Process

1. **Check PCB markings** on the board's silkscreen (top/bottom, edges, or corners)
2. **Compare with tables** in each devboard folder:
   - [USB_Devkit/README.md](./USB_Devkit/README.md)
   - [Arduino_Shield/README.md](./Arduino_Shield/README.md)
   - [Raspberry_Pi_Shield/README.md](./Raspberry_Pi_Shield/README.md)
   - [Mini_Board/README.md](./Mini_Board/README.md)
3. **If you still cannot identify your board**, you probably have one of early engineering devices
   - Use "identify chip" tutorial in [libtropic](https://github.com/tropicsquare/libtropic)
   - The TROPIC01 part number will appear in the debug output

## Troubleshooting

**No markings found?** Check the board's physical shape and form factor, then use the libtropic tutorial to identify the chip.

**Custom or pre-production board?** Contact Tropic Square support with a photo of your board.