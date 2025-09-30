/* SPDX-License-Identifier: GPL-2.0-only */

#include <baseboard/variants.h>
#include <console/console.h>
#include <fw_config.h>
#include <gpio.h>
#include <inttypes.h>

/* t: base table; o: override table */
#define GPIO_PADBASED_OVERRIDE(t, o) gpio_padbased_override(t, o, ARRAY_SIZE(o))
/* t: table */
#define GPIO_CONFIGURE_PADS(t) gpio_configure_pads(t, ARRAY_SIZE(t))

static const struct pad_config i2s_enable_pads[] = {
	/* I2S_MCLK1_OUT */
	PAD_CFG_NF(GPP_D09, NONE, DEEP, NF2),
	/* I2S0_SCLK_HDR */
	PAD_CFG_NF(GPP_D10, NONE, DEEP, NF1),
	/* I2S0_SFRM_HDR */
	PAD_CFG_NF(GPP_D11, NONE, DEEP, NF1),
	/* I2S0_TXD_HDR */
	PAD_CFG_NF(GPP_D12, NONE, DEEP, NF1),
	/* I2S0_RXD_HDR */
	PAD_CFG_NF(GPP_D13, NONE, DEEP, NF1),
	/* I2S1_TXD_HDR */
	PAD_CFG_NF(GPP_S00, NONE, DEEP, NF1),
	/* I2S1_RXD_HDR */
	PAD_CFG_NF(GPP_S01, NONE, DEEP, NF1),
	/* I2S1_SCLK_HDR */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF1),
	/* I2S1_SFRM_HDR */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF1),
	/* I2S2_SCLK_HDR */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF1),
	/* I2S2_SFRM_HDR */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF1),
	/* I2S2_TXD_HDR */
	PAD_NC(GPP_S06, NONE),
	/* I2S2_RXD_HDR */
	PAD_NC(GPP_S07, NONE),

	/* DMIC_CLK */
	PAD_NC(GPP_D16, NONE),
	/* DMIC_DATA */
	PAD_NC(GPP_D17, NONE),
};

static const struct pad_config hda_enable_pads[] = {
	/* GPP_D10:     HDA_BCLK (HDR) */
	PAD_CFG_NF(GPP_D10, NONE, DEEP, NF1),
	/* GPP_D11:     HDA_SYNC (HDR) */
	PAD_CFG_NF(GPP_D11, NONE, DEEP, NF1),
	/* GPP_D12:     HDA_SDO (HDR) */
	PAD_CFG_NF(GPP_D12, NONE, DEEP, NF1),
	/* GPP_D13:     HDA_SDI0 (HDR) */
	PAD_CFG_NF(GPP_D13, NONE, DEEP, NF1),
	/* GPP_D16:     HDA_RST_N (HDR) */
	PAD_CFG_NF(GPP_D16, NONE, DEEP, NF1),
	/* GPP_D17:     HDA_SDI1 (HDR) */
	PAD_CFG_NF(GPP_D17, NONE, DEEP, NF1),
	/* GPP_S04:     DMIC0_CLK (HDR) */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF5),
	/* GPP_S05:     DMIC0_DATA (HDR) */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF5),
	/* GPP_S06:     DMIC1_CLK (HDR) */
	PAD_CFG_NF(GPP_S06, NONE, DEEP, NF5),
	/* GPP_S07:     DMIC1_DATA (HDR) */
	PAD_CFG_NF(GPP_S07, NONE, DEEP, NF5),
};

