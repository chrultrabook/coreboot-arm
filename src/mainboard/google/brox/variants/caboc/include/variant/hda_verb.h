/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef MAINBOARD_HDA_VERB_H
#define MAINBOARD_HDA_VERB_H

#include <device/azalia_device.h>

const u32 cim_verb_data[] = {
	/* coreboot specific header */
	0x10ec0236,	// Codec Vendor / Device ID: Realtek ALC236
	0x103C8C60,	// Subsystem ID
	0x0000001A,	// Number of jacks (NID entries)

	AZALIA_RESET(0x1),
	/* NID 0x01, HDA Codec Subsystem ID Verb table */
	AZALIA_SUBVENDOR(0, 0x103C8C60),

	/* Pin Widget Verb Table */

	/* 1 bit reset */
	0x0205001A,
	0x0204C003,
	0x0205001A,
	0x02048003,
	0x0205007F,
	0x02046001,
	0x0205007F,
	0x02040001,

	/*
	 * DMIC
	 * Requirement is to use PCH DMIC.
	 * Vendor suggested to follow verb table to config NID 0x12, NID 0x13.
	 */
	/* Pin widget 0x12 - DMIC */
	AZALIA_PIN_CFG(0, 0x12, 0x411111F0),
	/* Pin widget 0x13 - DMIC */
	AZALIA_PIN_CFG(0, 0x13, 0x40000000),
	/* Pin widget 0x14 - Front (Port-D) */
	AZALIA_PIN_CFG(0, 0x14, 0x90170110),
	/* Pin widget 0x18 - NPC */
	AZALIA_PIN_CFG(0, 0x18, 0x411111F0),
	/* Pin widget 0x19 - MIC2 (Port-F) */
	AZALIA_PIN_CFG(0, 0x19, 0x03A11020),
	/* Pin widget 0x1A - LINE1 (Port-C) */
	AZALIA_PIN_CFG(0, 0x1a, 0x411111F0),
	/* Pin widget 0x1B - NPC */
	AZALIA_PIN_CFG(0, 0x1b, 0x411111F0),
	/* Pin widget 0x1D - BEEP-IN */
	AZALIA_PIN_CFG(0, 0x1d, 0x40600001),
	/* Pin widget 0x1E - NPC */
	AZALIA_PIN_CFG(0, 0x1e, 0x411111F0),
	/* Pin widget 0x21 - HP1-OUT (Port-I) */
	AZALIA_PIN_CFG(0, 0x21, 0x03211040),

	/*
	 * Widget node 0x20 - 1
	 */
	0x0205003C,
	0x02040354,
	0x0205003C,
	0x02040314,
	/*
	 * Widget node 0x20 - 2
	 * Set JD2 pull high
	 */
	0x0205001B,
	0x02040A4B,
	0x0205000B,
	0x02047778,
	/*
	 * Widget node 0x20 - 3
	 */
	0x02050046,
	0x02040004,
	0x05750003,
	0x057409A3,
	/* disable EQ first */
	0x05350000,
	0x0534201A,
	/* Left Channel */
	0x0535001d,
	0x05340800,
	0x0535001e,
	0x05340800,
	0x05350003,
	0x05341F7B,
	0x05350004,
	0x05340000,
	/* Right Channel */
	0x05450000,
	0x05442000,
	0x0545001d,
	0x05440800,
	0x0545001e,
	0x05440800,
	0x05450003,
	0x05441F7B,
	0x05450004,
	0x05440000,
	/* enable EQ */
	0x05350000,
	0x0534E01A,
	/* 2.2W/4ohm */
	0x02050038,
	0x02046901,
	/* Disable AGC and set AGC limit to 0dB */
	0x02050016,
	0x02044C50,
	0x02050012,
	0x0204EBC0,
	/* Set AGC Front and Post gain 0dB then Enable AGC */
	0x02050013,
	0x0204401F,
	0x02050016,
	0x02044E50,
	/* Headphone Pop */
	0x05750007,
	0x057412B2,
	0x05750007,	/* Padding */
	0x057412B2,	/* Padding */
};

const u32 pc_beep_verbs[] = {
	/* Dos beep path - 1 */
	0x02050036,
	0x02047151,
	0x02050010,
	0x02040320,
	/* Dos beep path - 2 */
	0x0143B000,
	0x01470740,
	0x01470C02,
	0x01470C02,
};

AZALIA_ARRAY_SIZES;

#endif
