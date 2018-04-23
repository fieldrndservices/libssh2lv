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

#include "libssh2.h"

#include "labssh2.h"

static const char* UNKNOWN_STATUS = "Unknown status";

bool
labssh2_status_is_ok(
    labssh2_status_t status
) {
    return status >= LABSSH2_STATUS_OK;
}

bool
labssh2_status_is_err(
    labssh2_status_t status
) {
    return status < LABSSH2_STATUS_OK;
}

const char*
labssh2_status_string(
    labssh2_status_t status
) {
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
        case LABSSH2_STATUS_ERROR_BANNER_RECV: return "Banner Receive Error";
        case LABSSH2_STATUS_ERROR_BANNER_SEND: return "Banner Send Error";
        case LABSSH2_STATUS_ERROR_INVALID_MAC: return "Invalid MAC Error";
        case LABSSH2_STATUS_ERROR_KEX_FAILURE: return "Key Exchange Failure Error";
        case LABSSH2_STATUS_ERROR_SOCKET_SEND: return "Socket Send Error";
        case LABSSH2_STATUS_ERROR_KEY_EXCHANGE_FAILURE: return "Key Exchange Failure Error";
        case LABSSH2_STATUS_ERROR_TIMEOUT: return "Timeout Error";
        case LABSSH2_STATUS_ERROR_HOST_KEY_INITIALIZE: return "Host Key Iinitialize Error";
        case LABSSH2_STATUS_ERROR_HOST_KEY_SIGNATURE: return "Host Key Signature Error";
        case LABSSH2_STATUS_ERROR_DECRYPTION: return "Decryption Error";
        case LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT: return "Socket Disconnect Error";
        case LABSSH2_STATUS_ERROR_PROTOCOL: return "Protocol Error";
        case LABSSH2_STATUS_ERROR_PASSWORD_EXPIRED: return "Password Expired Error";
        case LABSSH2_STATUS_ERROR_FILE: return "File Error";
        case LABSSH2_STATUS_ERROR_NONE: return "None Error";
        case LABSSH2_STATUS_ERROR_AUTHENTICATION: return "Authentication Error";
        case LABSSH2_STATUS_ERROR_PUBLIC_KEY_UNVERIFIED: return "Key Unverified Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_OUT_OF_ORDER: return "Channel Out of Order Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_FAILURE: return "Channel Failure Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_REQUEST_DENIED: return "Channel Request Denied Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_UNKNOWN: return "Channel Unknown Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_WINDOW_EXCEEDED: return "Channel Window Exceeded Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_PACKET_EXCEEDED: return "Channel Packet Exceeded Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_CLOSED: return "Channel Closed Error";
        case LABSSH2_STATUS_ERROR_CHANNEL_EOF_SENT: return "Channel EOF Sent Error";
        case LABSSH2_STATUS_ERROR_SCP_PROTOCOL: return "SCP Protocol Error";
        case LABSSH2_STATUS_ERROR_ZLIB: return "zlib Error";
        case LABSSH2_STATUS_ERROR_SOCKET_TIMEOUT: return "Socket Timeout Error";
        case LABSSH2_STATUS_ERROR_SFTP_PROTOCOL: return "SFTP Protocol Error";
        case LABSSH2_STATUS_ERROR_REQUEST_DENIED: return "Request Denied Error";
        case LABSSH2_STATUS_ERROR_METHOD_NOT_SUPPORTED: return "Method Not Supported Error";
        case LABSSH2_STATUS_ERROR_INVALID: return "Invalid Error";
        case LABSSH2_STATUS_ERROR_INVALID_POLL_TYPE: return "Invalid Poll Type Error";
        case LABSSH2_STATUS_ERROR_PUBLIC_KEY_PROTOCOL: return "Public Key Protocol Error";
        case LABSSH2_STATUS_ERROR_EXECUTE_AGAIN: return "Execute Again Error";
        case LABSSH2_STATUS_ERROR_BUFFER_TOO_SMALL: return "Buffer Too Small Error";
        case LABSSH2_STATUS_ERROR_BAD_USE: return "Bad Use Error";
        case LABSSH2_STATUS_ERROR_COMMPRESS: return "Compress Error";
        case LABSSH2_STATUS_ERROR_OUT_OF_BOUNDARY: return "Out of Boundary Error";
        case LABSSH2_STATUS_ERROR_AGENT_PROTOCOL: return "Agent Protocol Error";
        case LABSSH2_STATUS_ERROR_SOCKET_RECV: return "Socket Receive Error";
        case LABSSH2_STATUS_ERROR_ENCRYPTION: return "Encryption Error";
        case LABSSH2_STATUS_ERROR_BAD_SOCKET: return "Bad Socket Error";
        case LABSSH2_STATUS_ERROR_KNOWN_HOSTS: return "Known Hosts Error"; 
        case LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM: return "Unknown Hash Algorithm Error";
        case LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE: return "Hash Unavailable Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE: return "Unknown Host Name Type Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING: return "Unknwon Key Encoding Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM: return "Unknown Key Algorithm Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_MODE: return "Unknown Mode Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_BLOCK_DIRECTION: return "Unknown Block Direction Error";
        case LABSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION: return "Unknown Session Option Error";
        case LABSSH2_STATUS_ERROR_SESSION_NOT_STARTED: return "Session Not Started Error";
        default: return UNKNOWN_STATUS;
    }
}