static const struct pad_config sndw_alc721_enable_pads[] = {
	/* Soundwire GPIO Config */
	/* GPP_S00:     SNDW3_CLK_CODEC (HDR) */
	PAD_CFG_NF(GPP_S00, NONE, DEEP, NF1),
	/* GPP_S01:     SNDW3_DATA0_CODEC (HDR) */
	PAD_CFG_NF(GPP_S01, NONE, DEEP, NF1),
	/* GPP_S02:     SNDW3_DATA1_CODEC (HDR) */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF1),
	/* GPP_S03:     SNDW3_DATA2_CODEC (HDR) */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF1),
	/* GPP_S04:     DMIC0_CLK (HDR) */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF5),
	/* GPP_S05:     DMIC0_DATA (HDR) */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF5),
	/* GPP_S06:     DMIC1_CLK (HDR) */
	PAD_CFG_NF(GPP_S06, NONE, DEEP, NF5),
	/* GPP_S07:     DMIC1_DATA (HDR) */
	PAD_CFG_NF(GPP_S07, NONE, DEEP, NF5),
};

static const struct pad_config audio_disable_pads[] = {
	/* GPP_D10:     HDA_BCLK (HDR) */
	PAD_NC(GPP_D10, NONE),
	/* GPP_D11:     HDA_SYNC (HDR) */
	PAD_NC(GPP_D11, NONE),
	/* GPP_D12:     HDA_SDO (HDR) */
	PAD_NC(GPP_D12, NONE),
	/* GPP_D13:     HDA_SDI0 (HDR) */
	PAD_NC(GPP_D13, NONE),
	/* GPP_D16:     HDA_RST_N (HDR) */
	PAD_NC(GPP_D16, NONE),
	/* GPP_D17:     HDA_SDI1 (HDR) */
	PAD_NC(GPP_D17, NONE),
	/* GPP_S00:     SNDW3_CLK_CODEC (HDR) */
	PAD_NC(GPP_S00, NONE),
	/* GPP_S01:     SNDW3_DATA0_CODEC (HDR) */
	PAD_NC(GPP_S01, NONE),
	/* GPP_S02:     SNDW3_DATA1_CODEC (HDR) */
	PAD_NC(GPP_S02, NONE),
	/* GPP_S03:     SNDW3_DATA2_CODEC (HDR) */
	PAD_NC(GPP_S03, NONE),
	/* GPP_S04:     DMIC0_CLK (HDR) */
	PAD_NC(GPP_S04, NONE),
	/* GPP_S05:     DMIC0_DATA (HDR) */
	PAD_NC(GPP_S05, NONE),
	/* GPP_S06:     DMIC1_CLK (HDR) */
	PAD_NC(GPP_S06, NONE),
	/* GPP_S07:     DMIC1_DATA (HDR) */
	PAD_NC(GPP_S07, NONE),
};

static const struct pad_config x4slot_pads[] = {
	/* GPP_F10:     X4_PCIE_SLOT1_PWR_EN */
	PAD_CFG_GPO(GPP_F10, 1, PLTRST),
	/* GPP_E03:     X4_DT_PCIE_RST_N */
	PAD_CFG_GPO(GPP_E03, 1, PLTRST),
	/* GPP_D03:     X4_SLOT_WAKE_N */
	PAD_CFG_GPI_SCI_LOW(GPP_D03, NONE, DEEP, LEVEL),
};

static const struct pad_config x4slot_disable_pads[] = {
	/* GPP_F10:     X4_PCIE_SLOT1_PWR_EN */
	PAD_CFG_GPO(GPP_F10, 0, PLTRST),
	/* GPP_E03:     X4_DT_PCIE_RST_N */
	PAD_NC(GPP_E03, NONE),
	/* GPP_D03:     X4_SLOT_WAKE_N */
	PAD_NC(GPP_D03, NONE),
};

/*
 * WWAN: power sequence requires three stages:
 * step 1: 3.3V power, FCP# (Full Card Power), RST#, and PERST# off
 * step 2: deassert FCP#
 * step 3: deassert RST# first, and then PERST#.
 * NOTE: Since PERST# is gated by platform reset, PERST# deassertion will happen
 * at much later time and time between RST# and PERSET# is guaranteed.
 */
