# License Guide for TROPIC01 Development Boards Repository

## Quick Reference

This repository uses a **multi-license structure** to properly accommodate different types of content:

| What | License | Where to Find |
|:-----|:--------|:--------------|
| 🔧 **Hardware Designs** | CERN-OHL-P-2.0 | [../LICENSE.txt](../LICENSE.txt) |
| 💻 **Tropic Square Firmware** | BSD-3-Clause-Clear | [FIRMWARE-LICENSE.txt](FIRMWARE-LICENSE.txt) |
| 📦 **Third-Party Software** | Various | [NOTICE.txt](NOTICE.txt) |

---

## Detailed License Information

### Hardware Designs (CERN-OHL-P-2.0)

**Applies to:**
- All PCB layouts (`.kicad_pcb` files)
- All schematics (`.kicad_sch` files)
- Mechanical designs and enclosures
- Bill of Materials (BOM)
- Gerber files
- Hardware documentation and images

**What you can do:**
- ✅ Use the designs commercially
- ✅ Modify the designs
- ✅ Manufacture boards based on the designs
- ✅ Distribute the designs (modified or unmodified)
- ✅ Sell products based on the designs

**Requirements:**
- 📄 Include copyright notice and attribution to Tropic Square
- 📄 Make source files available if you distribute hardware
- 📄 Indicate modifications if you change the design

**License file:** [../LICENSE.txt](../LICENSE.txt)  
**Full license text:** https://ohwr.org/cernohl

---

### Firmware & Software (Tropic Square Code)

**License:** BSD-3-Clause-Clear  
**Applies to:**
- Application code in `TROPIC01_USB_Devkit/legacy_fw/firmware_B/app/`
- Custom drivers and hardware interfaces
- All code authored by Tropic Square

**What you can do:**
- ✅ Use commercially
- ✅ Modify
- ✅ Distribute (source or binary)
- ✅ Sublicense

**Requirements:**
- 📄 Retain copyright notice
- 📄 Include license text and disclaimer
- 📄 Don't use Tropic Square name for endorsement

**Notable:** Clear BSD explicitly states **no patent license is granted**.

**License file:** [FIRMWARE-LICENSE.txt](FIRMWARE-LICENSE.txt)

---

### Third-Party Software Components

The firmware includes components from other sources. Each has its own license:

#### 1. STM32 HAL Drivers (BSD-3-Clause)

**Component:** STMicroelectronics Hardware Abstraction Layer  
**Location:** `TROPIC01_USB_Devkit/legacy_fw/firmware_B/sdk/stm32/STM32U5xx_HAL_Driver/`  
**Copyright:** STMicroelectronics (2021)  
**License:** BSD-3-Clause (permissive)

**What you can do:**
- ✅ Use, modify, distribute freely
- ✅ Commercial use allowed
- ✅ Compatible with other permissive licenses

**Requirements:**
- 📄 Retain STMicroelectronics copyright notice
- 📄 Include license text

---

#### 2. Microsoft Azure RTOS / USBX (Proprietary)

**Component:** USB stack library  
**Location:** `TROPIC01_USB_Devkit/legacy_fw/firmware_B/sdk/stm32/STM32_USBX_Library/`  
**Copyright:** Microsoft Corporation  
**License:** Microsoft Software License Terms

⚠️ **IMPORTANT RESTRICTIONS:**

This component is **NOT fully open source**. Key restrictions:

1. **Hardware Limitation:**  
   - Can ONLY be used with STM32U5 and other licensed MCUs
   - See `LICENSED-HARDWARE.txt` for approved hardware list
   - Cannot port to non-licensed hardware

2. **Distribution Restrictions:**
   - Cannot redistribute as standalone component
   - Must include Microsoft license terms with any distribution
   - Cannot remove license or copyright notices

3. **Usage Restrictions:**
   - Cannot use to create competing RTOS products
   - Cannot reverse engineer

**Compliance Requirements:**
- ✅ Only use with STM32U5 hardware (or other licensed MCUs)
- ✅ Include Microsoft license file with distributions
- ✅ Review full license before commercial use

**License file:** `TROPIC01_USB_Devkit/legacy_fw/firmware_B/sdk/stm32/STM32_USBX_Library/LICENSE.txt`

**Alternative:** For fully open firmware, consider replacing with TinyUSB (MIT license).

---

## How to Comply with These Licenses

### If you're using the hardware designs:

1. ✅ Manufacture boards freely
2. ✅ Modify the designs as needed
3. ✅ If you distribute modified designs:
   - Include the CERN-OHL-P-2.0 license
   - Provide attribution to Tropic Square
   - Make source files (KiCad files) available
   - Document your modifications

### If you're using the firmware:

1. **For Tropic Square code (Clear BSD):**
   - Retain copyright notices in source files
   - Include license text in distributions
   - Include disclaimer in documentation

2. **For STM32 HAL (BSD-3-Clause):**
   - Retain STMicroelectronics copyright
   - Include license text

3. **For Azure RTOS/USBX (Microsoft):**
   - ⚠️ **Verify your hardware is licensed** (STM32U5 is approved)
   - Include Microsoft license file
   - Do not use on non-licensed hardware
   - Review full Microsoft license terms

### If you're distributing products:

