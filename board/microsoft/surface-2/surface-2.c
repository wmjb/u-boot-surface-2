// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2010-2013, NVIDIA CORPORATION.  All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <log.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gp_padctrl.h>
#include <power/regulator.h>
#include "pinmux-config-surface-2.h"
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <linux/delay.h>
#include <i2c.h>
#include <env.h>

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

int microsoft_board_init(void)
{
	/* Set up boot-on regulators */
	regulators_enable_boot_on(true);

	return 0;
}


/*
 * Routine: pin_mux_mmc
 * Description: setup the MMC muxes, power rails, etc.
 */

/*
void pin_mux_mmc(void)
{

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


}
*/
