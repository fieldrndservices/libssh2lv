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
labssh2_userauth_list_len(
    labssh2_session_t* handle, 
    const char* username,
    size_t username_len,
    size_t* len
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (username == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* list = libssh2_userauth_list(handle->inner, username, username_len);
    if (list == NULL) {
        return labssh2_status_from_result(libssh2_session_last_errno(handle->inner));
    }
    *len = strlen(list);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_userauth_list(
    labssh2_session_t* handle, 
    const char* username,
    size_t username_len,
    uint8_t* buffer
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (username == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    const char* list = libssh2_userauth_list(handle->inner, username, username_len);
    if (list == NULL) {
        return labssh2_status_from_result(libssh2_session_last_errno(handle->inner));
    }
    memcpy(buffer, list, strlen(list));
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_userauth_authenticated(
    labssh2_session_t* handle, 
    int* authenticated
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *authenticated = libssh2_userauth_authenticated(handle->inner);
    return LABSSH2_STATUS_OK;
}

