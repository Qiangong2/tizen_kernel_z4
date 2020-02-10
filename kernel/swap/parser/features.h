/**
 * @file parser/features.h
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
 * Features control interface declaration.
 */


#ifndef _FEATURES_H
#define _FEATURES_H

struct conf_data;

int once_features(void);

int set_features(struct conf_data *conf);

#endif /* _FEATURES_H */
