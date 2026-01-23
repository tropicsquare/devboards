#ifndef GPREG_H
#define	GPREG_H

// RAM-retention registers used for reboot infos

// STM32F1 does have only 16bit registers
#define GPREG_BOOT_POR           0x00000000UL // default
#define	GPREG_BOOT_REBOOT        0x00000100UL // 
#define GPREG_BOOT_STAY_IN_BOOT  0x0000AABBUL //

#define GPREG_WDID_REBOOT_RQ     0x0000b098UL

#define GPREG_WR_ENABLE {RCC->APB1ENR |= RCC_APB1ENR_BKPEN;}

#define GPREG_BOOT  BKP->DR1 // information for bootloader (if present)
#define GPREG_WDID  BKP->DR2 // watchdog-reset reason

#define	GPREG_WRITE(reg, value) {GPREG_WR_ENABLE; reg=value;}

#endif // ! GPREG_H

