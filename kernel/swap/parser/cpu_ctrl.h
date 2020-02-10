/**
 * @file parser/cpu_ctrl.h
 * @author Vasiliy Ulyanov <v.ulyanov@samsung.com>
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
 * CPU controls interface.
 */

#ifndef _CPU_CTRL_H_
#define _CPU_CTRL_H_

struct cpumask;

#ifdef CONFIG_SMP
int swap_disable_nonboot_cpus_lock(struct cpumask *mask);
int swap_enable_nonboot_cpus_unlock(struct cpumask *mask);

int init_cpu_deps(void);

#else /* CONFIG_SMP */

static inline int swap_disable_nonboot_cpus_lock(struct cpumask *mask)
{
	return 0;
}

static inline int swap_enable_nonboot_cpus_unlock(struct cpumask *mask)
{
	return 0;
}

static inline int init_cpu_deps(void)
{
	return 0;
}

#endif /* CONFIG_SMP */

#endif /* _CPU_CTRL_H_ */