static const struct pad_config pre_mem_wwan_pwr_seq1_pads[] = {
	/* GPP_E01:     EN_WWAN_PWR */
	PAD_CFG_GPO(GPP_E01, 1, PLTRST),
	/* GPP_A09:  M.2_WWAN_FCP_OFF_N */
	PAD_CFG_GPO(GPP_A09, 0, PLTRST),
	/* GPP_B20:     M.2_WWAN_RST_N */
	PAD_CFG_GPO(GPP_B20, 0, PLTRST),
	/* GPP_D03:     M.2_WWAN_PERST_GPIO_N */
	PAD_CFG_GPO(GPP_D03, 0, PLTRST),
};

static const struct pad_config pre_mem_wwan_pwr_seq2_pads[] = {
	/* GPP_A09:     M.2_WWAN_FCP_OFF_N */
	PAD_CFG_GPO(GPP_A09, 1, PLTRST),
};

static const struct pad_config wwan_pwr_seq3_pads[] = {
	/* GPP_D03:     M.2_WWAN_PERST_GPIO_N */
	PAD_CFG_GPO(GPP_D03, 1, PLTRST),
	/* GPP_B20:     M.2_WWAN_RST_N */
	PAD_CFG_GPO(GPP_B20, 1, PLTRST),
	/* GPP_E02:     WWAN_WAKE_GPIO_N */
	PAD_CFG_GPI_SCI_LOW(GPP_E02, NONE, DEEP, LEVEL),
};

static const struct pad_config wwan_disable_pads[] = {
	/* GPP_A09:     M.2_WWAN_FCP_OFF_N */
	PAD_NC(GPP_A09, NONE),
	/* GPP_D03:     M.2_WWAN_PERST_GPIO_N */
	PAD_NC(GPP_D03, NONE),
	/* GPP_B20:     M.2_WWAN_RST_N */
	PAD_NC(GPP_B20, NONE),
	/* GPP_E01:     EN_WWAN_PWR */
	PAD_NC(GPP_E01, NONE),
	/* GPP_A10:     M.2_WWAN_DISABLE_N */
	PAD_NC(GPP_A10, NONE),
	/* GPP_E02:     WWAN_WAKE_GPIO_N */
	PAD_NC(GPP_E02, NONE),
};

/* Gen4 NVME: at the top M.2 slot */
static const struct pad_config pre_mem_gen4_ssd_pwr_pads[] = {
	/* GPP_H18:     GEN4_SSD_PWREN */
	PAD_CFG_GPO(GPP_H18, 0, PLTRST),
};

static const struct pad_config gen4_ssd_pads[] = {
	/* GPP_H18:     GEN4_SSD_PWREN */
	PAD_CFG_GPO(GPP_H18, 1, PLTRST),
	/* GPP_A08:     M2_GEN4_SSD_RESET_N */
	PAD_CFG_GPO(GPP_A08, 1, PLTRST),
};

static const struct pad_config ufs_enable_pads[] = {
	/* GPP_D21:     GPP_D21_UFS_REFCLK */
	PAD_CFG_NF(GPP_D21, NONE, DEEP, NF1),
};

static const struct pad_config pcie_wlan_enable_pads[] = {
	/* GPP_A11:     WLAN_RST_N */
	PAD_CFG_GPO(GPP_A11, 1, PLTRST),
	/* GPP_A12:     WIFI_WAKE_N */
	PAD_CFG_GPI_SCI_LOW(GPP_A12, NONE, DEEP, LEVEL),
};

static const struct pad_config pcie_wlan_disable_pads[] = {
	/* GPP_A11:     WLAN_RST_N */
	PAD_NC(GPP_A11, NONE),
	/* GPP_A12:     WIFI_WAKE_N */
	PAD_NC(GPP_A12, NONE),
};

