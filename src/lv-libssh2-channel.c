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
#include "lv_libssh2-listener-private.h"
#include "lv_libssh2-channel-private.h"

lv_libssh2_status_t
lv_libssh2_channel_create(
    lv_libssh2_session_t* session,
    lv_libssh2_channel_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_CHANNEL* inner = libssh2_channel_open_ex(
        session->inner,
        "session",
        sizeof("session") - 1,
        LIBSSH2_CHANNEL_WINDOW_DEFAULT,
        LIBSSH2_CHANNEL_PACKET_DEFAULT,
        NULL,
        0
    );
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_session_last_errno(session->inner));
    }
    lv_libssh2_channel_t* channel = malloc(sizeof(lv_libssh2_channel_t));
    if (channel == NULL) {
        libssh2_channel_free(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    channel->inner = inner;
    *handle = channel;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_destroy(
    lv_libssh2_channel_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    libssh2_channel_free(handle->inner);
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_close(
    lv_libssh2_channel_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_close(handle->inner);
    if (result != 0) {
        return lv_libssh2_status_from_result(result);
    }
    handle->inner = NULL;
    free(handle);
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_read(
    lv_libssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_read_ex(handle->inner, 0, buffer, buffer_len);
    if (result < 0) {
        return lv_libssh2_status_from_result(result);
    }
    *byte_count = result;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_read_stderr(
    lv_libssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_read_ex(handle->inner, SSH_EXTENDED_DATA_STDERR, buffer, buffer_len);
    if (result < 0) {
        return lv_libssh2_status_from_result(result);
    }
    *byte_count = result;
    return LV_LIBSSH2_STATUS_OK;
}


lv_libssh2_status_t
lv_libssh2_channel_direct_tcpip(
    lv_libssh2_session_t* session,
    const char* host,
    int port,
    const char* server_host,
    int server_port,
    lv_libssh2_channel_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (host == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (server_host == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_CHANNEL* inner = libssh2_channel_direct_tcpip_ex(session->inner, host, port, server_host, server_port);
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_session_errno(session->inner));
    }
    lv_libssh2_channel_t* channel = malloc(sizeof(lv_libssh2_channel_t));
    if (channel == NULL) {
        libssh2_channel_free(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    channel->inner = inner;
    *handle = channel;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_eof(
    lv_libssh2_channel_t* handle,
    int* eof
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_eof(handle->inner);
    if (result < 0) {
        return lv_libssh2_status_from_result(result);
    }
    *eof = result;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_flush(
    lv_libssh2_channel_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_flush_ex(handle->inner, 0);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_channel_forward_accept(
    lv_libssh2_session_t* session,
    lv_libssh2_listener_t* listener,
    lv_libssh2_channel_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (listener == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_CHANNEL* inner = libssh2_channel_forward_accept(listener->inner);
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_session_last_errno(session->inner));
    }
    lv_libssh2_channel_t* channel = malloc(sizeof(lv_libssh2_channel_t));
    if (channel == NULL) {
        libssh2_channel_free(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    channel->inner = inner;
    *handle = channel;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_forward_cancel(
    lv_libssh2_listener_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_forward_cancel(handle->inner);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_channel_forward_listen(
    lv_libssh2_session_t* session,
    const int port,
    lv_libssh2_listener_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_LISTENER* inner = libssh2_channel_forward_listen_ex(session->inner, NULL, port, NULL, 16);
    if (inner == NULL) {
        return lv_libssh2_status_from_result(libssh2_session_last_errno(session->inner));
    }
    lv_libssh2_listener_t* listener = malloc(sizeof(lv_libssh2_listener_t));
    if (listener == NULL) {
        libssh2_channel_forward_cancel(inner);
        return LV_LIBSSH2_STATUS_ERROR_MALLOC;
    }
    listener->inner = inner;
    *handle = listener;
    return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_channel_exit_status(
    lv_libssh2_channel_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_get_exit_status(handle->inner);
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_channel_set_ignore_mode(
    lv_libssh2_channel_t* handle,
    lv_libssh2_ignore_modes_t mode
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    switch (mode) {
        case LV_LIBSSH2_IGNORE_MODES_NORMAL:
            int result = libssh2_channel_handle_extended_data2(handle->inner, LIBSSH2_CHANNEL_EXTENDED_DATA_NORMAL);
            return lv_libssh2_status_from_result(result);
        case LV_LIBSSH2_IGNORE_MODES_MERGE:
            int result = libssh2_channel_handle_extended_data2(handle->inner, LIBSSH2_CHANNEL_EXTENDED_DATA_MERGE);
            return lv_libssh2_status_from_result(result);
        case LV_LIBSSH2_IGNORE_MODES_IGNORE:
            int result = libssh2_channel_handle_extended_data2(handle->inner, LIBSSH2_CHANNEL_EXTENDED_DATA_IGNORE);
            return lv_libssh2_status_from_result(result);
        default: return LV_LIBSSH2_STATUS_ERROR_UNKOWN_IGNORE_MODE;
    }
}

lv_libssh2_status_t
lv_libssh2_channel_shell(
    lv_libssh2_channel_t* handle
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_process_startup(
        handle->inner,
        "shell",
        sizeof("shell") - 1,
        NULL,
        0
    );
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_channel_exec(
    lv_libssh2_channel_t* handle,
    const char* command,
    const size_t command_len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_process_startup(
        handle->inner,
        "exec",
        sizeof("exec") - 1,
        command,
        command_len
    );
    return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_channel_subsystem(
    lv_libssh2_channel_t* handle,
    const char* subsystem,
    const size_t subsystem_len
) {
    if (handle == NULL) {
        return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
    }
    int result = libssh2_channel_process_startup(
        handle->inner,
        "subsystem",
        sizeof("subsystem") - 1,
        subsystem,
        subsystem_len
    );
    return lv_libssh2_status_from_result(result);
}