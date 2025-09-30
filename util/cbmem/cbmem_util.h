/* SPDX-License-Identifier: GPL-2.0-only */

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include <commonlib/bsd/cbmem_id.h>
#include <commonlib/bsd/helpers.h>

extern int cbmem_util_verbose;

#define debug(x...)                                                            \
	do {                                                                   \
		if (cbmem_util_verbose > 1)                                    \
			printf("[%s:%d %s()] ", __FILE__, __LINE__, __func__); \
		if (cbmem_util_verbose)                                        \
			printf(x);                                             \
	} while (0)

#define die(x...)                   \
	do {                        \
		fprintf(stderr, x); \
		exit(1);            \
	} while (0)

struct cbmem_console {
	uint32_t size;
	uint32_t cursor;
	uint8_t body[];
} __packed;

#define CBMC_CURSOR_MASK ((1 << 28) - 1)
#define CBMC_OVERFLOW    (1 << 31)

/**
 * Function pointer type used by CBMEM foreach iteration calls.
 *
 * @param id CBMEM_ID_* value.
 * @param physical_address CBMEM entry address in physical memory.
 * @param buf heap-allocated buffer with CBMEM entry contents.
 * @param size is the size of CBMEM entry in bytes.
 * @param data callback-specific context data.
 *
 * @returns true if iteration should finish, false if it should continue.
 */
typedef bool (*cbmem_iterator_callback)(const uint32_t id, const uint64_t physical_address,
					const uint8_t *buf, const size_t size, void *data);

/* Common CBMEM access API */

enum cbmem_drv_backend_type {
	CBMEM_DRV_BACKEND_ANY,
	CBMEM_DRV_BACKEND_DEVMEM,
	CBMEM_DRV_BACKEND_SYSFS,
};

/**
 * Pick and initialize CBMEM driver. Function can either probe for available drivers or initialize only the selected one.
 *
 * @param backend preferred CBMEM backend to initialize.
 * @param writeable initialize CBMEM driver in writeable mode. Required by devmem driver to modify CBMEM.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_drv_init(enum cbmem_drv_backend_type backend, bool writeable);

/**
 * Cleanup and terminate previously initialized CBMEM driver.
 * **MUST** be called if cbmem_drv_init() succeeded.
 */
void cbmem_drv_terminate(void);

/**
 * Get CBMEM entry as an allocated buffer.
 *
 * @param id CBMEM_ID_* value.
 * @param buf_out return pointer for the allocated buffer containing entry contents.
 * @param size_out size of returned buffer.
 * @param addr_out pointer to the output buffer for entry address in physical memory. Optional.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_drv_get_cbmem_entry(uint32_t id, uint8_t **buf_out, size_t *size_out, uint64_t *addr_out);

/**
 * Write provided buffer contents to the CBMEM entry.
 *
 * @param id CBMEM_ID_* value.
 * @param buf pointer to the source buffer.
 * @param buf_size size of the source buffer.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_drv_write_cbmem_entry(uint32_t id, uint8_t *buf, size_t buf_size);

/**
 * Backend-specific function iterating over CBMEM entries.
 *
 * @param cb user callback function to call during iteration.
 * @param data pointer to the context data for the callback.
 * @param with_contents tells whether the callback should get NULL (false) or copy of the entry (true).
 */
void cbmem_drv_foreach_cbmem_entry(cbmem_iterator_callback cb, void *data, bool with_contents);

/* API for accessing CBMEM via /dev/mem */

/**
 * Initialize the driver.
 *
 * @param writeable tries to map CBMEM in R/W mode.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_devmem_init(bool writeable);

/**
 * Cleanup and terminate the driver. **MUST** be called if cbmem_devmem_init succeeded before.
 */
void cbmem_devmem_terminate(void);

/**
 * Get CBMEM entry as an allocated buffer.
 *
 * @param id CBMEM_ID_* value.
 * @param buf_out return pointer for the allocated buffer containing entry contents.
 * @param size_out size of returned buffer. Optional.
 * @param addr_out pointer to the output buffer for entry address in physical memory. Optional.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_devmem_get_cbmem_entry(uint32_t id, uint8_t **buf_out, size_t *size_out, uint64_t *addr_out);

/**
 * Write provided buffer contents to the CBMEM entry.
 *
 * @param id CBMEM_ID_* value.
 * @param buf pointer to the source buffer.
 * @param buf_size size of the source buffer.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_devmem_write_cbmem_entry(uint32_t id, uint8_t *buf, size_t buf_size);

/**
 * Backend-specific function iterating over CBMEM entries.
 *
 * @param cb user callback function to call during iteration.
 * @param data pointer to the context data for the callback.
 * @param with_contents tells whether the callback should get NULL (false) or copy of the entry (true).
 */
void cbmem_devmem_foreach_cbmem_entry(cbmem_iterator_callback cb, void *data,
				      bool with_contents);

/* API for accessing CBMEM via sysfs entries */

/**
 * Initialize the sysfs driver.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_sysfs_init(void);

/**
 * Get CBMEM entry as an allocated buffer.
 *
 * @param id CBMEM_ID_* value.
 * @param buf_out return pointer for the allocated buffer containing entry contents.
 * @param size_out size of returned buffer.
 * @param addr_out pointer to the output buffer for entry address in physical memory. Optional.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_sysfs_get_cbmem_entry(uint32_t id, uint8_t **buf_out, size_t *size_out, uint64_t *addr_out);

/**
 * Write provided buffer contents to the CBMEM entry.
 *
 * @param id CBMEM_ID_* value.
 * @param buf pointer to the source buffer.
 * @param buf_size size of the source buffer.
 *
 * @returns true on success, false otherwise.
 */
bool cbmem_sysfs_write_cbmem_entry(uint32_t id, uint8_t *buf, size_t buf_size);

/**
 * Backend-specific function iterating over CBMEM entries.
 *
 * @param cb user callback function to call during iteration.
 * @param data pointer to the context data for the callback.
 * @param with_contents tells whether the callback should get NULL (false) or copy of the entry (true).
 */
void cbmem_sysfs_foreach_cbmem_entry(cbmem_iterator_callback cb, void *data,
				     bool with_contents);
