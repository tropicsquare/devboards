# TROPIC01 devboards
Welcome, PCB designers! 

This repository collects all resources for development boards which we designed with our TROPIC01 secure element. All PCB designs are freely available under **[CERN-OHL-P-2.0](https://ohwr.org/cernohl) Open Hardware license**. We encourage you to re-use designs, modify, or build your own boards!

> [!TIP]
> **Looking for code examples, ready to get started?** Head over to [libtropic](https://github.com/tropicsquare/libtropic) and pick a tutorial that matches your interests. Whether you're new to secure elements or building production systems, you'll find step-by-step guides to get you up and running quickly.

| Product name             | Image  | Description                           | HW resources                   |
|:------------------------:|:------:|:-------------------------------------:|:------------------------------:|
| **USB Devkit**           | <a href="USB_Devkit/PCB/TS1303/img/TS1303_USB_Devkit_prod_photo.png"><img src="USB_Devkit/PCB/TS1303/img/TS1303_USB_Devkit_prod_photo.png" width="100"></a> | For TROPIC01 evaluation over USB port     | [View](./USB_Devkit/)          |
| **Raspberry Pi Shield**  | <a href="Raspberry_Pi_Shield/TS1501/img/TS1501_Raspberry_Pi_Shield_prod_photo.png"><img src="Raspberry_Pi_Shield/TS1501/img/TS1501_Raspberry_Pi_Shield_prod_photo.png" width="100"></a> | Shield for Raspberry Pi form factor   | [View](./Raspberry_Pi_Shield/) |
| **Mini Board**           | <a href="Mini_Board/TS1701/img/TS1701_Mini_Board_prod_photo.png"><img src="Mini_Board/TS1701/img/TS1701_Mini_Board_prod_photo.png" width="100"></a> | Compact PCB board                    | [View](./Mini_Board/)          |
| **Arduino Shield**       | <a href="Arduino_Shield/TS1401/img/TS1401_Arduino_Shield_prod_photo.png"><img src="Arduino_Shield/TS1401/img/TS1401_Arduino_Shield_prod_photo.png" width="100"></a> | Shield for Arduino form factor        | [View](./Arduino_Shield/)      |

## Using These Designs

Each particular board folder contains complete design files for all released revisions:
- **Schematics** (`.kicad_sch`) - Showing how components are wired together
- **PCB layouts** (`.kicad_pcb`) - Design of the PCB - how components are physically placed on the PCB
- **BOMs** - Component lists with part numbers
- **Gerbers** - Manufacturing files for PCB fabrication

You're free to use these as reference, modify for your needs, or manufacture your own boards.

> [!IMPORTANT]
> Some devboards include firmware in their folders. Note that firmware or its parts may be under various licensing schemes - see the [licensing section](#license) below for details.

## Contributing

Found an improvement? We welcome contributions! Please open an issue or pull request with:
- Clear description of changes
- Updated schematics/layouts if applicable

## FAQ

See [FAQ.md](FAQ.md)

## About TROPIC01

For more information about the TROPIC01 secure element, please see the official documentation:

*   [TROPIC01 Development resources](https://github.com/tropicsquare/tropic01/tree/main/)
*   [TROPIC01 Website](https://www.tropicsquare.com/tropic01)

## License

This repository does not contain only PCB resources, but in some cases also HW which comes with devkit. Each part is under different license, more details in following table:

| Component        | License  | Applies To                                                                                          |
|:-----------------|:---------|:----------------------------------------------------------------------------------------------------|
| **PCB Hardware**            | [CERN-OHL-P-2.0](LICENSE.txt) | Arduino_Shield/, Raspberry_Pi_Shield/, Mini_Board/, USB_Devkit/PCB/ |
| **Firmware**                | [BSD-3-Clause-Clear](LICENSES/FIRMWARE-LICENSE.txt) | USB_Devkit/legacy_fw/                         |
| **Third-party USB library** | Specific | USB_Devkit/legacy_fw/firmware_B/sdk/ - See [LICENSES/NOTICE.txt](LICENSES/NOTICE.txt)    |