static const struct pad_config cnvi_enable_pads[] = {
	/* GPP_B09:     BT_RF_KILL_N */
	PAD_CFG_GPO(GPP_B09, 1, DEEP),
	/* GPP_C10:     WIFI_RF_KILL_N */
	PAD_CFG_GPO(GPP_C10, 1, DEEP),
	/* GPP_F00:     M.2_CNV_BRI_DT_BT_UART2_RTS_N */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F00, NONE, DEEP, NF1),
	/* GPP_F01:     M.2_CNV_BRI_RSP_BT_UART2_RXD */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F01, NONE, DEEP, NF1),
	/* GPP_F02:     M.2_CNV_RGI_DT_BT_UART2_TXD */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F02, NONE, DEEP, NF1),
	/* GPP_F03:     M.2_CNV_RGI_RSP_BT_UART2_CTS_N */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F03, NONE, DEEP, NF1),
	/* GPP_F04:     CNV_RF_RESET_R_N */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F04, NONE, DEEP, NF1),
	/* GPP_F05:     CRF_CLKREQ_R */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F05, NONE, DEEP, NF3),
};

static const struct pad_config cnvi_disable_pads[] = {
	/* GPP_B09:     BT_RF_KILL_N */
	PAD_NC(GPP_B09, NONE),
	/* GPP_C10:     WIFI_RF_KILL_N */
	PAD_NC(GPP_C10, NONE),
	/* GPP_F00:     M.2_CNV_BRI_DT_BT_UART2_RTS_N */
	PAD_NC(GPP_F00, NONE),
	/* GPP_F01:     M.2_CNV_BRI_RSP_BT_UART2_RXD */
	PAD_NC(GPP_F01, NONE),
	/* GPP_F02:     M.2_CNV_RGI_DT_BT_UART2_TXD */
	PAD_NC(GPP_F02, NONE),
	/* GPP_F03:     M.2_CNV_RGI_RSP_BT_UART2_CTS_N */
	PAD_NC(GPP_F03, NONE),
	/* GPP_F04:     CNV_RF_RESET_R_N */
	PAD_NC(GPP_F04, NONE),
	/* GPP_F05:     CRF_CLKREQ_R */
	PAD_NC(GPP_F05, NONE),
};

static const struct pad_config touchscreen_disable_pads[] = {
	/* GPP_F08:     TCH_PNL1_PWR_EN */
	PAD_CFG_GPO(GPP_F08, 0, PLTRST),

	/* GPP_E11:     THC0_SPI1_CLK_TCH_PNL1 */
	PAD_NC(GPP_E11, NONE),
	/* GPP_E12:     THC0_SPI1_IO_0_I2C4_SCL_TCH_PNL1 NF8: I2C4_SCL */
	PAD_NC(GPP_E12, NONE),
	/* GPP_E13:     THC0_SPI1_IO_1_I2C4_SDA_TCH_PNL1 NF8: I2C4 SDA */
	PAD_NC(GPP_E13, NONE),
	/* GPP_E14:     THC0_SPI1_IO_2_TCH_PNL1 */
	PAD_NC(GPP_E14, NONE),
	/* GPP_E15:     THC0_SPI1_IO_3_TCH_PNL1 */
	PAD_NC(GPP_E15, NONE),
	/* GPP_E16:     THC0_SPI1_RST_N_TCH_PNL1 */
	PAD_NC(GPP_E16, NONE),
	/* GPP_E17:     THC0_SPI1_CS0_N_TCH_PNL1 */
	PAD_NC(GPP_E17, NONE),
	/* GPP_E18:     THC0_SPI1_INT_N_TCH_PNL1 */
	PAD_NC(GPP_E18, NONE),
	/* GPP_VGPIO3_THC0: THC0_WOT */
	PAD_NC(GPP_VGPIO3_THC0, NONE),
};

