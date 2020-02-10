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
 * Copyright (C) Samsung Electronics, 2016
 *
 * 2016         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */


#ifndef _SWAP_ASM_PROBES_THUMB_H
#define _SWAP_ASM_PROBES_THUMB_H


int make_trampoline_thumb(struct arch_insn_arm *ainsn, u32 vaddr, u32 insn,
			  u32 *tramp, size_t tramp_len);
int noret_thumb(u32 opcode);


#endif /* _SWAP_ASM_PROBES_THUMB_H */
