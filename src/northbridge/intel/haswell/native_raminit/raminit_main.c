/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <assert.h>
#include <commonlib/bsd/clamp.h>
#include <console/console.h>
#include <cpu/intel/haswell/haswell.h>
#include <delay.h>
#include <device/pci_ops.h>
#include <northbridge/intel/haswell/chip.h>
#include <northbridge/intel/haswell/haswell.h>
#include <northbridge/intel/haswell/raminit.h>
#include <static.h>
#include <string.h>
#include <timer.h>
#include <types.h>

#include "raminit_native.h"

static enum raminit_status pre_training(struct sysinfo *ctrl)
{
	/* Skip on S3 resume */
	if (ctrl->bootmode == BOOTMODE_S3)
		return RAMINIT_STATUS_SUCCESS;

	for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++) {
		for (uint8_t slot = 0; slot < NUM_SLOTS; slot++) {
			if (!rank_in_ch(ctrl, slot + slot, channel))
				continue;

			printk(RAM_DEBUG, "C%uS%u:\n", channel, slot);
			printk(RAM_DEBUG, "\tMR0: 0x%04x\n", ctrl->mr0[channel][slot]);
			printk(RAM_DEBUG, "\tMR1: 0x%04x\n", ctrl->mr1[channel][slot]);
			printk(RAM_DEBUG, "\tMR2: 0x%04x\n", ctrl->mr2[channel][slot]);
			printk(RAM_DEBUG, "\tMR3: 0x%04x\n", ctrl->mr3[channel][slot]);
			printk(RAM_DEBUG, "\n");
		}
		if (ctrl->is_ecc) {
			union mad_dimm_reg mad_dimm = {
				.raw = mchbar_read32(MAD_DIMM(channel)),
			};
			/* Enable ECC I/O */
			mad_dimm.ecc_mode = 1;
			mchbar_write32(MAD_DIMM(channel), mad_dimm.raw);
			/* Wait 4 usec after enabling the ECC I/O, needed by HW */
			udelay(4);
		}
	}
	setup_wdb(ctrl);
	return RAMINIT_STATUS_SUCCESS;
}


static uint8_t get_cmd_stretch(const uint8_t tCMD)
{
	switch (tCMD) {
	case 1:
		return 0;
	case 2:
		return 2;
	case 3:
		return 3;
	default:
		return 2;
	}
}

/** TODO: Why do we re-read the MCHBAR registers? Is the cached value outdated? **/
static void update_command_rate(struct sysinfo *const ctrl, const uint8_t channel)
{
	const uint8_t old_n = clamp_s32(1, ctrl->tc_bankrank_a[channel].cmd_stretch, 3);
	const uint8_t new_n = ctrl->tCMD;

	/* Update CmdN timing */
	ctrl->tc_bankrank_a[channel].raw = mchbar_read32(TC_BANK_RANK_A_ch(channel));
	ctrl->tc_bankrank_a[channel].cmd_stretch = get_cmd_stretch(ctrl->tCMD);
	mchbar_write32(TC_BANK_RANK_A_ch(channel), ctrl->tc_bankrank_a[channel].raw);

	/* Switch to regular tXP value */
	ctrl->tc_bankrank_c[channel].raw = mchbar_read32(TC_BANK_RANK_C_ch(channel));
	ctrl->tc_bankrank_c[channel].tXP = ctrl->tXP;
	mchbar_write32(TC_BANK_RANK_C_ch(channel), ctrl->tc_bankrank_c[channel].raw);

	/* Adjust RT values to compensate */
	const int8_t delta = new_n - old_n;
	for (uint8_t rank = 0; rank < NUM_SLOTRANKS; rank++) {
		if (!rank_in_ch(ctrl, rank, channel))
			continue;

		/*
		 * RT (roundtrip) latency is the time it takes for signals to go
		 * from the memory controller to the DRAM and back to the memory
		 * controller. It's likely the delta is doubled for this reason.
		 */
		ctrl->rt_latency[channel].rank[rank] += delta * 2;
	}
	mchbar_write32(SC_ROUNDT_LAT_ch(channel), ctrl->rt_latency[channel].raw);
}

static enum raminit_status post_training(struct sysinfo *const ctrl)
{
	/* Command rate is always 1N for LPDDR3 */
	if (ctrl->lpddr) {
		return RAMINIT_STATUS_SUCCESS;
	}
	for (uint8_t channel = 0; channel < NUM_CHANNELS; channel++) {
		if (!does_ch_exist(ctrl, channel))
			continue;

		update_command_rate(ctrl, channel);
	}
	return RAMINIT_STATUS_SUCCESS;
}

struct task_entry {
	enum raminit_status (*task)(struct sysinfo *);
	bool is_enabled;
	const char *name;
};

static const struct task_entry cold_boot[] = {
	{ collect_spd_info,                                       true, "PROCSPD",    },
	{ initialise_mpll,                                        true, "INITMPLL",   },
	{ convert_timings,                                        true, "CONVTIM",    },
	{ configure_mc,                                           true, "CONFMC",     },
	{ configure_memory_map,                                   true, "MEMMAP",     },
	{ do_jedec_init,                                          true, "JEDECINIT",  },
	{ pre_training,                                           true, "PRETRAIN",   },
	{ train_sense_amp_offset,                                 true, "SOT",        },
	{ train_receive_enable,                                   true, "RCVET",      },
	{ train_read_mpr,                                         true, "RDMPRT",     },
	{ train_jedec_write_leveling,                             true, "JWRL",       },
	{ optimise_comp,                                          true, "OPTCOMP",    },
	{ post_training,                                          true, "POSTTRAIN",  },
	{ activate_mc,                                            true, "ACTIVATE",   },
	{ save_training_values,                                   true, "SAVE_TRAIN", },
	{ save_non_training,                                      true, "SAVE_NONT",  },
	{ raminit_done,                                           true, "RAMINITEND", },
};

