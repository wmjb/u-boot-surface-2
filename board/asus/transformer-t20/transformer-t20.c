// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  (C) Copyright 2010,2011
 *  NVIDIA Corporation <www.nvidia.com>
 *
 *  (C) Copyright 2021
 *  Svyatoslav Ryhel <clamor95@gmail.com>
 */

/* T20 Transformers derive from Ventana board */

#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <log.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/tegra.h>
#include <asm/arch-tegra/board.h>
#include <asm/arch/clock.h>
#include <asm/arch/funcmux.h>
#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>
#include <asm/gpio.h>
#include <linux/delay.h>

#define PMU_I2C_ADDRESS		0x34

#define TPS6586X_SUPPLYENE	0x14
#define EXITSLREQ_BIT		BIT(1)
#define SLEEP_MODE_BIT		BIT(3)

#ifdef CONFIG_CMD_POWEROFF
int do_poweroff(struct cmd_tbl *cmdtp,
		       int flag, int argc, char *const argv[])
{
	struct udevice *dev;
	uchar data_buffer[1];
	int ret;

	ret = i2c_get_chip_for_busnum(0, PMU_I2C_ADDRESS, 1, &dev);
	if (ret) {
		debug("%s: Cannot find PMIC I2C chip\n", __func__);
		return 0;
	}

	ret = dm_i2c_read(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	data_buffer[0] &= ~EXITSLREQ_BIT;

	ret = dm_i2c_write(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	data_buffer[0] |= SLEEP_MODE_BIT;

	ret = dm_i2c_write(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	// wait some time and then print error
	mdelay(5000);
	printf("Failed to power off!!!\n");
	return 1;
}
#endif

#ifdef CONFIG_MMC_SDHCI_TEGRA
/*
 * Routine: pin_mux_mmc
 * Description: setup the pin muxes/tristate values for the SDMMC(s)
 */
void pin_mux_mmc(void)
{
	funcmux_select(PERIPH_ID_SDMMC4, FUNCMUX_SDMMC4_ATB_GMA_GME_8_BIT);
	funcmux_select(PERIPH_ID_SDMMC3, FUNCMUX_SDMMC3_SDB_4BIT);

	/* For power GPIO PI6 */
	pinmux_tristate_disable(PMUX_PINGRP_ATA);
	/* For CD GPIO PI5 */
	pinmux_tristate_disable(PMUX_PINGRP_ATC);
}
#endif

void pin_mux_usb(void)
{
	/* For USB0's GPIO PD0. For now, since we have no pinmux in fdt */
	pinmux_tristate_disable(PMUX_PINGRP_SLXK);
	/* For USB1's ULPI signals */
	funcmux_select(PERIPH_ID_USB2, FUNCMUX_USB2_ULPI);
	pinmux_set_func(PMUX_PINGRP_CDEV2, PMUX_FUNC_PLLP_OUT4);
	pinmux_tristate_disable(PMUX_PINGRP_CDEV2);
	/* USB1 PHY reset GPIO */
	pinmux_tristate_disable(PMUX_PINGRP_UAC);
}
