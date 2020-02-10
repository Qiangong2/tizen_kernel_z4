/*
 * Copyright (C) 2013-2015 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __MALI_MEMORY_OS_ALLOC_H__
#define __MALI_MEMORY_OS_ALLOC_H__

#include "mali_osk.h"
#include "mali_memory_types.h"


/** @brief Release Mali OS memory
 *
 * The session memory_lock must be held when calling this function.
 *
 * @param mem_bkend Pointer to the mali_mem_backend to release
 */
u32 mali_mem_os_release(mali_mem_backend *mem_bkend);

_mali_osk_errcode_t mali_mem_os_get_table_page(mali_dma_addr *phys, mali_io_address *mapping);

void mali_mem_os_release_table_page(mali_dma_addr phys, void *virt);

_mali_osk_errcode_t mali_mem_os_init(void);

void mali_mem_os_term(void);

u32 mali_mem_os_stat(void);

void mali_mem_os_free_page_node(struct mali_page_node *m_page);

int mali_mem_os_alloc_pages(mali_mem_os_mem *os_mem, u32 size);

u32 mali_mem_os_free(struct list_head *os_pages, u32 pages_count, mali_bool cow_flag);

_mali_osk_errcode_t mali_mem_os_put_page(struct page *page);

void mali_mem_os_mali_map(mali_mem_backend *mem_bkend, u32 vaddr, u32 props);

void mali_mem_os_mali_unmap(mali_mem_allocation *alloc);

int mali_mem_os_cpu_map(mali_mem_backend *mem_bkend, struct vm_area_struct *vma);

#endif /* __MALI_MEMORY_OS_ALLOC_H__ */
