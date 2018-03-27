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

#include "labssh2.h"

static const char* UNKNOWN_STATUS = "Unknown status";

bool
labssh2_status_is_ok(labssh2_status_t status)
{
    return status >= LABSSH2_STATUS_OK;
}

bool
labssh2_status_is_err(labssh2_status_t status)
{
    return status < LABSSH2_STATUS_OK;
}

const char*
labssh2_status_string(labssh2_status_t status) {
    switch (status) {
        case LABSSH2_STATUS_MISMATCH: return "Mismatch";
        case LABSSH2_STATUS_MATCH: return "Match";
        case LABSSH2_STATUS_NOT_FOUND: return "Not Found";
        case LABSSH2_STATUS_END_OF_HOSTS: return "End of Hosts";
        case LABSSH2_STATUS_OK: return "No Error";
        case LABSSH2_STATUS_ERROR_GENERIC: return "Generic Error";
        case LABSSH2_STATUS_ERROR_MALLOC: return "Memory Allocation Error";
        case LABSSH2_STATUS_ERROR_FREE: return "Free Memory Error";
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return "Null Value Error";
        case LABSSH2_STATUS_ERROR_SOCKET_NONE: return "Socket None Error";
        case LABSSH2_STATUS_ERROR_BANNER_SEND: return "Banner Send Error";
        case LABSSH2_STATUS_ERROR_KEX_FAILURE: return "Key Exchange Error";
        case LABSSH2_STATUS_ERROR_SOCKET_SEND: return "Socket Send Error";
        case LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT: return "Socket Disconnect Error";
        case LABSSH2_STATUS_ERROR_PROTOCAL: return "Protocol Error";
        case LABSSH2_STATUS_ERROR_EXECUTE_AGAIN: return "Execute Again Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM: return "Unknown Hash Algorithm Error";
        case LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE: return "Hash Unavailable Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE: return "Unknown Host Name Type Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING: return "Unknwon Key Encoding Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM: return "Unknown Key Algorithm error";
        default: return UNKNOWN_STATUS;
    }
}

const char*
labssh2_status_message(
    labssh2_status_t status
) {
    switch (status) {
        case LABSSH2_STATUS_MISMATCH: return "Host was found, but the keys did not match";
        case LABSSH2_STATUS_MATCH: return "Hosts and keys match";
        case LABSSH2_STATUS_NOT_FOUND: return "No host match was found";
        case LABSSH2_STATUS_END_OF_HOSTS: return "There are no more hsots";
        case LABSSH2_STATUS_OK: return "The function completed successfully";
        case LABSSH2_STATUS_ERROR_GENERIC: return "An unknown or generic error has occurred, good luck";
        case LABSSH2_STATUS_ERROR_MALLOC: return "Unable to allocate memory, most likely out of memory";
        case LABSSH2_STATUS_ERROR_FREE: return "Unable to deallocate, or free, memory";
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return "The function argument/parameter cannot be NULL";
        case LABSSH2_STATUS_ERROR_SOCKET_NONE: return "The socket is invalid";
        case LABSSH2_STATUS_ERROR_BANNER_SEND: return "Unable to send banner to remote host";
        case LABSSH2_STATUS_ERROR_KEX_FAILURE: return "Encryption key exchange with remote host failed";
        case LABSSH2_STATUS_ERROR_SOCKET_SEND: return "Unable to send data on socket";
        case LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT: return "The socket was disconnected";
        case LABSSH2_STATUS_ERROR_PROTOCAL: return "An invalid SSH protocol response was received on the socket";
        case LABSSH2_STATUS_ERROR_EXECUTE_AGAIN: return "Marked for non-blocking I/O but the call would block";
        case LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM: return "Only the MD5 and SHA1 algorithms are supported";
        case LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE: return "The session has not been started, or the requested hash algorithm is not available";
        case LABSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE: return "Unknown host name type";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING: return "Unknown key encoding";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM: return "Unknown key algorithm";
        default: return UNKNOWN_STATUS;
    }
}

