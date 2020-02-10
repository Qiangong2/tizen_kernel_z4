/**
 * @author Vyacheslav Cherkashin <v.cherkashin@samsung.com>
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
 * Copyright (C) Samsung Electronics, 2016
 *
 * @section DESCRIPTION
 *
 * SWAP debugfs interface definition.
 */

#ifndef DRIVER_DEBUGFS_H
#define DRIVER_DEBUGFS_H


int driver_debugfs_init(void);
void driver_debugfs_uninit(void);

#endif // DRIVER_DEBUGFS_H
