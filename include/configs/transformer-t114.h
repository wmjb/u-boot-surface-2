/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2010-2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * Copyright (c) 2023, Svyatoslav Ryhel <clamor95@gmail.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#include "tegra114-common.h"

/* High-level configuration options */
#define CFG_TEGRA_BOARD_STRING		"ASUS Transformer"

/* Board-specific serial config */
#define CFG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#include "tegra-common-post.h"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"fastboot usb 0;" \
	"poweroff;"

#endif /* __CONFIG_H */
