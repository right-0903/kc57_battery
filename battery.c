// SPDX-License-Identifier: GPL-2.0
/*
 * kc57_battery.c - A charging limit driver for KC57 laptop
 *
 * based on qc71_laptop: https://github.com/pobrn/qc71_laptop
 *
 * Copyright (C) 2020-2023 Barnabás Pőcze <pobrn@protonmail.com>
 * Copyright (C) 2023-2024 nuvole <mitltlatltl@gmail.com>
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/power_supply.h>
#include <acpi/battery.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/version.h>

#include "kc57_battery.h"

/* ========================================================================== */

#if IS_ENABLED(CONFIG_ACPI_BATTERY)

static bool battery_hook_registered;

static bool nobattery;
module_param(nobattery, bool, 0444);
MODULE_PARM_DESC(nobattery, "do not expose battery related controls (default=false)");

/* ========================================================================== */

static ssize_t charge_control_end_threshold_show(struct device *dev,
						 struct device_attribute *attr, char *buf)
{
	int status, r_status;
	r_status = uniwill_read_ec_ram(BATT_CHARGE_CTRL_ADDR, (u8 *)&status);

	if (r_status < 0)
		return r_status;

	status &= BATT_CHARGE_CTRL_VALUE_MASK;

	if (status == 0)
		status = 100;

	return sprintf(buf, "%d\n", status);
}

static ssize_t charge_control_end_threshold_store(struct device *dev, struct device_attribute *attr,
						  const char *buf, size_t count)
{
	int status, value, r_status;

	if (kstrtoint(buf, 10, &value) || !(1 <= value && value <= 100))
		return -EINVAL;

	r_status = uniwill_read_ec_ram(BATT_CHARGE_CTRL_ADDR, (u8 *)&status);

	if (r_status < 0)
		return r_status;

	if (value == 100)
		value = 0;

	status = (status & ~BATT_CHARGE_CTRL_VALUE_MASK) | value;

	status = uniwill_write_ec_ram(BATT_CHARGE_CTRL_ADDR, status & 0xFF);

	if (status < 0)
		return status;

	return count;
}

static DEVICE_ATTR_RW(charge_control_end_threshold);
static struct attribute *kc57_laptop_batt_attrs[] = {
	&dev_attr_charge_control_end_threshold.attr,
	NULL
};
ATTRIBUTE_GROUPS(kc57_laptop_batt);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
static int kc57_laptop_batt_add(struct power_supply *battery, struct acpi_battery_hook *hook)
#else
static int kc57_laptop_batt_add(struct power_supply *battery)
#endif
{
	if (strcmp(battery->desc->name, "BAT0") != 0)
		return 0;

	return device_add_groups(&battery->dev, kc57_laptop_batt_groups);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
static int kc57_laptop_batt_remove(struct power_supply *battery, struct acpi_battery_hook *hook)
#else
static int kc57_laptop_batt_remove(struct power_supply *battery)
#endif
{
	if (strcmp(battery->desc->name, "BAT0") != 0)
		return 0;

	device_remove_groups(&battery->dev, kc57_laptop_batt_groups);
	return 0;
}

static struct acpi_battery_hook kc57_laptop_batt_hook = {
	.add_battery    = kc57_laptop_batt_add,
	.remove_battery = kc57_laptop_batt_remove,
	.name           = "KC57 laptop battery extension",
};

static int __init kc57_battery_setup(void)
{
	if (nobattery)
		return -ENODEV;

	battery_hook_register(&kc57_laptop_batt_hook);
	battery_hook_registered = true;

	return 0;
}

static void kc57_battery_cleanup(void)
{
	if (battery_hook_registered)
		battery_hook_unregister(&kc57_laptop_batt_hook);
}

#endif /* IS_ENABLED(CONFIG_ACPI_BATTERY) */


module_init(kc57_battery_setup);
module_exit(kc57_battery_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nuvole <mitltlatltl@gmail.com>");
MODULE_DESCRIPTION("Charging limit driver for KC57 laptop");
