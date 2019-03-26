/*
 * LV-LIBSSH2 - A LabVIEW-Friendly C library for libssh2
 *
 * Copyright (c) 2018 Field R&D Services, LLC. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * withoutmodification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the Field R&D Services nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Field R&D Services, LLC ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Field R&D Services, LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributor(s):
 *   Christopher R. Field <chris@fieldrndservices.com>
 */

#include <stdbool.h>
#include <stdlib.h>

#include "libssh2.h"

#include "lv_libssh2.h"
#include "lv_libssh2-status-private.h"
#include "lv_libssh2-fileinfo-private.h"

lv_libssh2_status_t
lv_libssh2_fileinfo_create(
    lv_libssh2_fileinfo_t** handle
) {
    *handle = NULL;
    lv_libssh2_fileinfo_t* file_info = malloc(sizeof(lv_libssh2_fileinfo_t));
    if (file_info == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    libssh2_struct_stat* inner = malloc(sizeof(libssh2_struct_stat));
    if (inner == NULL) {
        free(file_info);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    file_info->inner = inner;
    *handle = file_info;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_fileinfo_destroy(
    lv_libssh2_fileinfo_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    free(handle->inner);
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_fileinfo_size(
    lv_libssh2_fileinfo_t* handle,
    uint64_t* size
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *size = handle->inner->st_size;
    return LV_LIBSSH2_STATUS_OK;
}

