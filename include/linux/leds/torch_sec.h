/*
 * torch_sec.h
 * Samsung Mobile Torch Header
 *
 *
 * Copyright (C) 2012 Samsung Electronics, Inc.
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

struct torch_sec_data {
	int brightness_level;
	int max_brightness;
	int default_brightness;
	int brightness_current[10];

	void (*torch_enable)(int brightness);
	void (*torch_disable)(int brightness);
};

void torch_sec_register_flash_led_data(struct torch_sec_data data);
