// SPDX-License-Identifier: GPL-2.0
#ifndef KC57_BATTERY_H
#define KC57_BATTERY_H

#if IS_ENABLED(CONFIG_ACPI_BATTERY)

#include <linux/types.h>

/* tuxedo define them, use them instead of ec I/O methods provided by qc71_laptop to avoid race conditions */
int uniwill_read_ec_ram(u16 address, u8 *data);
int uniwill_read_ec_ram_with_retry(u16 address, u8 *data, int retries);
int uniwill_write_ec_ram(u16 address, u8 data);
int uniwill_write_ec_ram_with_retry(u16 address, u8 data, int retries);


/* ========================================================================== */
/*
 * EC register addresses and bitmasks,
 * some of them are not used,
 * only for documentation
 */

#define ADDR(page, offset)       (((uint16_t)(page) << 8) | ((uint16_t)(offset)))

/* battery charger control register */
#define BATT_CHARGE_CTRL_ADDR ADDR(0x07, 0xB9)
#define BATT_CHARGE_CTRL_VALUE_MASK GENMASK(6, 0)
#define BATT_CHARGE_CTRL_REACHED BIT(7)

#define BATT_STATUS_ADDR        ADDR(0x04, 0x32)
#define BATT_STATUS_DISCHARGING BIT(0)

/* possibly temp (in C) = value / 10 + X */
#define BATT_TEMP_ADDR ADDR(0x04, 0xA2)

#define BATT_ALERT_ADDR ADDR(0x04, 0x94)

#endif /* IS_ENABLED(CONFIG_ACPI_BATTERY) */

#endif /* KC57_BATTERY_H */
