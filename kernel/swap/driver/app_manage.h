/**
 * @file driver/app_manage.h
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
 * Copyright (C) Samsung Electronics, 2014
 *
 * @section DESCRIPTION
 *
 * Driver user <-> kernel connect implement.
 */

#ifndef __APP_MANAGE_H__
#define __APP_MANAGE_H__

#include "us_interaction.h"
#include "us_interaction_msg.h"

/**
 * @brief Sends pause message to kernel.
 *
 * @return us_interaction_send_msg result.
 */
static inline int app_manage_pause_apps(void)
{
	enum us_interaction_k2u_msg_t us_int_msg = US_INT_PAUSE_APPS;

	return us_interaction_send_msg(&us_int_msg, sizeof(us_int_msg));
}

/**
 * @brief Sends continue message to kernel.
 *
 * @return us_interaction_send_msg result.
 */
static inline int app_manage_cont_apps(void)
{
	enum us_interaction_k2u_msg_t us_int_msg = US_INT_CONT_APPS;

	return us_interaction_send_msg(&us_int_msg, sizeof(us_int_msg));
}

#endif /* __APP_MANAGE_H__ */