static const struct task_entry fast_boot[] = {
	{ collect_spd_info,                                       true, "PROCSPD",    },
	{ restore_non_training,                                   true, "RST_NONT",   },
	{ initialise_mpll,                                        true, "INITMPLL",   },
	{ configure_mc,                                           true, "CONFMC",     },
	{ configure_memory_map,                                   true, "MEMMAP",     },
	{ do_jedec_init,                                          true, "JEDECINIT",  },
	{ pre_training,                                           true, "PRETRAIN",   },
	{ restore_training_values,                                true, "RST_TRAIN",  },
	{ exit_selfrefresh,                                       true, "EXIT_SR",    },
	{ normal_state,                                           true, "NORMALMODE", },
	{ raminit_done,                                           true, "RAMINITEND", },
};

_Static_assert(ARRAY_SIZE(cold_boot) >= ARRAY_SIZE(fast_boot),
		"Code assumes cold boot task list is the longest one");

/* Return a generic stepping value to make stepping checks simpler */
static enum generic_stepping get_stepping(const uint32_t cpuid)
{
	switch (cpuid) {
	case CPUID_HASWELL_A0:
		die("Haswell stepping A0 is not supported\n");
	case CPUID_HASWELL_B0:
	case CPUID_HASWELL_ULT_B0:
	case CPUID_CRYSTALWELL_B0:
		return STEPPING_B0;
	case CPUID_HASWELL_C0:
	case CPUID_HASWELL_ULT_C0:
	case CPUID_CRYSTALWELL_C0:
		return STEPPING_C0;
	default:
		/** TODO: Add Broadwell support someday **/
		die("Unknown CPUID 0x%x\n", cpuid);
	}
}

static void initialize_ctrl(struct sysinfo *ctrl)
{
	const struct northbridge_intel_haswell_config *cfg = config_of_soc();
	const enum raminit_boot_mode bootmode = ctrl->bootmode;

	memset(ctrl, 0, sizeof(*ctrl));

	ctrl->cpu = cpu_get_cpuid();
	ctrl->stepping = get_stepping(ctrl->cpu);
	ctrl->vdd_mv = is_hsw_ult() ? 1350 : 1500; /** FIXME: Hardcoded, does it matter? **/
	ctrl->dq_pins_interleaved = cfg->dq_pins_interleaved;
	ctrl->restore_mrs = false;
	ctrl->bootmode = bootmode;
}

/** TODO: Adjust unit scale dynamically? **/
#define T_SCALE 1000 /* 1 = usecs, 1000 = msecs */

static enum raminit_status try_raminit(
	struct sysinfo *ctrl,
	const struct task_entry *const schedule,
	const size_t length)
{
	enum raminit_status status = RAMINIT_STATUS_UNSPECIFIED_ERROR;

	long spent_time[ARRAY_SIZE(cold_boot)] = { 0 };
	long total = 0;

	size_t i;
	for (i = 0; i < length; i++) {
		const struct task_entry *const entry = &schedule[i];
		assert(entry);
		assert(entry->name);
		if (!entry->is_enabled)
			continue;

		assert(entry->task);
		printk(RAM_DEBUG, "\nExecuting raminit task %s\n", entry->name);
		struct mono_time prev, curr;
		timer_monotonic_get(&prev);
		status = entry->task(ctrl);
		timer_monotonic_get(&curr);
		spent_time[i] = mono_time_diff_microseconds(&prev, &curr);
		printk(RAM_DEBUG, "\n");
		if (status) {
			i++;
			printk(BIOS_ERR, "raminit failed on step %s\n", entry->name);
			break;
		}
	}

	if (CONFIG(DEBUG_RAM_SETUP)) {
		const char unit_multiplier = T_SCALE == 1 ? 'u' : 'm';
		printk(RAM_DEBUG, "+------------------+------------+\n");
		printk(RAM_DEBUG, "| Task             |      %csecs |\n", unit_multiplier);
		printk(RAM_DEBUG, "+------------------+------------+\n");
		assert(i <= length);
		for (size_t j = 0; j < i; j++) {
			char buf[] = "                ";
			strncpy(buf, schedule[j].name, strlen(schedule[j].name));
			printk(RAM_DEBUG, "| %s | % 10ld |\n", buf, spent_time[j] / T_SCALE);
			total += spent_time[j];
		}
		printk(RAM_DEBUG, "+------------------+------------+\n");
		printk(RAM_DEBUG, "| Total            | % 10ld |\n", total / T_SCALE);
		printk(RAM_DEBUG, "+------------------+------------+\n");
	}

	return status;
}

void raminit_main(const enum raminit_boot_mode bootmode)
{
	/*
	 * The mighty_ctrl struct. Will happily nuke the pre-RAM stack
	 * if left unattended. Make it static and pass pointers to it.
	 */
	static struct sysinfo mighty_ctrl;

	mighty_ctrl.bootmode = bootmode;
	initialize_ctrl(&mighty_ctrl);

	enum raminit_status status = RAMINIT_STATUS_UNSPECIFIED_ERROR;

	if (bootmode != BOOTMODE_COLD) {
		status = try_raminit(&mighty_ctrl, fast_boot, ARRAY_SIZE(fast_boot));
		if (status == RAMINIT_STATUS_SUCCESS)
			return;
	}

	/** TODO: Try more than once **/
	status = try_raminit(&mighty_ctrl, cold_boot, ARRAY_SIZE(cold_boot));

	if (status != RAMINIT_STATUS_SUCCESS)
		die("Memory initialization was met with utmost failure and misery\n");
}
