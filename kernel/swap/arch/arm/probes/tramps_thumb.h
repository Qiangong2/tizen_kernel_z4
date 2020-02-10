/**
 * @author Alexey Gerenkov <a.gerenkov@samsung.com> User-Space Probes initial
 * implementation; Support x86/ARM/MIPS for both user and kernel spaces.
 * @author Ekaterina Gorelkina <e.gorelkina@samsung.com>: redesign module for
 * separating core and arch parts
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
 * Copyright (C) Samsung Electronics, 2006-2010
 *
 */


#ifndef _SWAP_ASM_TRAMPS_THUMB_H
#define _SWAP_ASM_TRAMPS_THUMB_H


#include <linux/types.h>


extern u16 gen_insn_execbuf_thumb[];
extern u16 pc_dep_insn_execbuf_thumb[];
extern u16 b_r_insn_execbuf_thumb[];
extern u16 b_off_insn_execbuf_thumb[];
extern u16 b_cond_insn_execbuf_thumb[];
extern u16 cbz_insn_execbuf_thumb[];


#endif /* _SWAP_ASM_TRAMPS_THUMB_H */