**Hardware Products:**
- Include attribution to Tropic Square
- Provide link to hardware source files (or include them)
- Include CERN-OHL-P-2.0 license notice

**Firmware/Software:**
- Include all required license files and notices
- Retain all copyright notices
- Include full license texts in documentation
- Ensure Microsoft Azure RTOS compliance if using USBX

---

## Frequently Asked Questions

### Q: Can I manufacture and sell boards based on these designs?
**A:** Yes! The CERN-OHL-P-2.0 license allows commercial use. Just provide attribution and make the design files available.

### Q: Can I modify the hardware and firmware?
**A:** Yes! Both licenses are permissive. For hardware, share your modifications under CERN-OHL-P-2.0. For firmware, keep the same license.

### Q: What if I want to use the firmware on non-STM32 hardware?
**A:** The Microsoft Azure RTOS/USBX component restricts this. You have two options:
1. Keep only using STM32U5 hardware (compliant)
2. Replace Azure RTOS/USBX with an open alternative like TinyUSB (MIT license)

### Q: Do I need to pay royalties?
**A:** No. All licenses here are royalty-free. CERN-OHL-P-2.0 and BSD licenses don't require royalties.

### Q: Can I keep my modifications private?
**A:** For firmware: Yes (BSD is permissive). For hardware: CERN-OHL-P-2.0 requires you to share modifications if you distribute the hardware or products based on it.

### Q: What's the difference between Clear BSD and BSD-3-Clause?
**A:** Clear BSD explicitly states that no patent license is granted. BSD-3-Clause is silent on patents. Both are otherwise very similar and compatible.

### Q: Can I combine this with my proprietary code?
**A:** Yes, both BSD and CERN-OHL-P-2.0 are permissive and allow combination with proprietary code. However, you must:
- Keep the original license notices
- Include the required license texts
- Comply with Microsoft Azure RTOS restrictions if using USBX

---

## License Compatibility

### Compatible Combinations ✅

These licenses work well together:

| License A | License B | Compatible? | Notes |
|:----------|:----------|:------------|:------|
| CERN-OHL-P-2.0 | BSD-3-Clause-Clear | ✅ Yes | Hardware + Firmware combination |
| CERN-OHL-P-2.0 | BSD-3-Clause | ✅ Yes | Permissive licenses |
| BSD-3-Clause-Clear | BSD-3-Clause | ✅ Yes | BSD family licenses |
| Any permissive | Proprietary code | ✅ Yes | Can combine |

### Restricted Combinations ⚠️

| License A | License B | Issue | Solution |
|:----------|:----------|:------|:---------|
| Microsoft Azure RTOS | GPL licenses | ⚠️ Incompatible | Don't combine, or replace Azure RTOS |
| Microsoft Azure RTOS | Non-STM32 hardware | ⚠️ Restricted | Only use with licensed hardware |

---

## Getting Fully Open Source

If you want a **completely open-source solution** without any proprietary components:

### Replace Azure RTOS/USBX with:

1. **TinyUSB** (Recommended)
   - License: MIT (permissive)
   - Excellent STM32 support
   - Active development
   - Repository: https://github.com/hathach/tinyusb

2. **CherryUSB**
   - License: Apache 2.0
   - Modern design
   - Good performance
   - Repository: https://github.com/cherry-embedded/CherryUSB

3. **libusb**
   - License: LGPL (with static linking exception)
   - Mature and stable

This would make the entire stack (hardware + firmware) **truly open source** with no hardware restrictions.

---

## SPDX License Identifiers

For source file headers, use these SPDX identifiers:

```c
// Hardware designs
// SPDX-License-Identifier: CERN-OHL-P-2.0
// Copyright (c) 2024 Tropic Square

// Tropic Square firmware
// SPDX-License-Identifier: BSD-3-Clause-Clear
// Copyright (c) 2024 Tropic Square

// STM32 HAL (existing code)
// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2021 STMicroelectronics
```

---

## Resources

### License Texts
- CERN-OHL-P-2.0: https://ohwr.org/cernohl
- BSD-3-Clause-Clear: https://spdx.org/licenses/BSD-3-Clause-Clear.html
- BSD-3-Clause: https://opensource.org/licenses/BSD-3-Clause

### Tools
- SPDX License List: https://spdx.org/licenses/
- OSI License Info: https://opensource.org/licenses/
- CERN OHL FAQ: https://ohwr.org/project/cernohl/wikis/faq

### Getting Help
- For hardware/firmware questions: Contact Tropic Square
- For STM32 HAL questions: Refer to STMicroelectronics
- For Azure RTOS questions: Consult Microsoft documentation
- For legal advice: Consult with a qualified attorney

---

## Summary Checklist

When using this repository, ensure you:

- [ ] Understand which components you're using
- [ ] Have read the applicable license files
- [ ] Include required copyright notices
- [ ] Include required license texts in distributions
- [ ] Verify hardware compatibility (for Azure RTOS/USBX)
- [ ] Provide attribution to Tropic Square (for hardware)
- [ ] Make hardware source files available (if distributing)
- [ ] Document any modifications to the designs

---

**Last Updated:** January 2026  
**Questions?** Please open an issue or contact Tropic Square

---

**Legal Disclaimer:** This guide is for informational purposes only and does not constitute legal advice. For binding legal guidance, consult with a qualified attorney familiar with open source and open hardware licensing.
