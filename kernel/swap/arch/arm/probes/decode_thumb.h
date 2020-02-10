/*
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
 * Copyright (C) Samsung Electronics, 2015
 *
 * 2015         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */


#ifndef _ARM_DECODE_THUMB_H
#define _ARM_DECODE_THUMB_H


#include "probes.h"


struct decode_info {
	u32 vaddr;
	void *tramp;
	probe_handler_arm_t handeler;
};


int decode_thumb(u32 insn, struct decode_info *info);


#endif /* _ARM_DECODE_THUMB_H */