static const struct pad_config touchscreen_thc_i2c_enable_pads[] = {
	/* GPP_E11:     THC0_SPI1_CLK_TCH_PNL1 */
	PAD_NC(GPP_E11, NONE),
	/* GPP_E12:     THC0_SPI1_IO_0_I2C4_SCL_TCH_PNL1 NF1: THC I2C0_SCL */
	PAD_CFG_NF(GPP_E12, NONE, DEEP, NF1),
	/* GPP_E13:     THC0_SPI1_IO_1_I2C4_SDA_TCH_PNL1 NF1: THC I2C0 SDA */
	PAD_CFG_NF(GPP_E13, NONE, DEEP, NF1),
	/* GPP_E14:     THC0_SPI1_IO_2_TCH_PNL1 */
	PAD_NC(GPP_E14, NONE),
	/* GPP_E15:     THC0_SPI1_IO_3_TCH_PNL1 */
	PAD_NC(GPP_E15, NONE),
	/* GPP_E16:     THC0_SPI1_RST_N_TCH_PNL1 */
	PAD_CFG_GPO(GPP_E16, 1, DEEP),
	/* GPP_E17:     THC0_SPI1_CS0_N_TCH_PNL1 */
	PAD_NC(GPP_E17, NONE),
	/* GPP_E18:     THC0_SPI1_INT_N_TCH_PNL1 */
	/* NOTE: this SPI INT NF is also used in THC-I2C mode */
	PAD_CFG_NF(GPP_E18, NONE, DEEP, NF3),
	/* GPP_VGPIO3_THC0: THC0_WOT */
	PAD_NC(GPP_VGPIO3_THC0, NONE),
};

static const struct pad_config touchscreen_thc_spi_enable_pads[] = {
	/* GPP_E11:     THC0_SPI1_CLK_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E11, NONE, DEEP, NF3),
	/* GPP_E12:     THC0_SPI1_IO_0_I2C4_SCL_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E12, NONE, DEEP, NF3),
	/* GPP_E13:     THC0_SPI1_IO_1_I2C4_SDA_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E13, NONE, DEEP, NF3),
	/* GPP_E14:     THC0_SPI1_IO_2_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E14, NONE, DEEP, NF3),
	/* GPP_E15:     THC0_SPI1_IO_3_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E15, NONE, DEEP, NF3),
	/* GPP_E16:     THC0_SPI1_RST_N_TCH_PNL1 NF3: THC HID-SPI */
	/* THC NOTE: use GPO instead of NF for THC0 Rst */
	PAD_CFG_GPO(GPP_E16, 1, DEEP),
	/* GPP_E17:     THC0_SPI1_CS0_N_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E17, NONE, DEEP, NF3),
	/* GPP_E18:     THC0_SPI1_INT_N_TCH_PNL1 NF3: THC HID-SPI */
	PAD_CFG_NF(GPP_E18, NONE, DEEP, NF3),
	/* GPP_VGPIO3_THC0: THC0_WOT */
	PAD_NC(GPP_VGPIO3_THC0, NONE),
};

static const struct pad_config touchpad_thc_i2c_enable_pads[] = {
	/* GPP_F12:     NF1: THC_I2C1_SCL */
	PAD_CFG_NF(GPP_F12, NONE, DEEP, NF1),
	/* GPP_F13:     NF1: THC_I2C1_SDA */
	PAD_CFG_NF(GPP_F13, NONE, DEEP, NF1),
	/* GPP_F18:     TCH_PAD_INT_N */
	/* NOTE: this SPI INT NF is also used in THC-I2C mode */
	/* NOTE: require rework to switch from GPP_A13 to GPP_F18 */
	PAD_CFG_NF(GPP_F18, NONE, DEEP, NF3),
	/* GPP_VGPIO3_THC1: THC1_WOT */
	PAD_NC(GPP_VGPIO3_THC1, NONE),
};

static const struct pad_config touchpad_lpss_i2c_enable_pads[] = {
	/* GPP_F12:     THC_I2C1_SCL_TCH_PAD */
	PAD_CFG_NF(GPP_F12, NONE, DEEP, NF8),
	/* GPP_F13:     THC_I2C1_SDA_TCH_PAD */
	PAD_CFG_NF(GPP_F13, NONE, DEEP, NF8),
	/* GPP_F18:     TCH_PAD_INT_N */
	/* NOTE: require rework to switch from GPP_A13 to GPP_F18 */
	PAD_CFG_GPI_APIC(GPP_F18, NONE, PLTRST, LEVEL, INVERT),
	/* GPP_VGPIO3_THC1: THC1_WOT */
	PAD_NC(GPP_VGPIO3_THC1, NONE),
};

