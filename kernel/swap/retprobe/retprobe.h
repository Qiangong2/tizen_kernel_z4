/*
 *  SWAP uprobe manager
 *  modules/us_manager/probes/uretprobe.h
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
 * Copyright (C) Samsung Electronics, 2014
 *
 * 2014	 Alexander Aksenov: Probes interface implement
 *
 */

#ifndef __URETPROBE_H__
#define __URETPROBE_H__

/* Common retprobe info */
struct retprobe_info {
	char *args;
	char ret_type;
};

#endif /* __URETPROBE_H__ */
