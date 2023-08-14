// SPDX-License-Identifier: GPL-2.0
#ifndef KC57_BATTERY_H
#define KC57_BATTERY_H

#if IS_ENABLED(CONFIG_ACPI_BATTERY)

#include <linux/init.h>

int  __init kc57_battery_setup(void);
void        kc57_battery_cleanup(void);

#else

static inline int kc57_battery_setup(void)
{
	return 0;
}

static inline void kc57_battery_cleanup(void)
{

}

#endif

#endif /* KC57_BATTERY_H */
