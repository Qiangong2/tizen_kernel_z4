/**
 * @file sampler/swap_sampler_errors.h
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
 * Sampler error codes.
 */


/**
 * @enum _swap_sampler_errors
 * @brief Sampler errors.
 */
enum _swap_sampler_errors {
	E_SS_SUCCESS = 0,           /**< Success. */
	E_SS_WRONG_QUANTUM = 1      /**< Wrong timer quantum set. */
};
