# License Structure - TROPIC01 Development Boards

## Implementation Summary

**Date:** January 2026  
**Status:** ✅ Multi-license structure implemented

This document provides a visual map of the license structure in this repository.

---

## File Structure

```
tropic01-devboards/
│
├── LICENSE.txt                           [CERN-OHL-P-2.0] Hardware designs
├── README.md                             Updated with license info
├── LICENSES/
│   ├── FIRMWARE-LICENSE.txt              [BSD-3-Clause-Clear] Firmware code
│   ├── NOTICE.txt                        Third-party components notices
│   ├── LICENSE_GUIDE.md                  Comprehensive license guide
│   └── LICENSE_STRUCTURE.md              This file
│
├── TROPIC01_Arduino_Shield/
│   ├── LICENSE.txt                       [CERN-OHL-P-2.0] PCB designs
│   └── TS1401/                           Hardware files
│
├── TROPIC01_Raspberry_Pi_Shield/
│   ├── LICENSE.txt                       [CERN-OHL-P-2.0] PCB designs
│   └── TS1501/                           Hardware files
│
├── TROPIC01_Mini_Board/
│   └── TS1701/
│       ├── LICENSE.txt                   [BSD-3-Clause-Clear] (existing)
│       └── (hardware files)
│
└── TROPIC01_USB_Devkit/
    ├── PCB/
    │   ├── LICENSE.txt                   [CERN-OHL-P-2.0] PCB designs
    │   └── (hardware files)
    │
    └── firmwares/
        └── firmware_B/
            ├── LICENSE.txt               [BSD-3-Clause-Clear] Application code
            ├── NOTICE.txt                Third-party notices
            ├── app/                      [BSD-3-Clause-Clear] Your code
            └── sdk/
                └── stm32/
                    ├── STM32U5xx_HAL_Driver/
                    │   └── LICENSE.md    [BSD-3-Clause] STMicroelectronics
                    └── STM32_USBX_Library/
                        ├── LICENSE.txt   [Microsoft Proprietary] ⚠️
                        └── LICENSED-HARDWARE.txt
```

---

## License Application Map

### 🔧 CERN-OHL-P-2.0 (Hardware Designs)

**Scope:** All hardware design files  
**Files:**
- `../LICENSE.txt` (root)
- `../TROPIC01_Arduino_Shield/LICENSE.txt`
- `../TROPIC01_Raspberry_Pi_Shield/LICENSE.txt`
- `../TROPIC01_USB_Devkit/PCB/LICENSE.txt`

**Applies to:**
- `.kicad_pcb` - PCB layouts
- `.kicad_sch` - Schematics
- `.kicad_pro` - Project files
- BOM files - Bill of Materials
- Gerber files - Manufacturing data
- Hardware documentation and images

### 💻 BSD-3-Clause-Clear (Tropic Square Firmware)

**Scope:** Firmware code authored by Tropic Square  
**Files:**
- `LICENSES/FIRMWARE-LICENSE.txt`
- `../TROPIC01_USB_Devkit/firmwares/firmware_B/LICENSE.txt`
- `../TROPIC01_Mini_Board/TS1701/LICENSE.txt`

**Applies to:**
- Application code (`app/`)
- Custom drivers
- Hardware interface code
- Configuration files by Tropic Square

### 📦 BSD-3-Clause (STMicroelectronics)

**Scope:** STM32 HAL drivers  
**File:** `TROPIC01_USB_Devkit/firmwares/firmware_B/sdk/stm32/STM32U5xx_HAL_Driver/LICENSE.md`

**Applies to:**
- HAL driver source files
- STM32-specific low-level code

### ⚠️ Microsoft Proprietary (Azure RTOS/USBX)

**Scope:** USB stack library  
**File:** `TROPIC01_USB_Devkit/firmwares/firmware_B/sdk/stm32/STM32_USBX_Library/LICENSE.txt`

**Applies to:**
- USBX library code
- Azure RTOS components

**Restriction:** Licensed only for use with STM32U5 and other approved hardware

---

## Documentation Files

| File | Purpose |
|:-----|:--------|
| `../LICENSE.txt` | Main hardware license (CERN-OHL-P-2.0) |
| `LICENSES/FIRMWARE-LICENSE.txt` | Firmware license (BSD-3-Clause-Clear) |
| `LICENSES/NOTICE.txt` | Complete third-party component notices |
| `LICENSES/LICENSE_GUIDE.md` | Comprehensive user guide |
| `LICENSES/LICENSE_STRUCTURE.md` | This file - visual map of license structure |
| `../README.md` | Updated with license summary |

---

## Quick Decision Tree

### "I want to manufacture boards"
→ Use: Hardware designs under CERN-OHL-P-2.0  
→ Requirements: Attribution to Tropic Square, share design files  
→ Files: See `LICENSE.txt`

