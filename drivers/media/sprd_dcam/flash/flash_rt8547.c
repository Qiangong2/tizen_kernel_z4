/*
* * Copyright (C) 2012 Spreadtrum Communications Inc.
* *
* * This software is licensed under the terms of the GNU General Public
* * License version 2, as published by the Free Software Foundation, and
* * may be copied, distributed, and modified under those terms.
* *
* * This program is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* * GNU General Public License for more details.
* */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <soc/sprd/hardware.h>
#include <soc/sprd/board.h>
#include <soc/sprd/adi.h>
#include <linux/leds.h>
#include <linux/highmem.h>
#include <linux/freezer.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <video/sprd_img.h>

#include "flash_rt8547.h"
#include <linux/leds/flashlight.h>
#include <linux/leds/rt8547_fled.h>


uint32_t rt8547_flash_on(uint32_t flash_index)
{
	rt8547_control(FLASHLIGHT_MODE_TORCH, TORCH_CURRENT_125MA);
	return 0;
}

uint32_t rt8547_flash_high_light(uint32_t flash_index)
{
	rt8547_control(FLASHLIGHT_MODE_FLASH, STROBE_CURRENT_1100MA);
	return 0;
}

uint32_t rt8547_flash_close(uint32_t flash_index)
{
	rt8547_control(FLASHLIGHT_MODE_OFF, 0);
	return 0;
}

