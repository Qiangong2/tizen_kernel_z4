/**
 * @file sampler/swap_sampler_module.h
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
 * Sampling module interface declaration.
 */


/* SWAP Sampler interface */

#ifndef __SWAP_SAMPLER_MODULE_H__
#define __SWAP_SAMPLER_MODULE_H__


typedef void (*swap_sample_cb_t)(struct pt_regs *);


/* Starts the SWAP Sampler */
int swap_sampler_start(unsigned int timer_quantum, swap_sample_cb_t cb);

/* Stops the SWAP Sampler */
int swap_sampler_stop(void);

#endif /* __SWAP_SAMPLER_MODULE_H__ */
