/**
 * @file buffer/swap_buffer_to_buffer_queue.h
 * @author Alexander Aksenov <a.aksenov@samsung.com>
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
 * Copyright (C) Samsung Electronics, 2013
 *
 * @section DESCRIPTION
 * SWAP Buffer interface for buffer queue.
 */

#ifndef __SWAP_BUFFER_TO_BUFFER_QUEUE_H__
#define __SWAP_BUFFER_TO_BUFFER_QUEUE_H__

#include <linux/types.h>

int swap_buffer_callback(void *buffer, bool wakeup);

#endif /* __SWAP_BUFFER_TO_BUFFER_QUEUE_H__ */
