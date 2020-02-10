/**
 * @file kprobe/arch/asm-arm/memory_rwx.h
 *
 * @author Vyacheslav Cherkashin <v.cherkashin@samsung.com> new memory allocator for slots
 *
 * @section LICENSE
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
 * @section COPYRIGHT
 *
 * Copyright (C) Samsung Electronics, 2014
 */


#ifndef _MEMORY_RWX_H
#define _MEMORY_RWX_H


int mem_rwx_once(void);
void mem_rwx_write_u32(unsigned long addr, unsigned long val);


#endif /* _MEMORY_RWX_H */
