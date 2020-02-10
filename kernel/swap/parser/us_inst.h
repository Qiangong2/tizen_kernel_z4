/**
 * @file parser/us_inst.h
 * @author Vyacheslav Cherkashin
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
 * User-space instrumentation controls interface.
 */


#ifndef _US_INST_H
#define _US_INST_H

/**
 * @enum MOD_TYPE
 * @brief Type of mod_us_inst behaviour. */
enum MOD_TYPE {
	MT_ADD,             /**< Add probes. */
	MT_DEL              /**< Remove probes. */
};

int mod_us_inst(struct list_head *head, enum MOD_TYPE mt);
void pfg_put_all(void);

#endif /* _US_INST_H */
