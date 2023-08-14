// SPDX-License-Identifier: GPL-2.0
#ifndef KC57_LAPTOP_WMI_H
#define KC57_LAPTOP_WMI_H

/* ========================================================================== */
/* WMI methods */

/* AcpiTest_MULong */
#define KC57_WMI_WMBC_GUID       "ABBC0F6F-8EA1-11D1-00A0-C90629100000"
#define KC57_WMBC_GETSETULONG_ID 4

/* ========================================================================== */
/* WMI events */

/* AcpiTest_EventULong */
#define KC57_WMI_EVENT0_GUID     "ABBC0F72-8EA1-11D1-00A0-C90629100000"

/* AcpiTest_EventString */
#define KC57_WMI_EVENT1_GUID     "ABBC0F71-8EA1-11D1-00A0-C90629100000"

/* AcpiTest_EventPackage */
#define KC57_WMI_EVENT2_GUID     "ABBC0F70-8EA1-11D1-00A0-C90629100000"

#endif /* KC57_LAPTOP_WMI_H */
