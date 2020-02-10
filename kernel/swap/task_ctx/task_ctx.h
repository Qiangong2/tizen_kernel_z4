#ifndef _TASK_CTX_H
#define _TASK_CTX_H

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


struct task_struct;

typedef void (*taskctx_t)(void *info);


int taskctx_run(struct task_struct *task, taskctx_t func, void *data);

int taskctx_get(void);
void taskctx_put(void);


#endif /* _TASK_CTX_H */
