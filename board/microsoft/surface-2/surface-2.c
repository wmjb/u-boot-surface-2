// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2010-2013, NVIDIA CORPORATION.  All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <log.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gp_padctrl.h>
#include "pinmux-config-surface-2.h"
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <linux/delay.h>
#include <i2c.h>

#define BAT_I2C_ADDRESS		0x48	/* TPS65090 charger */
#define PMU_I2C_ADDRESS		0x58	/* TPS65913 PMU */

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	pinmux_config_pingrp_table(tegra114_pinmux_set_nontristate,
		ARRAY_SIZE(tegra114_pinmux_set_nontristate));

	pinmux_config_pingrp_table(tegra114_pinmux_common,
		ARRAY_SIZE(tegra114_pinmux_common));

	pinmux_config_pingrp_table(unused_pins_lowpower,
		ARRAY_SIZE(unused_pins_lowpower));

	/* Initialize any non-default pad configs (APB_MISC_GP regs) */
	pinmux_config_drvgrp_table(surface_2_padctrl,
		ARRAY_SIZE(surface_2_padctrl));
}

#if defined(CONFIG_MMC_SDHCI_TEGRA)
/*
 * This recreates UEFI set up.
 *
 */
void init_tps65090() {
	struct udevice *dev;
	uchar reg, data_buffer[1];
	int ret;

	ret = i2c_get_chip_for_busnum(4, BAT_I2C_ADDRESS, 1, &dev);
	if (ret) {
		debug("%s: Cannot find charger I2C chip\n", __func__);
		return;
	}

//////// CHARGER ///////////////////////////////////////////////////////////////
	// TBD

//////// FET ///////////////////////////////////////////////////////////////////




	// TPS65090: FET1_CTRL = enable output auto discharge, enable FET6
	// Needed by panel backlight
	data_buffer[0] = 0x0f;
	ret = dm_i2c_write(dev, 0x0f, data_buffer, 1);
	if (ret)
		printf("%s: BAT i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65090: FET2_CTRL = enable output auto discharge, enable FET6
	data_buffer[0] = 0x0f;
	ret = dm_i2c_write(dev, 0x10, data_buffer, 1);
	if (ret)
		printf("%s: BAT i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65090: FET4_CTRL = enable output auto discharge, enable FET4
	// Needed by panel backlight
	data_buffer[0] = 0x03;
	ret = dm_i2c_write(dev, 0x12, data_buffer, 1);
	if (ret)
		printf("%s: BAT i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65090: FET5_CTRL = enable output auto discharge, enable FET6
	data_buffer[0] = 0x03;
	ret = dm_i2c_write(dev, 0x13, data_buffer, 1);
	if (ret)
		printf("%s: BAT i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);





        // TPS65090: FET6_CTRL = enable output auto discharge, enable FET6
        data_buffer[0] = 0x3;
        ret = dm_i2c_write(dev, 0x14, data_buffer, 1);
        if (ret)
                printf("%s: BAT i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


}

/*
 * Do I2C/PMU writes to bring up SD card bus power
 *
 */
void board_sdmmc_voltage_init(void)
{
	struct udevice *dev;
	uchar reg, data_buffer[1];
	int ret;

	

	ret = i2c_get_chip_for_busnum(4, PMU_I2C_ADDRESS, 1, &dev);
	if (ret) {
		debug("%s: Cannot find PMIC I2C chip\n", __func__);
		return;
	}

///////// SMPS's ////////////////////////////////////////////////////////////////
	// TPS65913: SMPS12_TSTEP

	data_buffer[0] = 0x03;
	ret = dm_i2c_write(dev, 0x21, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: SMPS12_CTRL
	data_buffer[0] = 0xD1;
	ret = dm_i2c_write(dev, 0x20, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: SMPS6_CTRL
	data_buffer[0] = 0xC0;
	ret = dm_i2c_write(dev, 0x2C, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


///////// LDO's ////////////////////////////////////////////////////////////////

	// TPS65913: LDO1_VOLTAGE = 3.3V
	data_buffer[0] = 0x07;
	reg = 0x51;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO1_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x50;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


	// TPS65913: LDO2_VOLTAGE = 3.3V
	data_buffer[0] = 0x27;
	reg = 0x53;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO2_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x52;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


	// TPS65913: LDO3_VOLTAGE = 3.3V
	data_buffer[0] = 0x07;
	reg = 0x55;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO3_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x54;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


	// TPS65913: LDO5_VOLTAGE = 3.3V
	data_buffer[0] = 0x13;
	reg = 0x59;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO5_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x58;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


	// TPS65913: LDO7_VOLTAGE = 3.3V
	data_buffer[0] = 0x13;
	reg = 0x5D;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO7_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x5C;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


	// TPS65913: LDO8_VOLTAGE = 3.3V
	data_buffer[0] = 0x07;
	reg = 0x5F;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO8_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x5E;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

////////////////////
	// TPS65913: LDO9_VOLTAGE = 3.3V
	data_buffer[0] = 0x31;
	reg = 0x61;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDO9_CTRL = Active
	data_buffer[0] = 0x01;
	reg = 0x60;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);
//////////////////


	// TPS65913: LDOLN_VOLTAGE = 3.3V
	data_buffer[0] = 0x13;
	reg = 0x63;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);

	// TPS65913: LDOLN_CTRL = Active
	data_buffer[0] = 0x11;
	reg = 0x62;

	ret = dm_i2c_write(dev, reg, data_buffer, 1);
	if (ret)
		printf("%s: PMU i2c_write %02X<-%02X returned %d\n", __func__, reg, data_buffer[0], ret);


//	init_tps65090();

}

/*
 * Routine: pin_mux_mmc
 * Description: setup the MMC muxes, power rails, etc.
 */
void pin_mux_mmc(void)
{
	/*
	 * NOTE: We don't do mmc-specific pin muxes here.
	 * They were done globally in pinmux_init().
	 */

	/* Bring up the SDIO3 power rail */

        init_tps65090();

	board_sdmmc_voltage_init();


	gpio_request(TEGRA_GPIO(K, 1), "Enable USB K1");
	gpio_direction_output(TEGRA_GPIO(K, 1), 0);
	udelay(5);
	gpio_set_value(TEGRA_GPIO(K, 1), 1);
	printf("Enabled GPIO K1\n");

        gpio_request(TEGRA_GPIO(X, 5), "Enable USB X5");
        gpio_direction_output(TEGRA_GPIO(X, 5), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(X, 5), 1);
        printf("Enabled GPIO X5\n");

        gpio_request(TEGRA_GPIO(X, 6), "Enable USB X6");
        gpio_direction_output(TEGRA_GPIO(X, 6), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(X, 6), 1);
        printf("Enabled GPIO X6\n");

        gpio_request(TEGRA_GPIO(N, 4), "Enable USB N4");
        gpio_direction_output(TEGRA_GPIO(N, 4), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(N, 4), 1);
        printf("Enabled GPIO N4\n");

/*

        gpio_request(TEGRA_GPIO(R, 3), "Enable USB R3");
        gpio_direction_output(TEGRA_GPIO(R,3), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(R, 3), 1);
        printf("Enabled GPIO R3\n");

        gpio_request(TEGRA_GPIO(O, 3), "Enable USB O3");
        gpio_direction_output(TEGRA_GPIO(O, 3), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(O, 3), 1);
        printf("Enabled GPIO O3\n");

        gpio_request(TEGRA_GPIO(R, 7), "Enable USB R7");
        gpio_direction_output(TEGRA_GPIO(R, 7), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(R, 7), 1);
        printf("Enabled GPIO R7\n");



        gpio_request(TEGRA_GPIO(BB, 3), "Enable USB BB3"); //front cam on
        gpio_direction_output(TEGRA_GPIO(BB, 3), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(BB, 3), 1);
        printf("Enabled GPIO BB3\n");

        gpio_request(TEGRA_GPIO(BB, 5), "Enable USB BB5"); // on back cam
        gpio_direction_output(TEGRA_GPIO(BB, 5), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(BB, 5), 1);
        printf("Enabled GPIO BB5\n");


        gpio_request(TEGRA_GPIO(BB, 7), "Enable USB BB7"); //unknown
        gpio_direction_output(TEGRA_GPIO(BB, 7), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(BB, 7), 1);
        printf("Enabled GPIO BB7\n");



        gpio_request(TEGRA_GPIO(S, 1), "Enable USB S1"); //unknown
        gpio_direction_output(TEGRA_GPIO(S, 1), 0);
        udelay(5);
        gpio_set_value(TEGRA_GPIO(S, 1), 1);
        printf("Enabled GPIO S1\n");
*/
//        init_tps65090();

}
#endif /* MMC */
