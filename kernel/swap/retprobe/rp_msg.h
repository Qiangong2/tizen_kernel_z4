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


#ifndef _RP_MSG_H
#define _RP_MSG_H


struct pt_regs;


void rp_msg_entry(struct pt_regs *regs, unsigned long func_addr,
		  const char *fmt);
void rp_msg_exit(struct pt_regs *regs, unsigned long func_addr,
		 char ret_type, unsigned long ret_addr);


#endif /* _RP_MSG_H */
