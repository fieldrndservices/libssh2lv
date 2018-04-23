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
#include "labssh2-channel-private.h"
#include "labssh2-fileinfo-private.h"

labssh2_status_t
labssh2_scp_send(
    labssh2_session_t* session, 
    const char* path,
    int mode,
    size_t file_size,
    labssh2_channel_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_CHANNEL* inner = libssh2_scp_send64(session->inner, path, mode, file_size, 0, 0);
    if (inner == NULL) {
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    labssh2_channel_t* channel = malloc(sizeof(labssh2_channel_t));
    if (channel == NULL) {
        libssh2_channel_free(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    channel->inner = inner;
    *handle = channel;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_scp_receive(
    labssh2_session_t* session, 
    const char* path,
    labssh2_fileinfo_t* file_info,
    labssh2_channel_t** handle
) {
    *handle = NULL;
    if (session == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (path == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (file_info == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    LIBSSH2_CHANNEL* inner = libssh2_scp_recv2(session->inner, path, file_info->inner);
    if (inner == NULL) {
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    labssh2_channel_t* channel = malloc(sizeof(labssh2_channel_t));
    if (channel == NULL) {
        libssh2_channel_free(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    channel->inner = inner;
    *handle = channel;
    return LABSSH2_STATUS_OK;
}
