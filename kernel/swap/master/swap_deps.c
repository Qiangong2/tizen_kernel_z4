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


#include <linux/module.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <ksyms/ksyms.h>
#include "swap_deps.h"


static struct files_struct *(*__get_files_struct)(struct task_struct *);
static void (*__put_files_struct)(struct files_struct *fs);

struct files_struct *swap_get_files_struct(struct task_struct *task)
{
	return __get_files_struct(task);
}
EXPORT_SYMBOL_GPL(swap_get_files_struct);

void swap_put_files_struct(struct files_struct *fs)
{
	__put_files_struct(fs);
}
EXPORT_SYMBOL_GPL(swap_put_files_struct);


int chef_once(void)
{
	const char *sym;
	static unsigned once_flag = 0;

	if (once_flag)
		return 0;

	sym = "get_files_struct";
	__get_files_struct = (void *)swap_ksyms(sym);
	if (__get_files_struct == NULL)
		goto not_found;

	sym = "put_files_struct";
	__put_files_struct = (void *)swap_ksyms(sym);
	if (__put_files_struct == NULL)
		goto not_found;

	once_flag = 1;
	return 0;

not_found:
	printk("ERROR: symbol '%s' not found\n", sym);
	return -ESRCH;
}

struct dentry *swap_get_dentry(const char *filepath)
{
	struct path path;
	struct dentry *dentry = NULL;

	if (kern_path(filepath, LOOKUP_FOLLOW, &path) == 0) {
		dentry = dget(path.dentry);
		path_put(&path);
	}

	return dentry;
}
EXPORT_SYMBOL_GPL(swap_get_dentry);

void swap_put_dentry(struct dentry *dentry)
{
	dput(dentry);
}
EXPORT_SYMBOL_GPL(swap_put_dentry);