const char*
labssh2_status_message(
    labssh2_status_t status
) {
    switch (status) {
        case LABSSH2_STATUS_MISMATCH: return "A host was found, but the keys did not match.";
        case LABSSH2_STATUS_MATCH: return "The hosts and keys match.";
        case LABSSH2_STATUS_NOT_FOUND: return "No match for the host was found.";
        case LABSSH2_STATUS_END_OF_HOSTS: return "There are no more hosts.";
        case LABSSH2_STATUS_OK: return "The function completed successfully.";
        case LABSSH2_STATUS_ERROR_GENERIC: return "An unknown or generic error has occurred, good luck.";
        case LABSSH2_STATUS_ERROR_MALLOC: return "Unable to allocate memory, most likely the system is out of memory.";
        case LABSSH2_STATUS_ERROR_FREE: return "Unable to deallocate, or free, memory.";
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return "The function argument/parameter cannot be NULL.";
        case LABSSH2_STATUS_ERROR_SOCKET_NONE: return "The socket is invalid.";
        case LABSSH2_STATUS_ERROR_BANNER_RECV: return "";
        case LABSSH2_STATUS_ERROR_BANNER_SEND: return "Unable to send banner to remote host.";
        case LABSSH2_STATUS_ERROR_INVALID_MAC: return "";
        case LABSSH2_STATUS_ERROR_KEX_FAILURE: return "The encryption key exchange with the remote host has failed.";
        case LABSSH2_STATUS_ERROR_SOCKET_SEND: return "Unable to send data on socket.";
        case LABSSH2_STATUS_ERROR_KEY_EXCHANGE_FAILURE: return "";
        case LABSSH2_STATUS_ERROR_TIMEOUT: return "";
        case LABSSH2_STATUS_ERROR_HOST_KEY_INITIALIZE: return "";
        case LABSSH2_STATUS_ERROR_HOST_KEY_SIGNATURE: return "";
        case LABSSH2_STATUS_ERROR_DECRYPTION: return "";
        case LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT: return "The socket was disconnected.";
        case LABSSH2_STATUS_ERROR_PROTOCOL: return "An invalid SSH protocol response was received on the socket.";
        case LABSSH2_STATUS_ERROR_PASSWORD_EXPIRED: return "";
        case LABSSH2_STATUS_ERROR_FILE: return "";
        case LABSSH2_STATUS_ERROR_NONE: return "";
        case LABSSH2_STATUS_ERROR_AUTHENTICATION: return "";
        case LABSSH2_STATUS_ERROR_PUBLIC_KEY_UNVERIFIED: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_OUT_OF_ORDER: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_FAILURE: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_REQUEST_DENIED: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_UNKNOWN: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_WINDOW_EXCEEDED: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_PACKET_EXCEEDED: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_CLOSED: return "";
        case LABSSH2_STATUS_ERROR_CHANNEL_EOF_SENT: return "";
        case LABSSH2_STATUS_ERROR_SCP_PROTOCOL: return "";
        case LABSSH2_STATUS_ERROR_ZLIB: return "";
        case LABSSH2_STATUS_ERROR_SOCKET_TIMEOUT: return "";
        case LABSSH2_STATUS_ERROR_SFTP_PROTOCOL: return "";
        case LABSSH2_STATUS_ERROR_REQUEST_DENIED: return "";
        case LABSSH2_STATUS_ERROR_METHOD_NOT_SUPPORTED: return "";
        case LABSSH2_STATUS_ERROR_INVALID: return "";
        case LABSSH2_STATUS_ERROR_INVALID_POLL_TYPE: return "";
        case LABSSH2_STATUS_ERROR_PUBLIC_KEY_PROTOCOL: return "";
        case LABSSH2_STATUS_ERROR_EXECUTE_AGAIN: return "The operation has been marked for non-blocking I/O but the call would block.";
        case LABSSH2_STATUS_ERROR_BUFFER_TOO_SMALL: return "The buffer has been deemed too small to fit the data. You are advised to call the function again with a larger buffer.";
        case LABSSH2_STATUS_ERROR_BAD_USE: return "";
        case LABSSH2_STATUS_ERROR_COMMPRESS: return "";
        case LABSSH2_STATUS_ERROR_OUT_OF_BOUNDARY: return "";
        case LABSSH2_STATUS_ERROR_AGENT_PROTOCOL: return "";
        case LABSSH2_STATUS_ERROR_SOCKET_RECV: return "";
        case LABSSH2_STATUS_ERROR_ENCRYPTION: return "";
        case LABSSH2_STATUS_ERROR_BAD_SOCKET: return "";
        case LABSSH2_STATUS_ERROR_KNOWN_HOSTS: return ""; 
case LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM: return "Only the MD5 and SHA1 algorithms are supported.";
        case LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE: return "The session has not been started, or the requested hash algorithm is not available.";
        case LABSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE: return "The host name type is unknown.";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING: return "The key encoding is unknown.";
        case LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM: return "The key algorithm is unknown.";
        case LABSSH2_STATUS_ERROR_UNKNOWN_MODE: return "The session mode is unknown.";
        case LABSSH2_STATUS_ERROR_UNKNOWN_BLOCK_DIRECTION: return "The block direction is unknown";
        case LABSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION: return "The session option is unknown";
        case LABSSH2_STATUS_ERROR_SESSION_NOT_STARTED: return "The session has not yet been started";
        default: return UNKNOWN_STATUS;
    }
}

