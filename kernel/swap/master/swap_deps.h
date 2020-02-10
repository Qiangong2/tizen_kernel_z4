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


#ifndef _SWAP_DEPS_H
#define _SWAP_DEPS_H


struct task_struct;
struct files_struct;


struct files_struct *swap_get_files_struct(struct task_struct *task);
void swap_put_files_struct(struct files_struct *fs);


int chef_once(void);

struct dentry *swap_get_dentry(const char *filepath);
void swap_put_dentry(struct dentry *dentry);


#endif /* _SWAP_DEPS_H */
