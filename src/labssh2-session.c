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

#include "labssh2.h"
#include "labssh2-status-private.h"
#include "labssh2-session-private.h"

labssh2_status_t
labssh2_session_create(
    labssh2_session_t** handle
) {
    *handle = NULL;
    LIBSSH2_SESSION* inner = libssh2_session_init_ex(NULL, NULL, NULL, NULL);
    if (inner == NULL) {
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    labssh2_session_t* session = malloc(sizeof(labssh2_session_t));
    if (session == NULL) {
        libssh2_session_free(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    session->inner = inner;
    *handle = session;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_destroy(
    labssh2_session_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_set_blocking(handle->inner, LABSSH2_SESSION_MODE_BLOCKING);
    int result = libssh2_session_free(handle->inner);
    if (result != 0) {
        return LABSSH2_STATUS_ERROR_FREE;
    }
    handle->inner = NULL;
    free(handle);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_connect(
    labssh2_session_t* handle, 
    const uintptr_t socket
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_handshake(handle->inner, socket);
    return labssh2_status_from_result(result);
}

labssh2_status_t
labssh2_session_disconnect(
    labssh2_session_t* handle, 
    const char* description
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    } 
    libssh2_session_set_blocking(handle->inner, LABSSH2_SESSION_MODE_BLOCKING);
    libssh2_session_disconnect_ex(handle->inner, SSH_DISCONNECT_BY_APPLICATION, description, "");
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_hostkey_hash_len(
    labssh2_hostkey_hash_types_t type,
    size_t* len
) {
    switch (type) {
        case LABSSH2_HOSTKEY_HASH_TYPE_MD5: *len = 16; break;
        case LABSSH2_HOSTKEY_HASH_TYPE_SHA1: *len = 20; break;
        default: return LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM;
    }
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_hostkey_hash(
    labssh2_session_t* handle, 
    const labssh2_hostkey_hash_types_t type, 
    uint8_t* buffer
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* hash = libssh2_hostkey_hash(handle->inner, type);
    if (hash == NULL) {
        return LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE;
    }
    size_t len = 0;
    const labssh2_status_t result = labssh2_session_hostkey_hash_len(type, &len);
    if (labssh2_status_is_err(result)) {
        return result;
    }
    memcpy(buffer, hash, len);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_hostkey_len(
    labssh2_session_t* handle,
    size_t* len
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int type = 0;
    const char* result = libssh2_session_hostkey(handle->inner, len, &type);
    if (result == NULL) {
        return LABSSH2_STATUS_ERROR_GENERIC;
    }
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_session_hostkey(
    labssh2_session_t* handle, 
    uint8_t* buffer, 
    labssh2_hostkey_types_t* type
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    size_t len = 0;
    int libssh2_type = 0;
    const char* hostkey = libssh2_session_hostkey(handle->inner, &len, &libssh2_type);
    if (hostkey == NULL) {
        return LABSSH2_STATUS_ERROR_GENERIC;
    }
    switch (libssh2_type) {
        case LIBSSH2_HOSTKEY_TYPE_RSA: *type = LABSSH2_HOSTKEY_TYPE_RSA; break;
        case LIBSSH2_HOSTKEY_TYPE_DSS: *type = LABSSH2_HOSTKEY_TYPE_DSS; break;
        default: *type = LABSSH2_HOSTKEY_TYPE_UNKNOWN;
    }
    memcpy(buffer, hostkey, len);
    return LABSSH2_STATUS_OK;
}