labssh2_status_t 
labssh2_status_from_result(
    int libssh2_result
) {
    if (libssh2_result >= 0) {
        return LABSSH2_STATUS_OK;
    }
    switch (libssh2_result) {
        case LIBSSH2_ERROR_SOCKET_NONE: return LABSSH2_STATUS_ERROR_SOCKET_NONE;
        case LIBSSH2_ERROR_BANNER_RECV: return LABSSH2_STATUS_ERROR_BANNER_RECV;
        case LIBSSH2_ERROR_BANNER_SEND: return LABSSH2_STATUS_ERROR_BANNER_SEND;
        case LIBSSH2_ERROR_INVALID_MAC: return LABSSH2_STATUS_ERROR_INVALID_MAC;
        case LIBSSH2_ERROR_KEX_FAILURE: return LABSSH2_STATUS_ERROR_KEX_FAILURE;
        case LIBSSH2_ERROR_ALLOC: return LABSSH2_STATUS_ERROR_MALLOC;
        case LIBSSH2_ERROR_SOCKET_SEND: return LABSSH2_STATUS_ERROR_SOCKET_SEND;
        case LIBSSH2_ERROR_KEY_EXCHANGE_FAILURE: return LABSSH2_STATUS_ERROR_KEY_EXCHANGE_FAILURE;
        case LIBSSH2_ERROR_TIMEOUT: return LABSSH2_STATUS_ERROR_TIMEOUT;
        case LIBSSH2_ERROR_HOSTKEY_INIT: return LABSSH2_STATUS_ERROR_HOST_KEY_INITIALIZE;
        case LIBSSH2_ERROR_HOSTKEY_SIGN: return LABSSH2_STATUS_ERROR_HOST_KEY_SIGNATURE;
        case LIBSSH2_ERROR_DECRYPT: return LABSSH2_STATUS_ERROR_DECRYPTION;
        case LIBSSH2_ERROR_SOCKET_DISCONNECT: return LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT;
        case LIBSSH2_ERROR_PROTO: return LABSSH2_STATUS_ERROR_PROTOCOL;
        case LIBSSH2_ERROR_PASSWORD_EXPIRED: return LABSSH2_STATUS_ERROR_PASSWORD_EXPIRED;
        case LIBSSH2_ERROR_FILE: return LABSSH2_STATUS_ERROR_FILE;
        case LIBSSH2_ERROR_METHOD_NONE: return LABSSH2_STATUS_ERROR_NONE;
        case LIBSSH2_ERROR_AUTHENTICATION_FAILED: return LABSSH2_STATUS_ERROR_AUTHENTICATION;
        case LIBSSH2_ERROR_PUBLICKEY_UNVERIFIED: return LABSSH2_STATUS_ERROR_PUBLIC_KEY_UNVERIFIED;
        case LIBSSH2_ERROR_CHANNEL_OUTOFORDER: return LABSSH2_STATUS_ERROR_CHANNEL_OUT_OF_ORDER;
        case LIBSSH2_ERROR_CHANNEL_FAILURE: return LABSSH2_STATUS_ERROR_CHANNEL_FAILURE;
        case LIBSSH2_ERROR_CHANNEL_REQUEST_DENIED: return LABSSH2_STATUS_ERROR_CHANNEL_REQUEST_DENIED;
        case LIBSSH2_ERROR_CHANNEL_UNKNOWN: return LABSSH2_STATUS_ERROR_CHANNEL_UNKNOWN;
        case LIBSSH2_ERROR_CHANNEL_WINDOW_EXCEEDED: return LABSSH2_STATUS_ERROR_CHANNEL_WINDOW_EXCEEDED;
        case LIBSSH2_ERROR_CHANNEL_PACKET_EXCEEDED: return LABSSH2_STATUS_ERROR_CHANNEL_PACKET_EXCEEDED;
        case LIBSSH2_ERROR_CHANNEL_CLOSED: return LABSSH2_STATUS_ERROR_CHANNEL_CLOSED;
        case LIBSSH2_ERROR_CHANNEL_EOF_SENT: return LABSSH2_STATUS_ERROR_CHANNEL_EOF_SENT;
        case LIBSSH2_ERROR_SCP_PROTOCOL: return LABSSH2_STATUS_ERROR_SCP_PROTOCOL;
        case LIBSSH2_ERROR_ZLIB: return LABSSH2_STATUS_ERROR_ZLIB;
        case LIBSSH2_ERROR_SOCKET_TIMEOUT: return LABSSH2_STATUS_ERROR_SOCKET_TIMEOUT;
        case LIBSSH2_ERROR_SFTP_PROTOCOL: return LABSSH2_STATUS_ERROR_SFTP_PROTOCOL;
        case LIBSSH2_ERROR_REQUEST_DENIED: return LABSSH2_STATUS_ERROR_REQUEST_DENIED;
        case LIBSSH2_ERROR_METHOD_NOT_SUPPORTED: return LABSSH2_STATUS_ERROR_METHOD_NOT_SUPPORTED;
        case LIBSSH2_ERROR_INVAL: return LABSSH2_STATUS_ERROR_INVALID;
        case LIBSSH2_ERROR_INVALID_POLL_TYPE: return LABSSH2_STATUS_ERROR_INVALID_POLL_TYPE;
        case LIBSSH2_ERROR_PUBLICKEY_PROTOCOL: return LABSSH2_STATUS_ERROR_PUBLIC_KEY_PROTOCOL;
        case LIBSSH2_ERROR_EAGAIN: return LABSSH2_STATUS_ERROR_EXECUTE_AGAIN;
        case LIBSSH2_ERROR_BUFFER_TOO_SMALL: return LABSSH2_STATUS_ERROR_BUFFER_TOO_SMALL;
        case LIBSSH2_ERROR_BAD_USE: return LABSSH2_STATUS_ERROR_BAD_USE;
        case LIBSSH2_ERROR_COMPRESS: return LABSSH2_STATUS_ERROR_COMMPRESS;
        case LIBSSH2_ERROR_OUT_OF_BOUNDARY: return LABSSH2_STATUS_ERROR_OUT_OF_BOUNDARY;
        case LIBSSH2_ERROR_AGENT_PROTOCOL: return LABSSH2_STATUS_ERROR_AGENT_PROTOCOL;
        case LIBSSH2_ERROR_SOCKET_RECV: return LABSSH2_STATUS_ERROR_SOCKET_RECV;
        case LIBSSH2_ERROR_ENCRYPT: return LABSSH2_STATUS_ERROR_ENCRYPTION;
        case LIBSSH2_ERROR_BAD_SOCKET: return LABSSH2_STATUS_ERROR_BAD_SOCKET;
        case LIBSSH2_ERROR_KNOWN_HOSTS: return LABSSH2_STATUS_ERROR_KNOWN_HOSTS; 
        default: return LABSSH2_STATUS_ERROR_GENERIC;
    }
}

