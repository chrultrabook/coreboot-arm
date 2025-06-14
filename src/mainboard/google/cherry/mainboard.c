/* SPDX-License-Identifier: GPL-2.0-only */

#include <boardid.h>
#include <bootmode.h>
#include <console/console.h>
#include <device/device.h>
#include <device/mmio.h>
#include <ec/google/chromeec/ec.h>
#include <gpio.h>
#include <soc/bl31.h>
#include <soc/display.h>
#include <soc/dpm_v1.h>
#include <soc/i2c.h>
#include <soc/msdc.h>
#include <soc/mtcmos.h>
#include <soc/pcie.h>
#include <soc/spm.h>
#include <soc/usb.h>
#include <types.h>
#include <symbols.h>

#include "gpio.h"

/* GPIO to schematics names */
#define GPIO_AP_EDP_BKLTEN GPIO(DGI_D5)
#define GPIO_BL_PWM_1V8 GPIO(DISP_PWM0)
#define GPIO_EDP_HPD_1V8 GPIO(GPIO_07)
#define GPIO_EN_PP3300_DISP_X GPIO(I2SO1_D2)

bool mainboard_needs_pcie_init(void)
{
	uint32_t sku = sku_id();

	if (sku == CROS_SKU_UNKNOWN) {
		printk(BIOS_WARNING, "Unknown SKU (%#x); assuming PCIe", sku);
		return true;
	} else if (sku == CROS_SKU_UNPROVISIONED) {
		printk(BIOS_WARNING, "Unprovisioned SKU (%#x); assuming PCIe", sku);
		return true;
	}

	/*
	 * All cherry boards share the same SKU encoding. Therefore there is no need to check
	 * the board here.
	 * - BIT(1): NVMe (PCIe)
	 * - BIT(3): UFS (which takes precedence over BIT(1))
	 */
	if (sku & BIT(3))
		return false;
	if (sku & BIT(1))
		return true;

	/* Otherwise, eMMC */
	return false;
}

static void configure_backlight(void)
{
	/* Enable backlight before payload handoff if we're not building for CrOS */
	if (!CONFIG(CHROMEOS)) {
		gpio_output(GPIO_AP_EDP_BKLTEN, 1);
		gpio_output(GPIO_BL_PWM_1V8, 1);
	}
	/* Leave it disabled for depthcharge */
	else {
		gpio_output(GPIO_AP_EDP_BKLTEN, 0);
		gpio_output(GPIO_BL_PWM_1V8, 0);
	}
}

static void power_on_panel(void)
{
	/* Default power sequence for most panels. */
	gpio_set_pull(GPIO_EDP_HPD_1V8, GPIO_PULL_ENABLE, GPIO_PULL_UP);
	gpio_set_mode(GPIO_EDP_HPD_1V8, 2);
	gpio_output(GPIO_EN_PP3300_DISP_X, 1);
}

static struct panel_description panel = {
	.configure_backlight = configure_backlight,
	.power_on = power_on_panel,
	.disp_path = DISP_PATH_EDP,
	.orientation = LB_FB_ORIENTATION_NORMAL,
};

struct panel_description *get_active_panel(void)
{
	return &panel;
}

static void configure_i2s(void)
{
	/* Audio PWR */
	mtcmos_audio_power_on();
	mtcmos_protect_audio_bus();

	/* SoC I2S */
	gpio_set_mode(GPIO(GPIO_02), PAD_GPIO_02_FUNC_TDMIN_LRCK);
	gpio_set_mode(GPIO(GPIO_03), PAD_GPIO_03_FUNC_TDMIN_BCK);
	gpio_set_mode(GPIO(I2SO2_D0), PAD_I2SO2_D0_FUNC_I2SO2_D0);
}

static void configure_audio(void)
{
	if (CONFIG(CHERRY_USE_RT1011) || CONFIG(CHERRY_USE_MAX98390))
		mtk_i2c_bus_init(I2C2, I2C_SPEED_FAST);

	if (CONFIG(CHERRY_USE_MAX98390))
		configure_i2s();
}

static void mainboard_init(struct device *dev)
{
	if (display_init_required()) {
		memset(_framebuffer, 0x00, REGION_SIZE(framebuffer));
		if (mtk_display_init((uintptr_t)_framebuffer, REGION_SIZE(framebuffer)) < 0)
			printk(BIOS_ERR, "%s: Failed to init display\n", __func__);
	}
	else
		printk(BIOS_INFO, "%s: Skipped display initialization\n", __func__);

	mtk_msdc_configure_emmc(true);
	mtk_msdc_configure_sdcard();
	setup_usb_host();

	configure_audio();

	if (dpm_init())
		printk(BIOS_ERR, "dpm init failed, DVFS may not work\n");

	if (spm_init())
		printk(BIOS_ERR, "spm init failed, system suspend may not work\n");

	if (CONFIG(ARM64_USE_ARM_TRUSTED_FIRMWARE))
		register_reset_to_bl31(GPIO_RESET.id, true);
}

static void mainboard_enable(struct device *dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
