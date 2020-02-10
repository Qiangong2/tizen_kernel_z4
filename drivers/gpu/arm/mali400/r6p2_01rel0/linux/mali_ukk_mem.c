/*
 * Copyright (C) 2010-2016 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <linux/fs.h>       /* file system operations */
#include <asm/uaccess.h>    /* user space access */

#include "mali_ukk.h"
#include "mali_osk.h"
#include "mali_kernel_common.h"
#include "mali_session.h"
#include "mali_ukk_wrappers.h"

#if SPRD_SOC
int mem_init_wrapper(struct mali_session_data *session_data, _mali_uk_init_mem_s __user *uargs)
{
    _mali_uk_init_mem_s kargs;
    _mali_osk_errcode_t err;

    MALI_CHECK_NON_NULL(uargs, -EINVAL);

    kargs.ctx = (uintptr_t)session_data;
    err = _mali_ukk_init_mem(&kargs);
    if (_MALI_OSK_ERR_OK != err)
    {
        return map_errcode(err);
    }

    if (0 != put_user(kargs.mali_address_base, &uargs->mali_address_base)) goto mem_init_rollback;
    if (0 != put_user(kargs.memory_size, &uargs->memory_size)) goto mem_init_rollback;

    return 0;

mem_init_rollback:
	{
		_mali_uk_term_mem_s kargs;
		kargs.ctx = (uintptr_t)session_data;
		err = _mali_ukk_term_mem(&kargs);
		if (_MALI_OSK_ERR_OK != err)
		{
			MALI_DEBUG_PRINT(4, ("reverting _mali_ukk_init_mem, as a result of failing put_user(), failed\n"));
		}
	}
    return -EFAULT;
}

int mem_term_wrapper(struct mali_session_data *session_data, _mali_uk_term_mem_s __user *uargs)
{
    _mali_uk_term_mem_s kargs;
    _mali_osk_errcode_t err;

    MALI_CHECK_NON_NULL(uargs, -EINVAL);

    kargs.ctx = (uintptr_t)session_data;
    err = _mali_ukk_term_mem(&kargs);
    if (_MALI_OSK_ERR_OK != err)
    {
        return map_errcode(err);
    }

    return 0;
}
#endif

int mem_alloc_wrapper(struct mali_session_data *session_data, _mali_uk_alloc_mem_s __user *uargs)
{
	_mali_uk_alloc_mem_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_alloc_mem_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_allocate(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.backend_handle, &uargs->backend_handle)) {
		return -EFAULT;
	}

	return 0;
}

int mem_free_wrapper(struct mali_session_data *session_data, _mali_uk_free_mem_s __user *uargs)
{
	_mali_uk_free_mem_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_free_mem_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_free(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.free_pages_nr, &uargs->free_pages_nr)) {
		return -EFAULT;
	}

	return 0;
}

int mem_bind_wrapper(struct mali_session_data *session_data, _mali_uk_bind_mem_s __user *uargs)
{
	_mali_uk_bind_mem_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_bind_mem_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_bind(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	return 0;
}

int mem_unbind_wrapper(struct mali_session_data *session_data, _mali_uk_unbind_mem_s __user *uargs)
{
	_mali_uk_unbind_mem_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_unbind_mem_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_unbind(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	return 0;
}


int mem_cow_wrapper(struct mali_session_data *session_data, _mali_uk_cow_mem_s __user *uargs)
{
	_mali_uk_cow_mem_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_cow_mem_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_cow(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.backend_handle, &uargs->backend_handle)) {
		return -EFAULT;
	}

	return 0;
}

int mem_cow_modify_range_wrapper(struct mali_session_data *session_data, _mali_uk_cow_modify_range_s __user *uargs)
{
	_mali_uk_cow_modify_range_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_cow_modify_range_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_cow_modify_range(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.change_pages_nr, &uargs->change_pages_nr)) {
		return -EFAULT;
	}
	return 0;
}


int mem_resize_mem_wrapper(struct mali_session_data *session_data, _mali_uk_mem_resize_s __user *uargs)
{
	_mali_uk_mem_resize_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_mem_resize_s))) {
		return -EFAULT;
	}
	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_mem_resize(&kargs);

	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	return 0;
}

int mem_write_safe_wrapper(struct mali_session_data *session_data, _mali_uk_mem_write_safe_s __user *uargs)
{
	_mali_uk_mem_write_safe_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_mem_write_safe_s))) {
		return -EFAULT;
	}

	kargs.ctx = (uintptr_t)session_data;

	/* Check if we can access the buffers */
	if (!access_ok(VERIFY_WRITE, kargs.dest, kargs.size)
	    || !access_ok(VERIFY_READ, kargs.src, kargs.size)) {
		return -EINVAL;
	}

	/* Check if size wraps */
	if ((kargs.size + kargs.dest) <= kargs.dest
	    || (kargs.size + kargs.src) <= kargs.src) {
		return -EINVAL;
	}

	err = _mali_ukk_mem_write_safe(&kargs);
	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.size, &uargs->size)) {
		return -EFAULT;
	}

	return 0;
}



