/*
 *  Dynamic Binary Instrumentation Module based on KProbes
 *  energy/energy_mod.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Copyright (C) Samsung Electronics, 2013
 *
 * 2013         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */


#include <linux/module.h>
#include <master/swap_initializer.h>
#include "energy.h"
#include "debugfs_energy.h"


SWAP_LIGHT_INIT_MODULE(energy_once, energy_init, energy_uninit,
		       init_debugfs_energy, exit_debugfs_energy);

MODULE_LICENSE("GPL");