### "I want to modify and use the firmware"
→ Use: Firmware under BSD-3-Clause-Clear  
→ Requirements: Retain copyright notices  
→ Files: See `FIRMWARE-LICENSE.txt`

### "I want to use on non-STM32 hardware"
→ Problem: Azure RTOS/USBX is restricted  
→ Solution: Replace with TinyUSB (MIT) or similar  
→ Details: See `LICENSE_GUIDE.md`

### "I want to sell products"
→ Hardware: ✅ Allowed (CERN-OHL-P-2.0)  
→ Firmware: ✅ Allowed (BSD-3-Clause-Clear)  
→ Restriction: Must use STM32U5 (or replace USBX)  
→ Details: See `NOTICE.txt`

---

## Compliance Checklist

### For Hardware Distribution:
- [ ] Include `LICENSE.txt` or reference to CERN-OHL-P-2.0
- [ ] Provide attribution to Tropic Square
- [ ] Make source files (KiCad) available
- [ ] Document any modifications

### For Firmware Distribution:
- [ ] Include `FIRMWARE-LICENSE.txt`
- [ ] Retain all copyright notices
- [ ] Include `NOTICE.txt` or equivalent third-party notices
- [ ] Include STM32 HAL license
- [ ] Include Microsoft Azure RTOS license if using USBX
- [ ] Verify hardware compliance for USBX

### For Combined Products:
- [ ] All hardware requirements above
- [ ] All firmware requirements above
- [ ] Clear documentation of what license applies where

---

## Changes Made (January 2026)

### New Files Created:
1. ✅ `../LICENSE.txt` - Root hardware license (CERN-OHL-P-2.0)
2. ✅ `LICENSES/FIRMWARE-LICENSE.txt` - Firmware license (BSD-3-Clause-Clear)
3. ✅ `LICENSES/NOTICE.txt` - Complete third-party notices
4. ✅ `LICENSES/LICENSE_GUIDE.md` - Comprehensive user guide
5. ✅ `../TROPIC01_Arduino_Shield/LICENSE.txt` - Hardware license
6. ✅ `../TROPIC01_Raspberry_Pi_Shield/LICENSE.txt` - Hardware license
7. ✅ `../TROPIC01_USB_Devkit/PCB/LICENSE.txt` - Hardware license
8. ✅ `../TROPIC01_USB_Devkit/firmwares/firmware_B/NOTICE.txt` - Firmware third-party notices

### Updated Files:
1. ✅ `../README.md` - Added comprehensive license section

### Existing Files (Preserved):
- `../TROPIC01_Mini_Board/TS1701/LICENSE.txt` - Kept original Clear BSD
- `../TROPIC01_USB_Devkit/firmwares/firmware_B/LICENSE.txt` - Kept original Clear BSD
- `../TROPIC01_USB_Devkit/firmwares/firmware_B/sdk/stm32/STM32U5xx_HAL_Driver/LICENSE.md` - Third-party
- `../TROPIC01_USB_Devkit/firmwares/firmware_B/sdk/stm32/STM32_USBX_Library/LICENSE.txt` - Third-party

---

## Benefits of This Structure

### ✅ Clarity
- Clear separation between hardware and firmware licenses
- No ambiguity about what license applies where
- Easy to understand for users and contributors

### ✅ Compliance
- Proper attribution to all parties
- All third-party licenses properly documented
- Clear notice of restrictions (Microsoft USBX)

### ✅ Openness
- Hardware fully open under CERN-OHL-P-2.0
- Firmware open under permissive BSD license
- Path forward for fully open solution (replace USBX)

### ✅ Legal Safety
- Proper documentation reduces legal risk
- Clear notice of Microsoft restrictions
- Compliance requirements spelled out

---

## Next Steps (Optional)

### For Full Open Source:
1. Consider replacing Azure RTOS/USBX with TinyUSB (MIT)
2. This would remove all hardware restrictions
3. Entire stack would be truly open source

### For Source Code:
1. Add SPDX identifiers to source file headers
2. Makes automatic license scanning easier
3. Industry best practice

### For Contributors:
1. Add CONTRIBUTING.md with license assignment policy
2. Clarify how contributions will be licensed

---

## Questions or Issues?

For questions about:
- **This license structure:** Open an issue in this repository
- **Hardware licenses:** See `LICENSE.txt` and `LICENSE_GUIDE.md`
- **Firmware licenses:** See `FIRMWARE-LICENSE.txt` and `NOTICE.txt`
- **Third-party components:** See `NOTICE.txt` for contact information
- **Legal advice:** Consult with a qualified attorney

---

**Implementation Status:** ✅ COMPLETE  
**Last Updated:** January 2026  
**Maintainer:** Tropic Square

---

*This multi-license structure follows open source and open hardware best practices, ensuring proper attribution, compliance, and clarity for all users of this repository.*