static const struct pad_config touchpad_i2c_disable_pads[] = {
	/* GPP_F12:     THC_I2C1_SCL_TCH_PAD */
	PAD_NC(GPP_F12, NONE),
	/* GPP_F13:     THC_I2C1_SDA_TCH_PAD */
	PAD_NC(GPP_F13, NONE),
	/* GPP_F18:     TCH_PAD_INT_N */
	/* NOTE: require rework to switch from GPP_A13 to GPP_F18 */
	PAD_NC(GPP_F18, NONE),
	/* GPP_VGPIO3_THC1: THC1_WOT */
	PAD_NC(GPP_VGPIO3_THC1, NONE),
};

static const struct pad_config thc0_enable_wake[] = {
	/* GPP_VGPIO3_THC0: THC0_WOT */
	PAD_CFG_GPI_APIC_DRIVER(GPP_VGPIO3_THC0, NONE, PLTRST, LEVEL, NONE),
};

static const struct pad_config thc1_enable_wake[] = {
	/* GPP_VGPIO3_THC1: THC1_WOT */
	PAD_CFG_GPI_APIC_DRIVER(GPP_VGPIO3_THC1, NONE, PLTRST, LEVEL, NONE),
};

static const struct pad_config ish_disable_pads[] = {
	/* GPP_B05:     C_EC_ISH_ALRT */
	PAD_NC(GPP_B05, NONE),
	/* GPP_B07:     SLATEMODE_HALLOUT_SNSR_R */
	PAD_NC(GPP_B07, NONE),
	/* GPP_B18:     ISH_I2C2_SDA_SNSR_HDR */
	PAD_NC(GPP_B18, NONE),
	/* GPP_B19:     ISH_I2C2_SCL_SNSR_HDR */
	PAD_NC(GPP_B19, NONE),
	/* GPP_B22:     ISH_GP_5_SNSR_HDR */
	PAD_NC(GPP_B22, NONE),
	/* GPP_B23:     ISH_GP_6_SNSR_HDR */
	PAD_NC(GPP_B23, NONE),
	/* GPP_D05:     ISH_UART0_ECAIC_RXD */
	PAD_NC(GPP_D05, NONE),
	/* GPP_D06:     ISH_UART0_ECAIC_TXD */
	PAD_NC(GPP_D06, NONE),
	/* GPP_F23:     SMC_LID / ISH_GP9A*/
	PAD_NC(GPP_F23, NONE),
};

static const struct pad_config ish_enable_pads[] = {
	/* GPP_B05:     C_EC_ISH_ALRT */
	PAD_CFG_NF(GPP_B05, NONE, DEEP, NF4),
	/* GPP_B07:     SLATEMODE_HALLOUT_SNSR_R */
	PAD_CFG_NF(GPP_B07, NONE, DEEP, NF4),
	/* GPP_B18:     ISH_I2C2_SDA_SNSR_HDR */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B18, NONE, DEEP, NF1),
	/* GPP_B19:     ISH_I2C2_SCL_SNSR_HDR */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B19, NONE, DEEP, NF1),
	/* GPP_B22:     ISH_GP_5_SNSR_HDR */
	PAD_CFG_NF(GPP_B22, NONE, DEEP, NF4),
	/* GPP_B23:     ISH_GP_6_SNSR_HDR */
	PAD_CFG_NF(GPP_B23, NONE, DEEP, NF4),
	/* GPP_D05:     ISH_UART0_ECAIC_RXD */
	PAD_CFG_NF(GPP_D05, NONE, DEEP, NF2),
	/* GPP_D06:     ISH_UART0_ECAIC_TXD */
	PAD_CFG_NF(GPP_D06, NONE, DEEP, NF2),
	/* GPP_F23:     SMC_LID / ISH_GP9A*/
	PAD_CFG_NF(GPP_F23, NONE, DEEP, NF8),
};

