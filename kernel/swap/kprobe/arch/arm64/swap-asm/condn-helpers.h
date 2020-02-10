#ifndef _ASM_CONDN_HELPER_H
#define _ASM_CONDN_HELPER_H

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
 * Copyright (C) Samsung Electronics, 2014
 *
 * 2014         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */


typedef unsigned long (probes_pstate_check_t)(unsigned long);


extern probes_pstate_check_t * const probe_condition_checks[16];


#endif /* _ASM_CONDN_HELPER_H */