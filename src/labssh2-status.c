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

#include <assert.h>

#include "libssh2.h"

#include "labssh2.h"

static const char* UNKNOWN_STATUS = "Unknown status";
static const char* UNKNOWN_LIBSSH2_ERROR = "Unknown libssh2 error code";

int
labssh2_status_code(labssh2_status_t status) {
    switch (status) {
        case LABSSH2_STATUS_OK: return 0;
        case LABSSH2_STATUS_ERROR_MEMORY: return -1;
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return -2;
        case LABSSH2_STATUS_ERROR_SESSION: return -3;
        case LABSSH2_STATUS_ERROR_KNOWNHOSTS: return -4;
        default: assert(UNKNOWN_STATUS);
    }
    return 1;
}

const char*
labssh2_status_string(labssh2_status_t status) {
    switch (status) {
        case LABSSH2_STATUS_OK: return "No Error";
        case LABSSH2_STATUS_ERROR_MEMORY: return "Out of Memory Error";
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return "Null Value Error";
        case LABSSH2_STATUS_ERROR_SESSION: return "Session Error";
        case LABSSH2_STATUS_ERROR_KNOWNHOSTS: return "Known Hosts Error";
        default: assert(UNKNOWN_STATUS);
    }
    return UNKNOWN_STATUS;
}

const char*
labssh2_status_error_to_message(int libssh2_error_code) 
{
    switch (libssh2_error_code) {
        case LIBSSH2_ERROR_NONE:
            return "Success";
        case LIBSSH2_ERROR_SOCKET_NONE:
            return "The socket is invalid";
        case LIBSSH2_ERROR_BANNER_RECV:
            return "Unable to receive banner from remote host";
        case LIBSSH2_ERROR_BANNER_SEND:
            return "Unable to send banner to remote host";
        case LIBSSH2_ERROR_KEX_FAILURE:
            return "Encryption key exchange with the remote host faield";
        case LIBSSH2_ERROR_SOCKET_SEND:
            return "Unable to send data on socket";
        case LIBSSH2_ERROR_SOCKET_DISCONNECT:
            return "The socket was disconnected";
        case LIBSSH2_ERROR_PROTO:
            return "An invalid SSH protocol response was received on the socket";
        case LIBSSH2_ERROR_EAGAIN:
            return "Marked for non-blocking I/O but the call would block";
        default:
            assert(UNKNOWN_LIBSSH2_ERROR);
    }
    return UNKNOWN_LIBSSH2_ERROR;
}