int mem_query_mmu_page_table_dump_size_wrapper(struct mali_session_data *session_data, _mali_uk_query_mmu_page_table_dump_size_s __user *uargs)
{
	_mali_uk_query_mmu_page_table_dump_size_s kargs;
	_mali_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = (uintptr_t)session_data;

	err = _mali_ukk_query_mmu_page_table_dump_size(&kargs);
	if (_MALI_OSK_ERR_OK != err) return map_errcode(err);

	if (0 != put_user(kargs.size, &uargs->size)) return -EFAULT;

	return 0;
}

int mem_dump_mmu_page_table_wrapper(struct mali_session_data *session_data, _mali_uk_dump_mmu_page_table_s __user *uargs)
{
	_mali_uk_dump_mmu_page_table_s kargs;
	_mali_osk_errcode_t err;
	void __user *user_buffer;
	void *buffer = NULL;
	int rc = -EFAULT;

	/* validate input */
	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	/* the session_data pointer was validated by caller */

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_dump_mmu_page_table_s)))
		goto err_exit;

	user_buffer = (void __user *)(uintptr_t)kargs.buffer;
	if (!access_ok(VERIFY_WRITE, user_buffer, kargs.size))
		goto err_exit;

	/* allocate temporary buffer (kernel side) to store mmu page table info */
	if (kargs.size <= 0)
		return -EINVAL;
	/* Allow at most 8MiB buffers, this is more than enough to dump a fully
	 * populated page table. */
	if (kargs.size > SZ_8M)
		return -EINVAL;

	buffer = (void *)(uintptr_t)_mali_osk_valloc(kargs.size);
	if (NULL == buffer) {
		rc = -ENOMEM;
		goto err_exit;
	}

	kargs.ctx = (uintptr_t)session_data;
	kargs.buffer = (uintptr_t)buffer;
	err = _mali_ukk_dump_mmu_page_table(&kargs);
	if (_MALI_OSK_ERR_OK != err) {
		rc = map_errcode(err);
		goto err_exit;
	}

	/* copy mmu page table info back to user space and update pointers */
	if (0 != copy_to_user(user_buffer, buffer, kargs.size))
		goto err_exit;

	kargs.register_writes = kargs.register_writes -
				(uintptr_t)buffer + (uintptr_t)user_buffer;
	kargs.page_table_dump = kargs.page_table_dump -
				(uintptr_t)buffer + (uintptr_t)user_buffer;

	if (0 != copy_to_user(uargs, &kargs, sizeof(kargs)))
		goto err_exit;

	rc = 0;

err_exit:
	if (buffer) _mali_osk_vfree(buffer);
	return rc;
}

int mem_usage_get_wrapper(struct mali_session_data *session_data, _mali_uk_profiling_memory_usage_get_s __user *uargs)
{
	_mali_osk_errcode_t err;
	_mali_uk_profiling_memory_usage_get_s kargs;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_mali_uk_profiling_memory_usage_get_s))) {
		return -EFAULT;
	}

	kargs.ctx = (uintptr_t)session_data;
	err = _mali_ukk_mem_usage_get(&kargs);
	if (_MALI_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	kargs.ctx = (uintptr_t)NULL; /* prevent kernel address to be returned to user space */
	if (0 != copy_to_user(uargs, &kargs, sizeof(_mali_uk_profiling_memory_usage_get_s))) {
		return -EFAULT;
	}

	return 0;
}

#if TIZEN_GLES_MEM_PROFILE
int mem_profile_gles_mem(struct mali_session_data *session_data,
				_mali_uk_gles_mem_profiler_s __user *uargs)
{
	_mali_uk_gles_mem_profiler_s kargs;
	struct mali_session_gles_mem_profile_info *info;
	struct mali_session_gles_mem_profile_api_info *api, *tmp;
	mali_bool flag = MALI_FALSE;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	if (copy_from_user(&kargs, uargs, sizeof(_mali_uk_gles_mem_profiler_s)))
		return -EFAULT;

	kargs.ctx = (uintptr_t)session_data;

	info = &session_data->gles_mem_profile_info[kargs.type];
	if (!info) {
		MALI_PRINT_ERROR(("No info is available. Something wrong"));
		return -EINVAL;
	}

	_MALI_OSK_LIST_FOREACHENTRY(api, tmp, &(info->api_head),
			struct mali_session_gles_mem_profile_api_info, link) {
		if (api->id == (u16)kargs.entrypoint) {
			/* This API is recorded already. Update it. */
			api->size += kargs.size;
			info->size += kargs.size;
			flag = MALI_TRUE;
			break;
		}
	}

	if (flag == MALI_FALSE) {
		/*
		 * This is the first time the API is recorded for this info.
		 * So add it.
		 */
		api = (struct mali_session_gles_mem_profile_api_info *)
			_mali_osk_calloc(1,
			sizeof(struct mali_session_gles_mem_profile_api_info));
		if (!api) {
			MALI_PRINT_ERROR(("Alloc failure"));
			return -ENOMEM;
		}
		memcpy(&api->api, &kargs.api, sizeof(api->api));

		api->id = (u16)kargs.entrypoint;
		api->size += kargs.size;
		info->size += kargs.size;

		mali_session_gles_mem_profile_api_add(info, api,
							&(info->api_head));
	}

	return 0;
}
#endif
