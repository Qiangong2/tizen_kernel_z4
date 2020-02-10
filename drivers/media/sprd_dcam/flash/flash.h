/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _DCAM_DRV_FLASH_H_
#define _DCAM_DRV_FLASH_H_

#include <linux/types.h>

int sprd_flash_on(void);
int sprd_flash_high_light(void);
int sprd_flash_close(void);
int sprd_flash_cfg(void *param, void *arg);

#ifdef CONFIG_CAMERA_MULTIFLASH
uint32_t sprd_multi_flash_on(uint32_t);
uint32_t sprd_multi_flash_high_light(uint32_t);
uint32_t sprd_multi_flash_close(uint32_t);
#endif

#endif
