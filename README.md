# TROPIC01 Development Boards

Welcome, PCB designers! This repository collects all resources for development boards which are available for the TROPIC01 secure element.

| Product name             | Image  | Description                           | HW resources                   |
|:------------------------:|:------:|:-------------------------------------:|:------------------------------:|
| **USB Devkit**           | <a href="USB_Devkit/PCB/TS1302/img/top_assembled.png"><img src="USB_Devkit/PCB/TS1302/img/top_assembled.png" width="100"></a> | For TROPIC01 evaluation over USB port     | [View](./USB_Devkit/)          |
| **Raspberry Pi Shield**  | <a href="Raspberry_Pi_Shield/TS1501_design/img/top_assembled.png"><img src="Raspberry_Pi_Shield/TS1501_design/img/top_assembled.png" width="100"></a> | Shield for Raspberry Pi form factor   | [View](./Raspberry_Pi_Shield/) |
| **Mini Board**           | <a href="Mini_Board/TS1701_PCB/img/top_assembled.png"><img src="Mini_Board/TS1701_PCB/img/top_assembled.png" width="100"></a> | Compact PCB board                    | [View](./Mini_Board/)          |
| **Arduino Shield**       | <a href="Arduino_Shield/TS1401_design/img/top_assembled.png"><img src="Arduino_Shield/TS1401_design/img/top_assembled.png" width="100"></a> | Shield for Arduino form factor        | [View](./Arduino_Shield/)      |


## I got a devboard, now what?

TBD, describe more the process how first steps might look.
* Identify devboard
* Follow tutorials in [libtropic](https://www.github.com/tropicsquare/libtropic/)

## I want to design PCB

TROPIC01 `Schematic symbol`, `3D model` and `Tropic Square logos` for silkscreen are available in `KiCad-lib/` folder, ready to be used with **KiCad** EDA tool. Check out [README.md](KiCad-lib/README.md) for details.

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
