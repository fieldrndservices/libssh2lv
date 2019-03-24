/*
 * LabSSH2 - A LabVIEW-Friendly C library for libssh2
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

#include "labssh2.h"
#include "labssh2-status-private.h"
#include "labssh2-session-private.h"
#include "labssh2-sftp-private.h"
#include "labssh2-sftp-attributes-private.h"

labssh2_status_t
labssh2_sftp_create(
    labssh2_session_t* session,
    labssh2_sftp_t** handle
) {
    *handle = NULL;
    LIBSSH2_SFTP* inner = libssh2_sftp_init(session->inner);
    if (inner == NULL) {
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    labssh2_sftp_t* sftp = malloc(sizeof(labssh2_sftp_t));
    if (sftp == NULL) {
        libssh2_sftp_shutdown(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    sftp->inner = inner;
    *handle = sftp;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_destroy(
    labssh2_sftp_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_shutdown(handle->inner);
    if (result != 0) {
        return LABSSH2_STATUS_ERROR_FREE;
    }
    handle->inner = NULL;
    free(handle);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_last_error(
    labssh2_sftp_t* handle,
    uint32_t* code
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *code = libssh2_sftp_last_error(handle->inner);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_open_file(
    labssh2_sftp_t* sftp,
    const char* path,
    uint32_t flags,
    int32_t permissions,
    labssh2_sftp_file_t** handle
) {
    *handle = NULL;
    if (sftp == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_SFTP_HANDLE* inner = libssh2_sftp_open_ex(
        sftp->inner,
        path,
        strlen(path),
        flags,
        permissions,
        LIBSSH2_SFTP_OPENFILE
    );
    if (inner == NULL) {
        return labssh2_status_from_result(libssh2_sftp_last_error(sftp->inner));
    }
    labssh2_sftp_file_t* file = malloc(sizeof(labssh2_sftp_file_t));
    if (file == NULL) {
        libssh2_sftp_close_handle(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    file->inner = inner;
    *handle = file;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_close_file(
    labssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_close_handle(handle->inner);
    if (result != 0) {
        return labssh2_status_from_result(result);
    }
    handle->inner = NULL;
    free(handle);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_open_directory(
    labssh2_sftp_t* sftp,
    const char* path,
    labssh2_sftp_directory_t** handle
) {
    *handle = NULL;
    if (sftp == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_SFTP_HANDLE* inner = libssh2_sftp_open_ex(
        sftp->inner,
        path,
        strlen(path),
        0,
        0,
        LIBSSH2_SFTP_OPENDIR
    );
    if (inner == NULL) {
        return labssh2_status_from_result(libssh2_sftp_last_error(sftp->inner));
    }
    labssh2_sftp_directory_t* directory = malloc(sizeof(labssh2_sftp_directory_t));
    if (directory == NULL) {
        libssh2_sftp_close_handle(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    directory->inner = inner;
    *handle = directory;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_close_directory(
    labssh2_sftp_directory_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_close_handle(handle->inner);
    if (result != 0) {
        return labssh2_status_from_result(result);
    }
    handle->inner = NULL;
    free(handle);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_read_file(
    labssh2_sftp_file_t* handle,
    uint8_t* buffer,
    size_t buffer_max_length,
    ssize_t* read_count
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_read(handle->inner, buffer, buffer_max_length);
    if (count < 0) {
        return labssh2_status_from_result(count);
    }
    *read_count = count;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_read_directory(
    labssh2_sftp_directory_t* handle,
    uint8_t* buffer,
    size_t buffer_max_length,
    labssh2_sftp_attributes_t* attributes,
    ssize_t* read_count
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_readdir_ex(
        handle->inner,
        buffer,
        buffer_max_length,
        NULL,
        0,
        attributes->inner
    );
    if (count < 0) {
        return labssh2_status_from_result(count);
    }
    *read_count = count;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_write_file(
    labssh2_sftp_file_t* handle,
    uint8_t* buffer,
    size_t buffer_length,
    ssize_t* write_count
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (buffer == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    ssize_t count = libssh2_sftp_write(
        handle->inner,
        buffer,
        buffer_length
    );
    if (count < 0) {
        return labssh2_status_from_result(count);
    }
    *write_count = count;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_file_sync(
    labssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fsync(handle->inner);
    return labssh2_status_from_result(result);
}

labssh2_status_t
labssh2_sftp_file_seek(
    labssh2_sftp_file_t* handle,
    size_t offset
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_sftp_seek64(handle->inner, offset);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_file_rewind(
    labssh2_sftp_file_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_sftp_seek64(handle->inner, 0);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_file_position(
    labssh2_sftp_file_t* handle,
    uint64_t* position
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    uint64_t pos = libssh2_sftp_tell64(handle->inner);
    *position = pos;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_file_status(
    labssh2_sftp_file_t* handle,
    labssh2_sftp_attributes_t* attributes
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fstat_ex(handle->inner, attributes->inner, 0);
    if (result != 0) {
        return labssh2_status_from_result(result);
    }
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_sftp_file_set_status(
    labssh2_sftp_file_t* handle,
    labssh2_sftp_attributes_t* attributes
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_sftp_fstat_ex(handle->inner, attributes->inner, 1);
    if (result != 0) {
        return labssh2_status_from_result(result);
    }
    return LABSSH2_STATUS_OK;
}

/* labssh2_status_t */
/* labssh2_sftp_link_status( */
/*     labssh2_sftp_file_t* handle, */
/*     labssh2_sftp_attributes_t* attributes */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_file_rename( */
/*     labssh2_sftp_file_t* handle, */
/*     const char* source_path, */
/*     const char* destination_path */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_unlink( */
/*     labssh2_sftp_t* handle, */
/*     const char* file_path */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_create_directory( */
/*     labssh2_sftp_t* handle, */
/*     const char* directory_path, */
/*     int32_t mode */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_remove_directory( */
/*     labssh2_sftp_t* handle, */
/*     const char* directory_path */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_create_link( */
/*     labssh2_sftp_t* handle, */
/*     const char* source_path, */
/*     const char* link_path */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_resolve_symbolic_link( */
/*     labssh2_sftp_t* handle, */
/*     const char* link_path, */
/*     char* source_path, */
/*     uint32_t source_path_max_length */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_resolve_real_link( */
/*     labssh2_sftp_t* handle, */
/*     const char* link_path, */
/*     char* source_path, */
/*     uint32_t source_path_max_length */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_attributes_create( */
/*     labssh2_sftp_attributes_t** handle */
/* ); */

/* labssh2_status_t */
/* labssh2_sftp_attributes_destroy( */
/*     labssh2_sftp_attributes_t* handle */
/* ); */
