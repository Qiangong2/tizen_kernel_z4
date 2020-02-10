/**
 * @file driver/device_driver.h
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
 *
 * SWAP device driver interface declaration.
 */

#ifndef __SWAP_DRIVER_DEVICE_DRIVER_H__
#define __SWAP_DRIVER_DEVICE_DRIVER_H__

/* Create and register device */
int swap_device_init(void);

/* Delete device */
void swap_device_exit(void);

#endif /* __SWAP_DRIVER_DEVICE_DRIVER_H__ */