static const struct pad_config fp_disable_pads[] = {
	/* GPP_E05:     GPP_E05_FPS_PWREN */
	PAD_NC(GPP_E05, NONE),
	/* GPP_C15:     FPS_RST_N */
	PAD_NC(GPP_C15, NONE),
	/* GPP_E19:     FPS_INT_N */
	PAD_NC(GPP_E19, NONE),
	/* GPP_E20:     FPS_FW_UPDATE */
	PAD_NC(GPP_E20, NONE),
	/* GPP_E17:     GPP_E17_GSPI0A_CS0 */
	PAD_NC(GPP_E17, NONE),
	/* GPP_F14:     GPP_F14_GPSI0A_MOSI */
	PAD_NC(GPP_F14, NONE),
	/* GPP_F15:     GPP_F15_GSPI0A_MISO */
	PAD_NC(GPP_F15, NONE),
	/* GPP_F16:     GPP_F16_GSPI0A_CLK */
	PAD_NC(GPP_F16, NONE),
};

static const struct pad_config fp_enable_pads[] = {
	/* GPP_E05:     GPP_E5_FPS_PWREN */
	PAD_CFG_GPO(GPP_E05, 1, DEEP),
	/* GPP_C15:     FPS_RST_N */
	PAD_CFG_GPO_LOCK(GPP_C15, 1, LOCK_CONFIG),
	/* GPP_E17:     GPP_E17_GSPI0A_CS0 */
	PAD_CFG_NF(GPP_E17, NONE, DEEP, NF5),
	/* GPP_E19:     FPS_INT_N */
	PAD_CFG_GPI_IRQ_WAKE(GPP_E19, NONE, PWROK, LEVEL, INVERT),
	/* GPP_E20:     FPS_FW_UPDATE */
	PAD_CFG_GPO_LOCK(GPP_E20, 0, LOCK_CONFIG),
	/* GPP_F14:     GPP_F14_GPSI0A_MOSI */
	PAD_CFG_NF(GPP_F14, NONE, DEEP, NF8),
	/* GPP_F15:     GPP_F15_GSPI0A_MISO */
	PAD_CFG_NF(GPP_F15, NONE, DEEP, NF8),
	/* GPP_F16:     GPP_F16_GSPI0A_CLK */
	PAD_CFG_NF(GPP_F16, NONE, DEEP, NF8),
};

static const struct pad_config pre_mem_fp_enable_pads[] = {
	/* GPP_C15:     FPS_RST_N */
	PAD_CFG_GPO(GPP_C15, 0, DEEP),
};

void fw_config_configure_pre_mem_gpio(void)
{
	if (!fw_config_is_provisioned()) {
		printk(BIOS_WARNING, "FW_CONFIG is not provisioned, Exiting\n");
		return;
	}

	if (!fw_config_probe(FW_CONFIG(CELLULAR, CELLULAR_ABSENT)))
		GPIO_CONFIGURE_PADS(pre_mem_wwan_pwr_seq1_pads);

	if (fw_config_probe(FW_CONFIG(STORAGE, STORAGE_NVME_GEN4))) {
		GPIO_CONFIGURE_PADS(pre_mem_gen4_ssd_pwr_pads);
	} else if (fw_config_probe(FW_CONFIG(STORAGE, STORAGE_UNKNOWN))) {
		GPIO_CONFIGURE_PADS(pre_mem_gen4_ssd_pwr_pads);
	}

	/*
	 * NOTE: We place WWAN sequence 2 here. According to the WWAN FIBOCOM
	 * FM350-GL datasheet, the minimum time requirement (Tpr: time between 3.3V
	 * and FCP#) is '0'. Therefore, it will be fine even though there is no
	 * GPIO configured for other PADs via fw_config to have the time delay
	 * introduced in between sequence 1 and 2. Also, FCP# was not the last PAD
	 * configured in sequence 1. Although the Tpr is '0' in the datasheet, three
	 * stages are preserved at this time to guarantee the sequence shown in the
	 * datasheet timing diagram.
	 */
	if (!fw_config_probe(FW_CONFIG(CELLULAR, CELLULAR_ABSENT)))
		GPIO_CONFIGURE_PADS(pre_mem_wwan_pwr_seq2_pads);

	if (fw_config_probe(FW_CONFIG(FP, FP_PRESENT)))
		GPIO_CONFIGURE_PADS(pre_mem_fp_enable_pads);

}

