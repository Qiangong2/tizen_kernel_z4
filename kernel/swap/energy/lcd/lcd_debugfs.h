#ifndef _LCD_DEBUGFS_H
#define _LCD_DEBUGFS_H

/**
 * @file energy/lcd/lcd_debugfs.h
 * @author Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 * @section LICENSE
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
 * Copyright (C) Samsung Electronics, 2013
 *
 * @section DESCRIPTION
 * Debugfs for LСD
 *
 */


struct dentry;
struct lcd_ops;

int register_lcd_debugfs(struct lcd_ops *ops);
void unregister_lcd_debugfs(struct lcd_ops *ops);

int init_lcd_debugfs(struct dentry *energy_dir);
void exit_lcd_debugfs(void);

#endif /* _LCD_DEBUGFS_H */
