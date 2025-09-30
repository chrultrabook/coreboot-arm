/* SPDX-License-Identifier: GPL-2.0-only */

#include <boot/coreboot_tables.h>
#include <bootmode.h>
#include <console/console.h>
#include <device/device.h>
#include <ec/google/chromeec/ec.h>
#include <gpio.h>
#include <soc/pcie.h>
#include <soc/qupv3_config_common.h>
#include <soc/qup_se_handlers_common.h>
#include "board.h"
#include <soc/usb/usb.h>

static void setup_usb(void)
{
	setup_usb_host0();
}

void lb_add_boot_mode(struct lb_header *header)
{
	if (!CONFIG(EC_GOOGLE_CHROMEEC))
		return;

	struct lb_boot_mode *mode = (struct lb_boot_mode *)lb_new_record(header);
	memset(mode, 0, sizeof(*mode));

	mode->tag = LB_TAG_BOOT_MODE;
	mode->size = sizeof(*mode);
	mode->boot_mode = LB_BOOT_MODE_NORMAL;

	if (google_chromeec_is_below_critical_threshold())
		mode->boot_mode = LB_BOOT_MODE_LOW_BATTERY;

	/* Booting into normal mode hence disable charging */
	if (mode->boot_mode == LB_BOOT_MODE_NORMAL)
		disable_slow_battery_charging();
}

bool mainboard_needs_pcie_init(void)
{
	return true;
}

static void display_startup(void)
{
	if (!display_init_required()) {
		printk(BIOS_INFO, "Skipping display init.\n");
		return;
	}

	/* TODO: add logic for display init */
}

static void mainboard_init(struct device *dev)
{
	gpi_firmware_load(QUP_0_GSI_BASE);
	gpi_firmware_load(QUP_1_GSI_BASE);
	gpi_firmware_load(QUP_2_GSI_BASE);

	/*
	 * Load console UART QUP firmware.
	 * This is required even if coreboot's serial output is disabled.
	 */
	if (!CONFIG(CONSOLE_SERIAL))
		qupv3_se_fw_load_and_init(QUPV3_2_SE5, SE_PROTOCOL_UART, FIFO);

	qupv3_se_fw_load_and_init(QUPV3_1_SE0, SE_PROTOCOL_I2C, MIXED); /* Touch I2C */
	qupv3_se_fw_load_and_init(QUPV3_1_SE6, SE_PROTOCOL_UART, FIFO); /* BT UART */
	qupv3_se_fw_load_and_init(QUPV3_0_SE0, SE_PROTOCOL_I2C, MIXED); /* Trackpad I2C */
	qupv3_se_fw_load_and_init(QUPV3_0_SE1, SE_PROTOCOL_I2C, MIXED); /* USB-A retimer */
	if (CONFIG(MAINBOARD_HAS_FINGERPRINT_VIA_SPI))
		qupv3_se_fw_load_and_init(QUPV3_2_SE2, SE_PROTOCOL_SPI, MIXED); /* Fingerprint SPI */

	/* Enable touchpad power */
	if (CONFIG_MAINBOARD_GPIO_PIN_FOR_TOUCHPAD_POWER)
		gpio_output(GPIO_TP_POWER_EN, 1);

	/*
	 * Deassert FPMCU reset. Power applied in romstage
	 * has now stabilized.
	 */
	if (CONFIG(MAINBOARD_HAS_FINGERPRINT))
		gpio_output(GPIO_FP_RST_L, 1);

	/* Setup USB related initial config */
	setup_usb();

	display_startup();
}

static void mainboard_enable(struct device *dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
