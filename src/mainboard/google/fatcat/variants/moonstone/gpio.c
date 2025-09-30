/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <boardid.h>
#include <soc/gpio.h>

/* Pad configuration in ramstage*/
static const struct pad_config gpio_table[] = {
	/* GPP_A00:     ESPI_IO0_EC_R */
	/*  GPP_A00 : GPP_A00 ==> ESPI_IO0_EC_R configured on reset, do not touch */

	/* GPP_A01:     ESPI_IO1_EC_R */
	/*  GPP_A01 : GPP_A01 ==> ESPI_IO1_EC_R configured on reset, do not touch */

	/* GPP_A02:     ESPI_IO2_EC_R */
	/*  GPP_A02 : GPP_A02 ==> ESPI_IO2_EC_R configured on reset, do not touch */

	/* GPP_A03:     ESPI_IO3_EC_R */
	/*  GPP_A03 : GPP_A03 ==> ESPI_IO3_EC_R configured on reset, do not touch */

	/* GPP_A04:     ESPI_CS0_EC_R_N */
	/*  GPP_A04 : GPP_A04 ==> ESPI_CS0_HDR_L configured on reset, do not touch */

	/* GPP_A05:     ESPI_CLK_EC_R */
	/*  GPP_A05 : GPP_A05 ==> ESPI_CLK_HDR configured on reset, do not touch */

	/* GPP_A06:     ESPI_RST_EC_R_N */
	/*  GPP_A06 : GPP_A06 ==> ESPI_RST_HDR configured on reset, do not touch */

	/* GPP_A08:     NC */
	PAD_NC(GPP_A08, NONE),
	/* GPP_A09:     NC */
	PAD_NC(GPP_A09, NONE),
	/* GPP_A10:     NC */
	PAD_NC(GPP_A10, NONE),
	/* GPP_A11:     NC */
	PAD_NC(GPP_A11, NONE),
	/* GPP_A12:     NC */
	PAD_NC(GPP_A12, NONE),
	/* GPP_A13:     MEM_STRAP_0 */
	PAD_CFG_GPI(GPP_A13, NONE, DEEP),
	/* GPP_A15:     NC */
	PAD_NC(GPP_A15, NONE),
	/* GPP_A16:     BT_RF_KILL_N */
	PAD_CFG_GPO(GPP_A16, 1, DEEP),
	/* GPP_A17:     WIFI_RF_KILL_N */
	PAD_CFG_GPO(GPP_A17, 1, DEEP),

	/* GPP_B00:     USBC_SML_CLK_PD */
	PAD_CFG_NF(GPP_B00, NONE, DEEP, NF1),
	/* GPP_B01:     USBC_SML_DATA_PD */
	PAD_CFG_NF(GPP_B01, NONE, DEEP, NF1),
	/* GPP_B02:     NC */
	PAD_NC(GPP_B02, NONE),
	/* GPP_B03:     NC */
	PAD_NC(GPP_B03, NONE),
	/* GPP_B04:     NC */
	PAD_NC(GPP_B04, NONE),
	/* GPP_B05:     ISH_GP_1_SNSR_HDR */
	PAD_CFG_NF(GPP_B05, NONE, DEEP, NF4),
	/* GPP_B06:     NC */
	PAD_NC(GPP_B06, NONE),
	/* GPP_B07:     NC */
	PAD_NC(GPP_B07, NONE),
	/* GPP_B08:     NC */
	PAD_NC(GPP_B08, NONE),
	/* GPP_B09:     NC */
	PAD_NC(GPP_B09, NONE),
	/* GPP_B10:     NC */
	PAD_NC(GPP_B10, NONE),
	/* GPP_B12:     PM_SLP_S0_N */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* GPP_B13:     PLT_RST_N */
	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
	/* GPP_B15:     NC */
	PAD_NC(GPP_B15, NONE),
	/* GPP_B16:     GEN5_SSD_PWREN */
	PAD_CFG_GPO(GPP_B16, 1, PLTRST),
	/* GPP_B17:     NC */
	PAD_NC(GPP_B17, NONE),
	/* GPP_B18:     ISH_I2C2_SDA_SNSR_HDR */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B18, NONE, DEEP, NF1),
	/* GPP_B19:     ISH_I2C2_SCL_SNSR_HDR */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B19, NONE, DEEP, NF1),
	/* GPP_B20:     NC */
	PAD_NC(GPP_B20, NONE),
	/* GPP_B21:     TCP_RETIMER_FORCE_PWR */
	PAD_CFG_GPO(GPP_B21, 0, DEEP),
	/* GPP_B22:     ISH_GP_5_SNSR_HDR */
	PAD_CFG_NF(GPP_B22, NONE, DEEP, NF4),
	/* GPP_B23:     NC */
	PAD_NC(GPP_B23, NONE),
	/* GPP_B24:     MEM_STRAP_3 */
	PAD_CFG_GPI(GPP_B24, NONE, DEEP),
	/* GPP_B25:     MEM_STRAP_2 */
	PAD_CFG_GPI(GPP_B25, NONE, DEEP),

	/* GPP_C00:     NC */
	PAD_NC(GPP_C00, NONE),
	/* GPP_C01:     NC */
	PAD_NC(GPP_C01, NONE),
	/* GPP_C02:     NC */
	PAD_NC(GPP_C02, NONE),
	/* GPP_C03:     NC */
	PAD_NC(GPP_C03, NONE),
	/* GPP_C04:     NC */
	PAD_NC(GPP_C04, NONE),
	/* GPP_C05:     NC */
	PAD_NC(GPP_C05, NONE),
	/* GPP_C06:     EN_FCAM_PWR */
	PAD_CFG_GPO(GPP_C06, 1, DEEP),
	/* GPP_C07:     MEM_CH_SEL */
	PAD_CFG_GPI(GPP_C07, NONE, DEEP),
	/* GPP_C08:     NC */
	PAD_NC(GPP_C08, NONE),
	/* GPP_C09:     NC */
	PAD_NC(GPP_C09, NONE),
	/* GPP_C10:     CLKREQ1_X4_GEN5_M2_SSD_N */
	PAD_CFG_NF(GPP_C10, NONE, DEEP, NF1),
	/* GPP_C11:     NC */
	PAD_NC(GPP_C11, NONE),
	/* GPP_C12:     NC */
	PAD_NC(GPP_C12, NONE),
	/* GPP_C13:     CLKREQ4_X1_GEN4_M2_WLAN_N */
	PAD_CFG_NF(GPP_C13, NONE, DEEP, NF1),
	/* GPP_C14:     NC */
	PAD_NC(GPP_C14, NONE),
	/* GPP_C16:     TBT_LSX0_TXD  */
	PAD_CFG_NF(GPP_C16, NONE, DEEP, NF1),
	/* GPP_C17:     TBT_LSX0_RXD  */
	PAD_CFG_NF(GPP_C17, NONE, DEEP, NF1),
	/* GPP_C18:     NC */
	PAD_NC(GPP_C18, NONE),
	/* GPP_C19:     NC */
	PAD_NC(GPP_C19, NONE),

	/* GPP_D00:     NC */
	PAD_NC(GPP_D00, NONE),
	/* GPP_D01:     NC */
	PAD_NC(GPP_D01, NONE),
	/* GPP_D02:     SOC_WP_OD */
	PAD_CFG_GPI(GPP_D02, NONE, DEEP),
	/* GPP_D03:     NC */
	PAD_NC(GPP_D03, NONE),
	/* GPP_D04:     NC */
	PAD_NC(GPP_D04, NONE),
	/* GPP_D05:     disable ISH_UART0_RXD */
	PAD_NC(GPP_D05, NONE),
	/* GPP_D07:     NC */
	PAD_NC(GPP_D07, NONE),
	/* GPP_D08:     NC */
	PAD_NC(GPP_D08, NONE),
	/* GPP_D09:     I2S_MCLK1_OUT */
	PAD_CFG_NF(GPP_D09, NONE, DEEP, NF2),
	/* GPP_D10:     HDA_BCLK */
	PAD_CFG_NF(GPP_D10, NONE, DEEP, NF1),
	/* GPP_D11:     HDA_SYNC */
	PAD_CFG_NF(GPP_D11, NONE, DEEP, NF1),
	/* GPP_D12:     HDA_SDO */
	PAD_CFG_NF(GPP_D12, NONE, DEEP, NF1),
	/* GPP_D13:     HDA_SDI0 */
	PAD_CFG_NF(GPP_D13, NONE, DEEP, NF1),
	/* GPP_D14:     NC */
	PAD_NC(GPP_D14, NONE),
	/* GPP_D15:     NC */
	PAD_NC(GPP_D15, NONE),
	/* GPP_D16:	DMIC_CLK */
	PAD_CFG_NF(GPP_D16, NONE, DEEP, NF3),
	/* GPP_D17:	DMIC_DATA */
	PAD_CFG_NF(GPP_D17, NONE, DEEP, NF3),
	/* GPP_D18:     NC */
	PAD_NC(GPP_D18, NONE),
	/* GPP_D19:     NC */
	PAD_NC(GPP_D19, NONE),
	/* GPP_D20:     CSE_EARLY_SW */
	PAD_CFG_GPI_SCI_HIGH(GPP_D20, NONE, DEEP, LEVEL),
	/* GPP_D21:     NC */
	PAD_NC(GPP_D21, NONE),
	/* GPP_D22:     NC */
	PAD_NC(GPP_D22, NONE),
	/* GPP_D23:     NC */
	PAD_NC(GPP_D23, NONE),
	/* GPP_D24:     MEM_STRAP_1 */
	PAD_CFG_GPI(GPP_D24, NONE, DEEP),
	/* GPP_D25:     NC */
	PAD_NC(GPP_D25, NONE),

	/* GPP_E01:     NC */
	PAD_NC(GPP_E01, NONE),
	/* GPP_E02:     GSC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC_LOCK(GPP_E02, NONE, LEVEL, INVERT, LOCK_CONFIG),
	/* GPP_E03:     GEN5_SSD_RESET_N */
	PAD_CFG_GPO(GPP_E03, 1, PLTRST),
	/* GPP_E05:     TCHSCR_RPT_EN */
	PAD_CFG_GPO(GPP_E05, 0, PLTRST),
	/* GPP_E06:     NC */
	PAD_NC(GPP_E06, NONE),
	/* GPP_E07 : [] ==> EC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC_LOCK(GPP_E07, NONE, LEVEL, INVERT, LOCK_CONFIG),
	/* GPP_E08:     NC */
	PAD_NC(GPP_E08, NONE),
	/* GPP_E10:     NC */
	PAD_NC(GPP_E10, NONE),
	/* GPP_E12:     TCHPAD_I2C4_SCL */
	PAD_CFG_NF(GPP_E12, NONE, DEEP, NF8),
	/* GPP_E13:     TCHPAD_I2C4_SDA */
	PAD_CFG_NF(GPP_E13, NONE, DEEP, NF8),
	/* GPP_E14:     NC */
	PAD_NC(GPP_E14, NONE),
	/* GPP_E15:     NC */
	PAD_NC(GPP_E15, NONE),
	/* GPP_E16:     NC */
	PAD_NC(GPP_E16, NONE),
	/* GPP_E18:     TCHPAD_INT# */
	PAD_CFG_GPI_APIC(GPP_E18, NONE, PLTRST, LEVEL, INVERT),
	/* GPP_E19:     FPMCU_PWREN */
	PAD_CFG_GPO(GPP_E19, 1, DEEP),
	/* GPP_E21:     I2C_PMC_PD_INT_N */
	PAD_CFG_NF(GPP_E21, NONE, DEEP, NF1),

	/* GPP_F00:     M.2_CNV_BRI_DT_BT_UART2_RTS_N */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
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
	/* GPP_F06:     NC */
	PAD_NC(GPP_F06, NONE),
	/* GPP_F07:     NC */
	PAD_NC(GPP_F07, NONE),
	/* GPP_F08:     EN_TCHSCR_PWR */
	PAD_CFG_GPO(GPP_F08, 1, DEEP),
	/* GPP_F09:     NC */
	PAD_NC(GPP_F09, NONE),
	/* GPP_F10:     ISH_GP_6_SNSR_HDR */
	PAD_CFG_NF(GPP_F10, NONE, DEEP, NF8),
	/* GPP_F11:     NC */
	PAD_NC(GPP_F11, NONE),
	/* GPP_F12:     TCHSCR_I2C5_SCL */
	PAD_CFG_NF(GPP_F12, NONE, DEEP, NF8),
	/* GPP_F13:     TCHSCR_I2C5_SDA */
	PAD_CFG_NF(GPP_F13, NONE, DEEP, NF8),
	/* GPP_F16:     TCHSCR_RST_L */
	PAD_CFG_GPO(GPP_F16, 1, DEEP),
	/* GPP_F17:     NC */
	PAD_NC(GPP_F17, NONE),
	/* GPP_F18:     TCHSCR_INT_L */
	PAD_CFG_GPI_APIC(GPP_F18, NONE, PLTRST, LEVEL, NONE),
	/* GPP_F19:     NC */
	PAD_NC(GPP_F19, NONE),
	/* GPP_F20:     NC */
	PAD_NC(GPP_F20, NONE),
	/* GPP_F22:     NC */
	PAD_NC(GPP_F22, NONE),
	/* GPP_F23:     NC */
	PAD_NC(GPP_F23, NONE),

	/* GPP_H00:     NC */
	PAD_NC(GPP_H00, NONE),
	/* GPP_H01:     NC */
	PAD_NC(GPP_H01, NONE),
	/* GPP_H02:     NC */
	PAD_NC(GPP_H02, NONE),
	/* GPP_H03:     NC */
	PAD_NC(GPP_H03, NONE),
	/* GPP_H04:     NC */
	PAD_NC(GPP_H04, NONE),
	/* GPP_H05:     NC */
	PAD_NC(GPP_H05, NONE),
	/* GPP_H06:     I2C3_SDA_PSS */
	PAD_CFG_NF(GPP_H06, NONE, DEEP, NF1),
	/* GPP_H07:     I2C3_SCL_PSS */
	PAD_CFG_NF(GPP_H07, NONE, DEEP, NF1),
	/* GPP_H08:     UART0_BUF_RXD */
	PAD_CFG_NF(GPP_H08, NONE, DEEP, NF1),
	/* GPP_H09:     UART0_BUF_TXD */
	PAD_CFG_NF(GPP_H09, NONE, DEEP, NF1),
	/* GPP_H10:     NC */
	PAD_NC(GPP_H10, NONE),
	/* GPP_H11:     NC */
	PAD_NC(GPP_H11, NONE),
	/* GPP_H13:     CPU_C10_GATE_N_R */
	PAD_CFG_NF(GPP_H13, NONE, DEEP, NF1),
	/* GPP_H14:     NC */
	PAD_NC(GPP_H14, NONE),
	/* GPP_H15:     NC */
	PAD_NC(GPP_H15, NONE),
	/* GPP_H16:     NC */
	PAD_NC(GPP_H16, NONE),
	/* GPP_H17:     NC */
	PAD_NC(GPP_H17, NONE),
	/* GPP_H19:     NC */
	PAD_NC(GPP_H19, NONE),
	/* GPP_H20:     NC */
	PAD_NC(GPP_H20, NONE),
	/* GPP_H21:     NC */
	PAD_NC(GPP_H21, NONE),
	/* GPP_H22:     NC */
	PAD_NC(GPP_H22, NONE),

	/* GPP_S00:     SNDW3_CLK_CODEC */
	PAD_CFG_NF(GPP_S00, NONE, DEEP, NF1),
	/* GPP_S01:     SNDW3_DATA0_CODEC */
	PAD_CFG_NF(GPP_S01, NONE, DEEP, NF1),
	/* GPP_S02:     SNDW3_DATA1_CODEC */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF1),
	/* GPP_S03:     SNDW3_DATA2_CODEC */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF1),
	/* GPP_S04:	DMIC_CLK_A0 */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF5),
	/* GPP_S05:	DMIC_DATA_0 */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF5),
	/* GPP_S06:     NC */
	PAD_NC(GPP_S06, NONE),
	/* GPP_S07:     NC */
	PAD_NC(GPP_S07, NONE),

	/* GPP_V00:     PM_BATLOW_N */
	PAD_CFG_NF(GPP_V00, NONE, DEEP, NF1),
	/* GPP_V01:     BC_ACOK_MCP */
	PAD_CFG_NF(GPP_V01, NONE, DEEP, NF1),
	/* GPP_V02:     LANWAKE_N_R */
	PAD_CFG_NF(GPP_V02, NONE, DEEP, NF1),
	/* GPP_V03:     PWRBTN_MCP_N */
	PAD_CFG_NF(GPP_V03, NONE, DEEP, NF1),
	/* GPP_V04:     PM_SLP_S3_N */
	PAD_CFG_NF(GPP_V04, NONE, DEEP, NF1),
	/* GPP_V05:     PM_SLP_S4_N */
	PAD_CFG_NF(GPP_V05, NONE, DEEP, NF1),
	/* GPP_V06:     PM_SLP_A_N */
	PAD_CFG_NF(GPP_V06, NONE, DEEP, NF1),
	/* GPP_V07:     SUSCLK */
	PAD_CFG_NF(GPP_V07, NONE, DEEP, NF1),
	/* GPP_V08:     SLP_WLAN_N */
	PAD_CFG_NF(GPP_V08, NONE, DEEP, NF1),
	/* GPP_V09:     PM_SLP_S5_N */
	PAD_CFG_NF(GPP_V09, NONE, DEEP, NF1),
	/* GPP_V10:     LANPHYPC_R_N */
	PAD_CFG_NF(GPP_V10, NONE, DEEP, NF1),
	/* GPP_V11:     PM_SLP_LAN_N */
	PAD_CFG_NF(GPP_V11, NONE, DEEP, NF1),
	/* GPP_V12:     WAKE_N */
	PAD_CFG_NF(GPP_V12, NONE, DEEP, NF1),
	/* GPP_V13:     GPP_V13_CATERR_N */
	PAD_CFG_NF(GPP_V13, NONE, DEEP, NF1),
	/* GPP_V14:     GPP_V14_FORCEPR_N */
	PAD_CFG_NF(GPP_V14, NONE, DEEP, NF1),
	/* GPP_V15:     GPP_V15_THERMTRIP_N */
	PAD_CFG_NF(GPP_V15, NONE, DEEP, NF1),
	/* GPP_V16:     GPP_V16_VCCST_EN */
	PAD_CFG_NF(GPP_V16, NONE, DEEP, NF1),
	/* GPP_V17:     SLP_S0_GATE_R */
	PAD_CFG_GPO(GPP_V17, 1, PLTRST),
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* GPP_H08:     UART0_BUF_RXD */
	PAD_CFG_NF(GPP_H08, NONE, DEEP, NF1),
	/* GPP_H09:     UART0_BUF_TXD */
	PAD_CFG_NF(GPP_H09, NONE, DEEP, NF1),
	/* GPP_H06: I2C3_SDA_PSS */
	PAD_CFG_NF(GPP_H06, NONE, DEEP, NF1),
	/* GPP_H07: I2C3_SCL_PSS */
	PAD_CFG_NF(GPP_H07, NONE, DEEP, NF1),
	/* GPP_E02: GSC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_E02, NONE, PLTRST, LEVEL, INVERT),
};

/* Pad configuration in romstage */
static const struct pad_config romstage_gpio_table[] = {
	/* GPP_B16:     GEN5_SSD_PWREN */
	PAD_CFG_GPO(GPP_B16, 1, PLTRST),
	/* GPP_C00:     GPP_C0_SMBCLK */
	PAD_CFG_NF(GPP_C00, NONE, DEEP, NF1),
	/* GPP_C01:     GPP_C1_SMBDATA */
	PAD_CFG_NF(GPP_C01, NONE, DEEP, NF1),
	/* GPP_E03:     GEN5_SSD_RESET_N */
	PAD_CFG_GPO(GPP_E03, 1, PLTRST),
};

const struct pad_config *variant_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(gpio_table);
	return gpio_table;
}

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

/* Create the stub for romstage gpio, typically use for power sequence */
const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}

static const struct cros_gpio cros_gpios[] = {
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE0_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE1_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE2_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE3_NAME),
	CROS_GPIO_WP_AH(GPIO_PCH_WP, CROS_GPIO_DEVICE4_NAME),
};

DECLARE_CROS_GPIOS(cros_gpios);
