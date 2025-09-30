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

static const struct pad_config hda_enable_pads[] = {
	/* GPP_D10:     HDA_BIT_CLK */
	PAD_CFG_NF(GPP_D10, NONE, DEEP, NF1),
	/* GPP_D11:     HDA_SYNC */
	PAD_CFG_NF(GPP_D11, NATIVE, DEEP, NF1),
	/* GPP_D12:     HDA_SDOUT */
	PAD_CFG_NF(GPP_D12, NATIVE, DEEP, NF1),
	/* GPP_D13:     HDA_SDIN0 */
	PAD_CFG_NF(GPP_D13, NATIVE, DEEP, NF1),
	/* GPP_S06:     SOC_DMIC_CLK1 */
	PAD_CFG_NF(GPP_S06, NONE, DEEP, NF5),
	/* GPP_S07:     SOC_DMIC_DATA1 */
	PAD_CFG_NF(GPP_S07, NONE, DEEP, NF5),
};

static const struct pad_config sndw_alc721_enable_pads[] = {
	/* GPP_S02:     SNDW0_CLK */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF3),
	/* GPP_S03:     SNDW0_DATA0 */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF3),
	/* GPP_S06:     SOC_DMIC_CLK1 */
	PAD_CFG_NF(GPP_S06, NONE, DEEP, NF5),
	/* GPP_S07:     SOC_DMIC_DATA1 */
	PAD_CFG_NF(GPP_S07, NONE, DEEP, NF5),
};

static const struct pad_config sndw_alc722_alc1320_enable_pads[] = {
	/* GPP_S00:     SNDW_3_SCL */
	PAD_CFG_NF(GPP_S00, NONE, DEEP, NF1),
	/* GPP_S01:     SNDW_3_SDA */
	PAD_CFG_NF(GPP_S01, NONE, DEEP, NF1),
	/* GPP_S02:     SNDW0_CLK */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF3),
	/* GPP_S03:     SNDW0_DATA0 */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF3),
	/* GPP_S04:     SNDW_2_SCL */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF2),
	/* GPP_S05:     SNDW_2_SDA */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF2),
	/* GPP_S06:     SOC_DMIC_CLK1 */
	PAD_CFG_NF(GPP_S06, NONE, DEEP, NF5),
	/* GPP_S07:     SOC_DMIC_DATA1 */
	PAD_CFG_NF(GPP_S07, NONE, DEEP, NF5),
};

static const struct pad_config enable_dmic_0_pads[] = {
	/* GPP_S02:     SOC_DMIC_CLK0 */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF5),
	/* GPP_S03:     SOC_DMIC_DATA0 */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF5),
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
	/* GPP_A09:     SOC_WWAN_OFF#_SW */
	PAD_CFG_GPO(GPP_A09, 0, PLTRST),
	/* GPP_B20:     SOC_WWAN_RST# */
	PAD_CFG_GPO(GPP_B20, 0, PLTRST),
	/* GPP_D03:     SOC_WWAN_PCIE_RST# */
	PAD_CFG_GPO(GPP_D03, 0, PLTRST),
};

static const struct pad_config pre_mem_wwan_pwr_seq2_pads[] = {
	/* GPP_A09:     SOC_WWAN_OFF#_SW */
	PAD_CFG_GPO(GPP_A09, 1, PLTRST),
};

static const struct pad_config wwan_pwr_seq3_pads[] = {
	/* GPP_D03:     SOC_WWAN_PCIE_RST# */
	PAD_CFG_GPO(GPP_D03, 1, PLTRST),
	/* GPP_B20:     SOC_WWAN_RST# */
	PAD_CFG_GPO(GPP_B20, 1, PLTRST),
	/* GPP_E02:     SOC_WWAN_WAKE2#_R */
	PAD_CFG_GPI_SCI_LOW(GPP_E02, NONE, DEEP, LEVEL),
};

static const struct pad_config wwan_disable_pads[] = {
	/* GPP_A09:     SOC_WWAN_OFF#_SW */
	PAD_NC(GPP_A09, NONE),
	/* GPP_D03:     SOC_WWAN_PCIE_RST#  */
	PAD_NC(GPP_D03, NONE),
	/* GPP_B20:     SOC_WWAN_RST# */
	PAD_NC(GPP_B20, NONE),
	/* GPP_A10:     SOC_WWAN_RA_DIS#_SW  */
	PAD_NC(GPP_A10, NONE),
	/* GPP_E02:     SOC_WWAN_WAKE2#_R */
	PAD_NC(GPP_E02, NONE),
};

static const struct pad_config bridge_disable_pads[] = {
	/* GPP_D18:     CLKREQ_PCIE#6 */
	PAD_NC(GPP_D18, NONE),
};

void fw_config_configure_pre_mem_gpio(void)
{
	if (!fw_config_is_provisioned()) {
		printk(BIOS_WARNING, "FW_CONFIG is not provisioned, Exiting\n");
		return;
	}

	if (!fw_config_probe(FW_CONFIG(WWAN, WWAN_ABSENT)))
		GPIO_CONFIGURE_PADS(pre_mem_wwan_pwr_seq1_pads);

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
	if (!fw_config_probe(FW_CONFIG(WWAN, WWAN_ABSENT)))
		GPIO_CONFIGURE_PADS(pre_mem_wwan_pwr_seq2_pads);
}

void fw_config_gpio_padbased_override(struct pad_config *padbased_table)
{
	if (!fw_config_is_provisioned()) {
		printk(BIOS_WARNING, "FW_CONFIG is not provisioned, Exiting\n");
		return;
	}

	if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_ALC256M_CG_HDA))) {
		printk(BIOS_INFO, "Configure GPIOs for HDA ALC256 mode.\n");
		GPIO_PADBASED_OVERRIDE(padbased_table, hda_enable_pads);
		GPIO_PADBASED_OVERRIDE(padbased_table, enable_dmic_0_pads);
	} else if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_ALC721_SNDW))) {
		printk(BIOS_INFO, "Configure GPIOs for Soundwire ALC721 mode.\n");
		GPIO_PADBASED_OVERRIDE(padbased_table, sndw_alc721_enable_pads);
	} else if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_ALC722_ALC1320_SNDW))) {
		printk(BIOS_INFO, "Configure GPIOs for Soundwire ALC722 & ALC1320 mode.\n");
		GPIO_PADBASED_OVERRIDE(padbased_table, sndw_alc722_alc1320_enable_pads);
	}

	if (fw_config_probe(FW_CONFIG(WWAN, WWAN_PRESENT))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, wwan_pwr_seq3_pads);
	} else {
		GPIO_PADBASED_OVERRIDE(padbased_table, wwan_disable_pads);
	}

	if (fw_config_probe(FW_CONFIG(BRIDGE, BRIDGE_HAYDEN))) {
		GPIO_PADBASED_OVERRIDE(padbased_table, bridge_disable_pads);
	}

}
