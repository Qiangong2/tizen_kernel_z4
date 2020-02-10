/*
 * @file fbiprobe/fbi_msg.h
 *
 * @author Vitaliy Cherepanov <v.cherepanov@samsung.com>
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
 *
 * @section DESCRIPTION
 *
 * Function body instrumetation
 *
 */

#ifndef __FBI_MSG_H__
#define __FBI_MSG_H__

#include <linux/types.h>

void fbi_msg(unsigned long var_id, size_t size, char *msg_buf);

#endif /* __FBI_MSG_H__ */
