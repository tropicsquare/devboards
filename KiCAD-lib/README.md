# TROPIC01 KiCAD Library

This directory contains support files for designing PCB with TROPIC01, **Tested with KiCAD 9.0**.

<img src="KiCAD_banner_TROPIC01.png" alt="KiCAD 3D Model Configuration" width="50%">

## Content

- `TROPIC01.kicad_sym` - Symbol library containing the TROPIC01 chip schematic symbol
- `TROPIC01.step` - 3D STEP model for the TROPIC01 chip package
- `logos.pretty/` - Tropic Square logo and branding, provided as footprints, ready to be used in silkscreen

As footprint we recommend to use standard `Package_DFN_QFN:QFN-32-1EP_4x4mm_P0.4mm_EP2.65x2.65mm` provided by KiCAD.

## Installation and Usage as Project-Specific Libraries (Recommended)

This method keeps the libraries just with your project. For more details refer to original KiCAD documentation about how to use libraries.

1. **Copy the KiCAD-lib folder** into your KiCAD project directory or a subdirectory (e.g., `libraries/`)

2. **Add the Symbol Library:**
  - Open your KiCAD project
  - Go to `Preferences` → `Manage Symbol Libraries...`
  - In the "Project Specific Libraries" tab, click the folder icon to add a new library
  - Browse to and select `TROPIC01.kicad_sym`
  - Give it a nickname (e.g., "TROPIC_SQUARE")
  - Click OK

3. **Add the Footprint Library:**
  - Go to `Preferences` → `Manage Footprint Libraries...`
  - In the "Project Specific Libraries" tab, click the folder icon to add a new library
  - Browse to and select the `logos.pretty` folder
  - Give it a nickname (e.g., "TROPIC_SQUARE"_Logos")
  - Click OK

4. **Configure 3D Model Path:**

By default KiCAD will use 3D model for used QFN32 package, but it is possible to change this model to the one provided by us, so you could see a palm tree on TROPIC01 package on your PCB when you render PCB in 3D viewer. To reference our 3D model instead of the one provided with official QFN32 footprint, you need to update the path:
  - Open the PCB editor
  - Select a TROPIC01 footprint
  - Right-click → `Properties` → `3D Models` tab
  - Update the path to point to provided `TROPIC01.step`
  - Rendered package together with silkscreen logo:


## Adding Logo Footprints to PCB

The logo footprints are designed for PCB silkscreen branding. They include various sizes to fit different board designs. These footprints contain only graphical elements and have no electrical connections.

1. Open the PCB Editor
2. Click the "Add Footprint" button (or press `O`)
3. In the footprint chooser, search for "tropicsquare"
4. Select the desired logo variant:
   - `tropicsquare_logo` - Full logo (default size)
   - `tropicsquare_logo_7` - Logo, 7mm
   - `tropicsquare_logo_8` - Logo, 8mm
   - `tropicsquare_name_7` - Company name text, 7mm
   - `tropicsquare_palm` - Palm icon (default size)
   - `tropicsquare_palm_8` - Palm icon, 8mm
   - `tropicsquare_palm_10` - Palm icon, 10mm
5. Place it on your PCB (typically on the silkscreen layer)

## License

See the LICENSE.txt file in the root directory for licensing information.
