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
#include "lv_libssh2-session-private.h"

#define BLOCK_DIRECTIONS_BOTH 3

lv_libssh2_status_t
lv_libssh2_session_create(
    lv_libssh2_session_t** handle
) {
    *handle = NULL;
    LIBSSH2_SESSION* inner = libssh2_session_init_ex(NULL, NULL, NULL, NULL);
    if (inner == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    lv_libssh2_session_t* session = malloc(sizeof(lv_libssh2_session_t));
    if (session == NULL) {
        libssh2_session_free(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    session->inner = inner;
    *handle = session;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_destroy(
    lv_libssh2_session_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_set_blocking(handle->inner, LV_LIBSSH2_SESSION_MODE_BLOCKING);
    int result = libssh2_session_free(handle->inner);
    if (result != 0) {
        return LV_LIBSSH2_STATUS_ERROR_FREE;
    }
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_connect(
    lv_libssh2_session_t* handle,
    const uintptr_t socket
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_handshake(handle->inner, socket);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_session_disconnect(
    lv_libssh2_session_t* handle,
    const char* description
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_set_blocking(handle->inner, LV_LIBSSH2_SESSION_MODE_BLOCKING);
    libssh2_session_disconnect_ex(handle->inner, SSH_DISCONNECT_BY_APPLICATION, description, "");
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_hostkey_hash_len(
    lv_libssh2_hostkey_hash_types_t type,
    size_t* len
) {
    switch (type) {
        case LV_LIBSSH2_HOSTKEY_HASH_TYPE_MD5: *len = 16; break;
        case LV_LIBSSH2_HOSTKEY_HASH_TYPE_SHA1: *len = 20; break;
        default: return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM;
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_hostkey_hash(
    lv_libssh2_session_t* handle,
    const lv_libssh2_hostkey_hash_types_t type,
    uint8_t* buffer
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* hash = libssh2_hostkey_hash(handle->inner, type);
    if (hash == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_HASH_UNAVAILABLE;
    }
    size_t len = 0;
    const lv_libssh2_status_t result = lv_libssh2_session_hostkey_hash_len(type, &len);
    if (lv_libssh2_status_is_err(result)) {
        return result;
    }
    memcpy(buffer, hash, len);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_hostkey_len(
    lv_libssh2_session_t* handle,
    size_t* len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int type = 0;
    const char* result = libssh2_session_hostkey(handle->inner, len, &type);
    if (result == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_GENERIC;
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_hostkey(
    lv_libssh2_session_t* handle,
    uint8_t* buffer,
    lv_libssh2_hostkey_types_t* type
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    size_t len = 0;
    int libssh2_type = 0;
    const char* hostkey = libssh2_session_hostkey(handle->inner, &len, &libssh2_type);
    if (hostkey == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_GENERIC;
    }
    switch (libssh2_type) {
        case LIBSSH2_HOSTKEY_TYPE_RSA: *type = LV_LIBSSH2_HOSTKEY_TYPE_RSA; break;
        case LIBSSH2_HOSTKEY_TYPE_DSS: *type = LV_LIBSSH2_HOSTKEY_TYPE_DSS; break;
        default: *type = LV_LIBSSH2_HOSTKEY_TYPE_UNKNOWN;
    }
    memcpy(buffer, hostkey, len);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_mode(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_modes_t* mode
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_get_blocking(handle->inner);
    switch (result) {
        case 0: *mode = LV_LIBSSH2_SESSION_MODE_NONBLOCKING; break;
        case 1: *mode = LV_LIBSSH2_SESSION_MODE_BLOCKING; break;
        default: return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_MODE;
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_set_mode(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_modes_t mode
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    switch (mode) {
        case LV_LIBSSH2_SESSION_MODE_NONBLOCKING: libssh2_session_set_blocking(handle->inner, 0); break;
        case LV_LIBSSH2_SESSION_MODE_BLOCKING: libssh2_session_set_blocking(handle->inner, 1); break;
        default: return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_MODE;
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_banner_len(
    lv_libssh2_session_t* handle,
    size_t* len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* result = libssh2_session_banner_get(handle->inner);
    if (result == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_GENERIC;
    }
    *len = strlen(result);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_banner(
    lv_libssh2_session_t* handle,
    char* buffer
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* banner = libssh2_session_banner_get(handle->inner);
    if (banner == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_GENERIC;
    }
    memcpy(buffer, banner, strlen(banner));
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_set_banner(
    lv_libssh2_session_t* handle,
    const char* banner
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (banner == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_banner_set(handle->inner, banner);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_session_block_directions(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_block_directions_t* directions
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_block_directions(handle->inner);
    switch (result) {
        case BLOCK_DIRECTIONS_BOTH:
            *directions = LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_BOTH;
            break;
        case LIBSSH2_SESSION_BLOCK_INBOUND:
            *directions = LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_READ;
            break;
        case LIBSSH2_SESSION_BLOCK_OUTBOUND:
            *directions = LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_WRITE;
            break;
        default:
            return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_BLOCK_DIRECTION;
    }
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_enable_option(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_options_t option
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = 0;
    switch (option) {
        case LV_LIBSSH2_SESSION_OPTIONS_SIGPIPE:
            result = libssh2_session_flag(handle->inner, LIBSSH2_FLAG_SIGPIPE, 1);
            return lv_libssh2_status_from_result(result);
        case LV_LIBSSH2_SESSION_OPTIONS_COMPRESS:
            result = libssh2_session_flag(handle->inner, LIBSSH2_FLAG_COMPRESS, 1);
            return lv_libssh2_status_from_result(result);
        default:
            return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION;
    }
}

lv_libssh2_status_t
lv_libssh2_session_disable_option(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_options_t option
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = 0;
    switch (option) {
        case LV_LIBSSH2_SESSION_OPTIONS_SIGPIPE:
            result = libssh2_session_flag(handle->inner, LIBSSH2_FLAG_SIGPIPE, 0);
            return lv_libssh2_status_from_result(result);
        case LV_LIBSSH2_SESSION_OPTIONS_COMPRESS:
            result = libssh2_session_flag(handle->inner, LIBSSH2_FLAG_COMPRESS, 0);
            return lv_libssh2_status_from_result(result);
        default:
            return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION;
    }
}

lv_libssh2_status_t
lv_libssh2_session_timeout(
    lv_libssh2_session_t* handle,
    long* milliseconds
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *milliseconds = libssh2_session_get_timeout(handle->inner);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_set_timeout(
    lv_libssh2_session_t* handle,
    const long milliseconds
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_set_timeout(handle->inner, milliseconds);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_last_error_code(
    lv_libssh2_session_t* handle,
    int* code
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *code = libssh2_session_last_errno(handle->inner);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_last_error_len(
    lv_libssh2_session_t* handle,
    int32_t* len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_last_error(handle->inner, NULL, len, 0);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_last_error(
    lv_libssh2_session_t* handle,
    char* buffer
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_session_last_error(handle->inner, &buffer, NULL, 1);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_set_last_error(
    lv_libssh2_session_t* handle,
    int code,
    const char* message
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_set_last_error(handle->inner, code, message);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_session_set_method_pref(
    lv_libssh2_session_t* handle,
    lv_libssh2_methods_t method,
    const char* prefs
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (prefs == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_session_method_pref(handle->inner, method, prefs);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_session_method_len(
    lv_libssh2_session_t* handle,
    lv_libssh2_methods_t method,
    size_t* len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* actual = libssh2_session_methods(handle->inner, method);
    if (actual == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_SESSION_NOT_STARTED;
    }
    *len = strlen(actual);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_session_method(
    lv_libssh2_session_t* handle,
    lv_libssh2_methods_t method,
    uint8_t* buffer
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* actual = libssh2_session_methods(handle->inner, method);
    if (actual == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_SESSION_NOT_STARTED;
    }
    memcpy(buffer, actual, strlen(actual));
    return LV_LIBSSH2_STATUS_OK;
}