void fw_config_gpio_padbased_override(struct pad_config *padbased_table)
{
	const struct soc_intel_pantherlake_config *config = config_of_soc();

	if (!fw_config_is_provisioned()) {
		printk(BIOS_WARNING, "FW_CONFIG is not provisioned, Exiting\n");
		return;
	}

	if (fw_config_probe(FW_CONFIG(STORAGE, STORAGE_NVME_GEN4))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, gen4_ssd_pads);
	} else if (fw_config_probe(FW_CONFIG(STORAGE, STORAGE_UFS))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, ufs_enable_pads);
	} else if (fw_config_probe(FW_CONFIG(STORAGE, STORAGE_UNKNOWN))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, gen4_ssd_pads);
		GPIO_PADBASED_OVERRIDE(padbased_table, ufs_enable_pads);
	}

	if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_NONE))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, audio_disable_pads);
	} else if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_ALC721_SNDW))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, sndw_alc721_enable_pads);
	} else if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_ALC256_HDA))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, hda_enable_pads);
	} else if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_MAX98360_ALC5682I_I2S))) {
		printk(BIOS_INFO, "Configure GPIOs for I2S MAX98360 ALC5682 audio.\n");
		GPIO_PADBASED_OVERRIDE(padbased_table, i2s_enable_pads);
	}

	if (fw_config_probe(FW_CONFIG(WIFI, WIFI_PCIE_6)) ||
		fw_config_probe(FW_CONFIG(WIFI, WIFI_PCIE_7))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, pcie_wlan_enable_pads);
		GPIO_PADBASED_OVERRIDE(padbased_table, cnvi_disable_pads);
	} else if (fw_config_probe(FW_CONFIG(WIFI, WIFI_CNVI_6)) ||
		fw_config_probe(FW_CONFIG(WIFI, WIFI_CNVI_7))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, cnvi_enable_pads);
		GPIO_PADBASED_OVERRIDE(padbased_table, pcie_wlan_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(CELLULAR, CELLULAR_PCIE)) ||
		fw_config_probe(FW_CONFIG(CELLULAR, CELLULAR_USB))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, wwan_pwr_seq3_pads);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, wwan_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(SD, SD_NONE)))
		GPIO_PADBASED_OVERRIDE(padbased_table, x4slot_disable_pads);
	else
		GPIO_PADBASED_OVERRIDE(padbased_table, x4slot_pads);

	if (fw_config_probe(FW_CONFIG(TOUCHPAD, TOUCHPAD_LPSS_I2C))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, touchpad_lpss_i2c_enable_pads);
	} else if (fw_config_probe(FW_CONFIG(TOUCHPAD, TOUCHPAD_THC_I2C))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, touchpad_thc_i2c_enable_pads);
		if (config->thc_wake_on_touch[1])
			GPIO_PADBASED_OVERRIDE(padbased_table, thc1_enable_wake);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, touchpad_i2c_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(TOUCHSCREEN, TOUCHSCREEN_THC_I2C)))
		GPIO_PADBASED_OVERRIDE(padbased_table, touchscreen_thc_i2c_enable_pads);
	else if (fw_config_probe(FW_CONFIG(TOUCHSCREEN, TOUCHSCREEN_THC_SPI))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, touchscreen_thc_spi_enable_pads);
		if (config->thc_wake_on_touch[0])
			GPIO_PADBASED_OVERRIDE(padbased_table, thc0_enable_wake);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, touchscreen_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(ISH, ISH_ENABLE))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, ish_enable_pads);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, ish_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(FP, FP_PRESENT))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, fp_enable_pads);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, fp_disable_pads);
	}
}
