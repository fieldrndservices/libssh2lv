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
#include "libssh2_sftp.h"

#include "lv-libssh2.h"
#include "lv-libssh2-status-private.h"
#include "lv-libssh2-session-private.h"
#include "lv-libssh2-sftp-private.h"
#include "lv-libssh2-sftp-attributes-private.h"

lv_libssh2_status_t
lv_libssh2_sftp_create(
    lv_libssh2_session_t* session,
    lv_libssh2_sftp_t** handle
) {
    *handle = NULL;
    LIBSSH2_SFTP* inner = libssh2_sftp_init(session->inner);
    if (inner == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    lv_libssh2_sftp_t* sftp = malloc(sizeof(lv_libssh2_sftp_t));
    if (sftp == NULL) {
        libssh2_sftp_shutdown(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    sftp->inner = inner;
    *handle = sftp;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_destroy(
    lv_libssh2_sftp_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_shutdown(handle->inner);
    if (result != 0) {
        return LV_LIBSSH2_STATUS_ERROR_FREE;
    }
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_last_error(
    lv_libssh2_sftp_t* handle,
    uint32_t* code
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *code = libssh2_sftp_last_error(handle->inner);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_open_file(
    lv_libssh2_sftp_t* sftp,
    const char* path,
    const uint32_t flags,
    const int32_t permissions,
    lv_libssh2_sftp_file_t** handle
) {
    *handle = NULL;
    if (sftp == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_SFTP_HANDLE* inner = libssh2_sftp_open_ex(
        sftp->inner,
        path,
        (unsigned int)strlen(path),
        flags,
        permissions,
        LIBSSH2_SFTP_OPENFILE
    );
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_sftp_last_error(sftp->inner));
    }
    lv_libssh2_sftp_file_t* file = malloc(sizeof(lv_libssh2_sftp_file_t));
    if (file == NULL) {
        libssh2_sftp_close_handle(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    file->inner = inner;
    *handle = file;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_close_file(
    lv_libssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_close_handle(handle->inner);
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_open_directory(
    lv_libssh2_sftp_t* sftp,
    const char* path,
    lv_libssh2_sftp_directory_t** handle
) {
    *handle = NULL;
    if (sftp == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_SFTP_HANDLE* inner = libssh2_sftp_open_ex(
        sftp->inner,
        path,
        (unsigned int)strlen(path),
        0,
        0,
        LIBSSH2_SFTP_OPENDIR
    );
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_sftp_last_error(sftp->inner));
    }
    lv_libssh2_sftp_directory_t* directory = malloc(sizeof(lv_libssh2_sftp_directory_t));
    if (directory == NULL) {
        libssh2_sftp_close_handle(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    directory->inner = inner;
    *handle = directory;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_close_directory(
    lv_libssh2_sftp_directory_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_close_handle(handle->inner);
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_read_file(
    lv_libssh2_sftp_file_t* handle,
    uint8_t* buffer,
    const size_t buffer_max_length,
    ssize_t* read_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_read(handle->inner, (char*)buffer, buffer_max_length);
    if (count < 0) {
        return lv_libssh2_status_from_result((int)count);
    }
    *read_count = count;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_read_directory(
    lv_libssh2_sftp_directory_t* handle,
    uint8_t* buffer,
    const size_t buffer_max_length,
    lv_libssh2_sftp_attributes_t* attributes,
    ssize_t* read_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_readdir_ex(
        handle->inner,
        (char*)buffer,
        buffer_max_length,
        NULL,
        0,
        attributes->inner
    );
    if (count < 0) {
        return lv_libssh2_status_from_result((int)count);
    }
    *read_count = count;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_write_file(
    lv_libssh2_sftp_file_t* handle,
    const uint8_t* buffer,
    const size_t buffer_length,
    ssize_t* write_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_write(
        handle->inner,
        (char*)buffer,
        buffer_length
    );
    if (count < 0) {
        return lv_libssh2_status_from_result((int)count);
    }
    *write_count = count;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_file_sync(
    lv_libssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fsync(handle->inner);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_sftp_file_seek(
    lv_libssh2_sftp_file_t* handle,
    const size_t offset
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_sftp_seek64(handle->inner, offset);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_file_rewind(
    lv_libssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_sftp_seek64(handle->inner, 0);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_file_position(
    lv_libssh2_sftp_file_t* handle,
    uint64_t* position
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    uint64_t pos = libssh2_sftp_tell64(handle->inner);
    *position = pos;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_file_status(
    lv_libssh2_sftp_file_t* handle,
    lv_libssh2_sftp_attributes_t* attributes
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (attributes == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fstat_ex(handle->inner, attributes->inner, 0);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_sftp_file_set_status(
    lv_libssh2_sftp_file_t* handle,
    lv_libssh2_sftp_attributes_t* attributes
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (attributes == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fstat_ex(handle->inner, attributes->inner, 1);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_sftp_link_status(
    lv_libssh2_sftp_t* handle,
    const char* path,
    lv_libssh2_sftp_attributes_t* attributes
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (attributes == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_stat_ex(
        handle->inner,
        path,
        (unsigned int)strlen(path),
        LIBSSH2_SFTP_LSTAT,
        attributes->inner
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_file_rename(
    lv_libssh2_sftp_file_t* handle,
    const char* source_path,
    const char* destination_path
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (source_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (destination_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_rename_ex(
        handle->inner,
        source_path,
        (unsigned int)strlen(source_path),
        destintation_path,
        (unsigned int)strlen(destination_path),
        LIBSSH2_SFTP_RENAME_OVERWRITE | LIBSSH2_SFTP_RENAME_ATOMIC | LIBSSH2_SFTP_RENAME_NATIVE
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_delete_file(
    lv_libssh2_sftp_t* handle,
    const char* file_path
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (file_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_unlink_ex(
        handle->inner,
        file_path,
        (unsigned int)strlen(file_path)
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_create_directory(
    lv_libssh2_sftp_t* handle,
    const char* directory_path,
    const int32_t mode
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (directory_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_mkdir_ex(
        handle->inner,
        directory_path,
        (unsigned int)strlen(directory_path),
        mode
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_delete_directory(
    lv_libssh2_sftp_t* handle,
    const char* directory_path
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (directory_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_rmdir_ex(
        handle->inner,
        directory_path,
        (unsigned int)strlen(directory_path)
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_create_link(
    lv_libssh2_sftp_t* handle,
    const char* source_path,
    const char* link_path
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (source_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (link_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_symlink_ex(
        handle->inner,
        source_path,
        (unsigned int)strlen(source_path),
        link_path,
        (unsigned int)strlen(link_path),
        LIBSSH2_SFTP_SYMLINK
    );
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_resolve_symbolic_link(
    lv_libssh2_sftp_t* handle,
    const char* link_path,
    char* source_path,
    const size_t source_path_max_length,
    size_t* read_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (link_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (source_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (read_count == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *read_count = 0;
    int result = libssh2_sftp_symlink_ex(
        handle->inner,
        link_path,
        (unsigned int)strlen(link_path),
        source_path,
        (unsigned int)source_path_max_length,
        LIBSSH2_SFTP_READLINK
    );
    if (result < 0) {
        return lv_libssh2_status_from_result(result);
    }
    *read_count = (size_t)result;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_resolve_real_link(
    lv_libssh2_sftp_t* handle,
    const char* link_path,
    char* source_path,
    const size_t source_path_max_length,
    size_t* read_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (link_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (source_path == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (read_count == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *read_count = 0;
    int result = libssh2_sftp_symlink_ex(
        handle->inner,
        link_path,
        (unsigned int)strlen(link_path),
        source_path,
        (unsigned int)source_path_max_length,
        LIBSSH2_SFTP_REALPATH
        );
    if (result < 0) {
        return lv_libssh2_status_from_result(result);
    }
    *read_count = (size_t)result;
    return LV_LIBSSH2_STATUS_OK;
}

/* lv_libssh2_status_t */
/* lv_libssh2_sftp_attributes_create( */
/*     lv_libssh2_sftp_attributes_t** handle */
/* ); */

/* lv_libssh2_status_t */
/* lv_libssh2_sftp_attributes_destroy( */
/*     lv_libssh2_sftp_attributes_t* handle */
/* ); */
