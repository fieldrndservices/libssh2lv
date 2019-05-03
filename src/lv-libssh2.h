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

/**
 * @file
 *
 * Includes the full LV-LIBSSH2 API.
 */

#ifndef LV_LIBSSH2_H
#define LV_LIBSSH2_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "libssh2.h"
#include "libssh2_sftp.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LV_LIBSSH2_API
#  ifdef _WIN32
#     if defined(LV_LIBSSH2_BUILD_SHARED) /* build dll */
#         define LV_LIBSSH2_API __declspec(dllexport)
#     elif !defined(LV_LIBSSH2_BUILD_STATIC) /* use dll */
#         define LV_LIBSSH2_API __declspec(dllimport)
#     else /* static library */
#         define LV_LIBSSH2_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define LV_LIBSSH2_API __attribute__((visibility("default")))
#     else
#         define LV_LIBSSH2_API
#     endif
#  endif
#endif

/**
 * Status
 */
typedef enum _lv_libssh2_status {
    LV_LIBSSH2_STATUS_OK = 0,

    LV_LIBSSH2_STATUS_ERROR_GENERIC = -1,
    LV_LIBSSH2_STATUS_ERROR_MALLOC = -2,
    LV_LIBSSH2_STATUS_ERROR_FREE = -3,
    LV_LIBSSH2_STATUS_ERROR_NULL_VALUE = -4,
    LV_LIBSSH2_STATUS_ERROR_SOCKET_NONE = -5,
    LV_LIBSSH2_STATUS_ERROR_BANNER_RECV = -6,
    LV_LIBSSH2_STATUS_ERROR_BANNER_SEND = -7,
    LV_LIBSSH2_STATUS_ERROR_INVALID_MAC = -8,
    LV_LIBSSH2_STATUS_ERROR_KEX_FAILURE = -9,
    LV_LIBSSH2_STATUS_ERROR_SOCKET_SEND = -10,
    LV_LIBSSH2_STATUS_ERROR_KEY_EXCHANGE_FAILURE = -11,
    LV_LIBSSH2_STATUS_ERROR_TIMEOUT = -12,
    LV_LIBSSH2_STATUS_ERROR_HOST_KEY_INITIALIZE = -13,
    LV_LIBSSH2_STATUS_ERROR_HOST_KEY_SIGNATURE = -14,
    LV_LIBSSH2_STATUS_ERROR_DECRYPTION = -15,
    LV_LIBSSH2_STATUS_ERROR_SOCKET_DISCONNECT = -16,
    LV_LIBSSH2_STATUS_ERROR_PROTOCOL = -17,
    LV_LIBSSH2_STATUS_ERROR_PASSWORD_EXPIRED = -18,
    LV_LIBSSH2_STATUS_ERROR_FILE = -19,
    LV_LIBSSH2_STATUS_ERROR_NONE = -20,
    LV_LIBSSH2_STATUS_ERROR_AUTHENTICATION = -21,
    LV_LIBSSH2_STATUS_ERROR_PUBLIC_KEY_UNVERIFIED = -22,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_OUT_OF_ORDER = -23,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_FAILURE = -24,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_REQUEST_DENIED = -25,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_UNKNOWN = -26,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_WINDOW_EXCEEDED = -27,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_PACKET_EXCEEDED = -28,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_CLOSED = -29,
    LV_LIBSSH2_STATUS_ERROR_CHANNEL_EOF_SENT = -30,
    LV_LIBSSH2_STATUS_ERROR_SCP_PROTOCOL = -31,
    LV_LIBSSH2_STATUS_ERROR_ZLIB = -32,
    LV_LIBSSH2_STATUS_ERROR_SOCKET_TIMEOUT = -33,
    LV_LIBSSH2_STATUS_ERROR_SFTP_PROTOCOL = -34,
    LV_LIBSSH2_STATUS_ERROR_REQUEST_DENIED = -35,
    LV_LIBSSH2_STATUS_ERROR_METHOD_NOT_SUPPORTED = -36,
    LV_LIBSSH2_STATUS_ERROR_INVALID = -37,
    LV_LIBSSH2_STATUS_ERROR_INVALID_POLL_TYPE = -38,
    LV_LIBSSH2_STATUS_ERROR_PUBLIC_KEY_PROTOCOL = -39,
    LV_LIBSSH2_STATUS_ERROR_EXECUTE_AGAIN = -40,
    LV_LIBSSH2_STATUS_ERROR_BUFFER_TOO_SMALL = -41,
    LV_LIBSSH2_STATUS_ERROR_BAD_USE = -42,
    LV_LIBSSH2_STATUS_ERROR_COMMPRESS = -43,
    LV_LIBSSH2_STATUS_ERROR_OUT_OF_BOUNDARY = -44,
    LV_LIBSSH2_STATUS_ERROR_AGENT_PROTOCOL = -45,
    LV_LIBSSH2_STATUS_ERROR_SOCKET_RECV = -46,
    LV_LIBSSH2_STATUS_ERROR_ENCRYPTION = -47,
    LV_LIBSSH2_STATUS_ERROR_BAD_SOCKET = -48,
    LV_LIBSSH2_STATUS_ERROR_KNOWN_HOSTS = -49,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM = -50,
    LV_LIBSSH2_STATUS_ERROR_HASH_UNAVAILABLE = -51,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE = -52,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING = -53,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM = -54,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_MODE = -55,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_BLOCK_DIRECTION = -56,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION = -57,
    LV_LIBSSH2_STATUS_ERROR_SESSION_NOT_STARTED = -58,
    LV_LIBSSH2_STATUS_ERROR_VERSION_TOO_OLD = -59,
    LV_LIBSSH2_STATUS_ERROR_UNKNOWN_IGNORE_MODE = -60
} lv_libssh2_status_t;

typedef enum _lv_libssh2_session_modes {
    LV_LIBSSH2_SESSION_MODE_NONBLOCKING = 0,
    LV_LIBSSH2_SESSION_MODE_BLOCKING = 1
} lv_libssh2_session_modes_t;

typedef enum _lv_libssh2_hostkey_hash_types {
    LV_LIBSSH2_HOSTKEY_HASH_TYPE_MD5 = 0,
    LV_LIBSSH2_HOSTKEY_HASH_TYPE_SHA1 = 1
} lv_libssh2_hostkey_hash_types_t;

typedef enum _lv_libssh2_hostkey_types {
    LV_LIBSSH2_HOSTKEY_TYPE_RSA = 0,
    LV_LIBSSH2_HOSTKEY_TYPE_DSS = 1,
    LV_LIBSSH2_HOSTKEY_TYPE_UNKNOWN = 2
} lv_libssh2_hostkey_types_t;

typedef enum _lv_libssh2_knownhost_name_types {
    LV_LIBSSH2_KNOWNHOST_NAME_TYPE_PLAIN = 0,
    LV_LIBSSH2_KNOWNHOST_NAME_TYPE_SHA1 = 1,
    LV_LIBSSH2_KNOWNHOST_NAME_TYPE_CUSTOM = 2,
} lv_libssh2_knownhost_name_types_t;

typedef enum _lv_libssh2_knownhost_key_encodings {
    LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_RAW = 0,
    LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_BASE64 = 1,
} lv_libssh2_knownhost_key_encodings_t;

typedef enum _lv_libssh2_knownhosts_check_results {
    LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_FAILURE = 0,
    LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_NOT_FOUND = 1,
    LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MATCH = 2,
    LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MISMATCH = 3,
} lv_libssh2_knownhosts_check_results_t;

typedef enum _lv_libssh2_knownhosts_get_results {
    LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_SUCCESS = 0,
    LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_END_OF_HOSTS = 1,
} lv_libssh2_knownhosts_get_results_t;

typedef enum _lv_libssh2_knownhost_key_algorithms {
    LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_RSA1 = 0,
    LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_SSHRSA = 1,
    LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_SSHDSS = 2,
} lv_libssh2_knownhost_key_algorithms_t;

typedef enum _lv_libssh2_session_block_directions {
    LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_READ = 0,
    LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_WRITE = 1,
    LV_LIBSSH2_SESSION_BLOCK_DIRECTIONS_BOTH = 2,
} lv_libssh2_session_block_directions_t;

typedef enum _lv_libssh2_session_options {
    LV_LIBSSH2_SESSION_OPTIONS_SIGPIPE = 0,
    LV_LIBSSH2_SESSION_OPTIONS_COMPRESS = 1,
} lv_libssh2_session_options_t;

typedef enum _lv_libssh2_methods {
    LV_LIBSSH2_METHOD_KEX = 0,
    LV_LIBSSH2_METHOD_HOSTKEY = 1,
    LV_LIBSSH2_METHOD_CRYPT_CS = 2,
    LV_LIBSSH2_METHOD_CRYPT_SC = 3,
    LV_LIBSSH2_METHOD_MAC_CS = 4,
    LV_LIBSSH2_METHOD_MAC_SC = 5,
    LV_LIBSSH2_METHOD_COMP_CS = 6,
    LV_LIBSSH2_METHOD_COMP_SC = 7,
    LV_LIBSSH2_METHOD_LANG_CS = 8,
    LV_LIBSSH2_METHOD_LANG_SC = 9,
} lv_libssh2_methods_t;

typedef enum _lv_libssh2_ignore_modes {
    LV_LIBSSH2_IGNORE_MODES_NORMAL = 0,
    LV_LIBSSH2_IGNORE_MODES_MERGE = 1,
    LV_LIBSSH2_IGNORE_MODES_IGNORE = 3,
} lv_libssh2_ignore_modes_t;

typedef enum _lv_libssh2_sftp_file_types {
    LV_LIBSSH2_SFTP_FILE_TYPES_MASK = 0170000,
    LV_LIBSSH2_SFTP_FILE_TYPES_NAMED_PIPE = 0010000,
    LV_LIBSSH2_SFTP_FILE_TYPES_SPECIAL_CHARACTER = 0020000,
    LV_LIBSSH2_SFTP_FILE_TYPES_DIRECTORY = 0040000,
    LV_LIBSSH2_SFTP_FILE_TYPES_SPECIAL_BLOCK = 0060000,
    LV_LIBSSH2_SFTP_FILE_TYPES_REGULAR = 0100000,
    LV_LIBSSH2_SFTP_FILE_TYPES_SYMBOLIC_LINK = 0120000,
    LV_LIBSSH2_SFTP_FILE_TYPES_SOCKET = 0140000,
} lv_libssh2_sftp_file_types_t;

typedef enum _lv_libssh2_sftp_file_modes {
    LV_LIBSSH2_SFTP_FILE_MODES_OWNER_MASK = 00000700,
    LV_LIBSSH2_SFTP_FILE_MODES_OWNER_READ = 00000400,
    LV_LIBSSH2_SFTP_FILE_MODES_OWNER_WRITE = 00000200,
    LV_LIBSSH2_SFTP_FILE_MODES_OWNER_EXECUTE = 00000100,
    LV_LIBSSH2_SFTP_FILE_MODES_GROUP_MASK = 00000070,
    LV_LIBSSH2_SFTP_FILE_MODES_GROUP_READ = 00000040,
    LV_LIBSSH2_SFTP_FILE_MODES_GROUP_WRITE = 00000020,
    LV_LIBSSH2_SFTP_FILE_MODES_GROUP_EXECUTE = 00000010,
    LV_LIBSSH2_SFTP_FILE_MODES_OTHER_MASK = 00000007,
    LV_LIBSSH2_SFTP_FILE_MODES_OTHER_READ = 00000004,
    LV_LIBSSH2_SFTP_FILE_MODES_OTHER_WRITE = 00000002,
    LV_LIBSSH2_SFTP_FILE_MODES_OTHER_EXECUTE = 00000001,
} lv_libssh2_sftp_file_modes_t;

typedef enum _lv_libssh2_sftp_open_flags {
    LV_LIBSSH2_SFTP_OPEN_FLAGS_READ = LIBSSH2_FXF_READ,
    LV_LIBSSH2_SFTP_OPEN_FLAGS_WRITE = LIBSSH2_FXF_WRITE,
    LV_LIBSSH2_SFTP_OPEN_FLAGS_APPEND = LIBSSH2_FXF_APPEND,
    LV_LIBSSH2_SFTP_OPEN_FLAGS_CREATE = LIBSSH2_FXF_CREAT,
    LV_LIBSSH2_SFTP_OPEN_FLAGS_TRUNCATE = LIBSSH2_FXF_TRUNC,
    LV_LIBSSH2_SFTP_OPEN_FLAGS_EXCLUDE = LIBSSH2_FXF_EXCL,
} lv_libssh2_sftp_open_flags_t;

typedef enum _lv_libssh2_sftp_open_types {
    LV_LIBSSH2_SFTP_OPEN_TYPES_FILE = LIBSSH2_SFTP_OPENFILE,
    LV_LIBSSH2_SFTP_OPEN_TYPES_DIRECTORY = LIBSSH2_SFTP_OPENDIR,
} lv_libssh2_sftp_open_types_t;

/**
 * The session
 */
typedef struct _lv_libssh2_session lv_libssh2_session_t;

/**
 * The known hosts
 */
typedef struct _lv_libssh2_knownhosts lv_libssh2_knownhosts_t;

/**
 * The known host
 */
typedef struct _lv_libssh2_knownhost lv_libssh2_knownhost_t;

/**
 * The SSH channel
 */
typedef struct _lv_libssh2_channel lv_libssh2_channel_t;

/**
 * The File Information structure, or stat info.
 */
typedef struct _lv_libssh2_fileinfo lv_libssh2_fileinfo_t;

/**
 * The listener.
 */
typedef struct _lv_libssh2_listener lv_libssh2_listener_t;

/**
 * The SFTP
 */
typedef struct _lv_libssh2_sftp lv_libssh2_sftp_t;

/**
  * The SFTP File
  */
typedef struct _lv_libssh2_sftp_file lv_libssh2_sftp_file_t;

/**
 * The SFTP Directory
 */
typedef struct _lv_libssh2_sftp_directory lv_libssh2_sftp_directory_t;

/**
 * The SFTP file/directory attributes
 */
typedef struct _lv_libssh2_sftp_attributes lv_libssh2_sftp_attributes_t;

/**
 * @defgroup global Global API
 *
 * Initialize and shutdown the library.
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_initialize();

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_shutdown();

/**
 * @}
 */

/**
 * @defgroup global Session API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_create(
    lv_libssh2_session_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_destroy(
    lv_libssh2_session_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_connect(
    lv_libssh2_session_t* handle,
    const uintptr_t socket
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_disconnect(
    lv_libssh2_session_t* handle,
    const char* description
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_hostkey_hash_len(
    const lv_libssh2_hostkey_hash_types_t type,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_hostkey_hash(
    lv_libssh2_session_t* handle,
    const lv_libssh2_hostkey_hash_types_t type,
    uint8_t* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_hostkey_len(
    lv_libssh2_session_t* handle,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_hostkey(
    lv_libssh2_session_t* handle,
    uint8_t* buffer,
    lv_libssh2_hostkey_types_t* type
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_set_mode(
    lv_libssh2_session_t* handle,
    const lv_libssh2_session_modes_t mode
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_mode(
    lv_libssh2_session_t* handle,
    lv_libssh2_session_modes_t* mode
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_banner_len(
    lv_libssh2_session_t* handle,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_banner(
    lv_libssh2_session_t* handle,
    char* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_set_banner(
    lv_libssh2_session_t* handle,
    const char* banner
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_block_direction(
    lv_libssh2_session_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_enable_option(
    lv_libssh2_session_t* handle,
    const lv_libssh2_session_options_t option
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_disable_option(
    lv_libssh2_session_t* handle,
    const lv_libssh2_session_options_t option
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_timeout(
    lv_libssh2_session_t* handle,
    long* milliseconds
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_set_timeout(
    lv_libssh2_session_t* handle,
    const long milliseconds
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_last_error_code(
    lv_libssh2_session_t* handle,
    int* code
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_last_error_len(
    lv_libssh2_session_t* handle,
    int32_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_last_error(
    lv_libssh2_session_t* handle,
    char* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_set_last_error(
    lv_libssh2_session_t* handle,
    const int code,
    const char* message
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_set_method_pref(
    lv_libssh2_session_t* handle,
    const lv_libssh2_methods_t method,
    const char* prefs
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_method_len(
    lv_libssh2_session_t* handle,
    const lv_libssh2_methods_t method,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_session_method(
    lv_libssh2_session_t* handle,
    const lv_libssh2_methods_t method,
    uint8_t* buffer
);

/**
 * @}
 */

/**
 * @defgroup global User Authentication API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_list_len(
    lv_libssh2_session_t* handle,
    const char* username,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_list(
    lv_libssh2_session_t* handle,
    const char* username,
    uint8_t* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_authenticated(
    lv_libssh2_session_t* handle,
    int* authenticated
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_hostbased_from_file(
    lv_libssh2_session_t* handle,
    const char* username,
    const char* public_key,
    const char* private_key,
    const char* passphrase,
    const char* hostname
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_password(
    lv_libssh2_session_t* handle,
    const char* username,
    const char* password
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_publickey_from_file(
    lv_libssh2_session_t* handle,
    const char* username,
    const char* public_key_path,
    const char* private_key_path,
    const char* passphrase
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_userauth_publickey_from_memory(
    lv_libssh2_session_t* handle,
    const char* username,
    const char* public_key_data,
    const size_t public_key_data_len,
    const char* private_key_data,
    const size_t private_key_data_len,
    const char* passphrase
);

/**
 * @}
 */

/**
 * @defgroup SCP API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_scp_send(
    lv_libssh2_session_t* session,
    const char* path,
    const int permissions,
    const size_t file_size,
    lv_libssh2_channel_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_scp_receive(
    lv_libssh2_session_t* session,
    const char* path,
    lv_libssh2_fileinfo_t* file_info,
    lv_libssh2_channel_t** handle
);

/**
 * @}
 */

/**
 * @defgroup SFTP API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_create(
    lv_libssh2_session_t* session,
    lv_libssh2_sftp_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_destroy(
    lv_libssh2_sftp_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_last_error(
    lv_libssh2_sftp_t* handle,
    uint32_t* code
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_open_file(
    lv_libssh2_sftp_t* sftp,
    const char* path,
    const uint32_t flags,
    const int32_t permissions,
    lv_libssh2_sftp_file_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_close_file(
    lv_libssh2_sftp_file_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_open_directory(
    lv_libssh2_sftp_t* sftp,
    const char* path,
    lv_libssh2_sftp_directory_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_close_directory(
    lv_libssh2_sftp_directory_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_read_file(
    lv_libssh2_sftp_file_t* handle,
    uint8_t* buffer,
    const size_t buffer_max_length,
    ssize_t* read_count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_read_directory(
    lv_libssh2_sftp_directory_t* handle,
    uint8_t* buffer,
    const size_t buffer_max_length,
    lv_libssh2_sftp_attributes_t* attributes,
    ssize_t* read_count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_write_file(
    lv_libssh2_sftp_file_t* handle,
    const uint8_t* buffer,
    const size_t buffer_length,
    ssize_t* write_count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_sync(
    lv_libssh2_sftp_file_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_seek(
    lv_libssh2_sftp_file_t* handle,
    const size_t offset
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_rewind(
    lv_libssh2_sftp_file_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_position(
    lv_libssh2_sftp_file_t* handle,
    uint64_t* position
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_status(
    lv_libssh2_sftp_file_t* handle,
    lv_libssh2_sftp_attributes_t* attributes
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_set_status(
    lv_libssh2_sftp_file_t* handle,
    lv_libssh2_sftp_attributes_t* attributes
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_link_status(
    lv_libssh2_sftp_t* handle,
    const char* path,
    lv_libssh2_sftp_attributes_t* attributes
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_file_rename(
    lv_libssh2_sftp_t* handle,
    const char* source_path,
    const char* destination_path
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_delete_file(
    lv_libssh2_sftp_t* handle,
    const char* file_path
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_create_directory(
    lv_libssh2_sftp_t* handle,
    const char* directory_path,
    const int32_t permissions
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_delete_directory(
    lv_libssh2_sftp_t* handle,
    const char* directory_path
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_create_link(
    lv_libssh2_sftp_t* handle,
    const char* source_path,
    const char* link_path
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_resolve_symbolic_link(
    lv_libssh2_sftp_t* handle,
    const char* link_path,
    uint8_t* source_path,
    const size_t source_path_max_length,
    size_t* read_count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_resolve_real_link(
    lv_libssh2_sftp_t* handle,
    const char* link_path,
    uint8_t* source_path,
    const size_t source_path_max_length,
    size_t* read_count
);

/**
 * @}
 */

/**
 * @defgroup SFTP Attribute API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_create(
    lv_libssh2_sftp_attributes_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_destroy(
    lv_libssh2_sftp_attributes_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_flags(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* flags
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_file_size(
    lv_libssh2_sftp_attributes_t* handle,
    uint64_t* file_size
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_uid(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* uid
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_gid(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* gid
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_permissions(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* permissions
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_atime(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* atime
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_mtime(
    lv_libssh2_sftp_attributes_t* handle,
    uint32_t* mtime
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_permissions(
    lv_libssh2_sftp_attributes_t* handle,
    const uint32_t permissions
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_uid(
    lv_libssh2_sftp_attributes_t* handle,
    const uint32_t uid
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_gid(
    lv_libssh2_sftp_attributes_t* handle,
    const uint32_t gid
);

/**
 * @}
 */

/**
 * @defgroup File Information API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_create(
    lv_libssh2_fileinfo_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_destroy(
    lv_libssh2_fileinfo_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_size(
    lv_libssh2_fileinfo_t* handle,
    uint64_t* size
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_atime(
    lv_libssh2_fileinfo_t* handle,
    uint64_t* atime
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_mtime(
    lv_libssh2_fileinfo_t* handle,
    uint64_t* mtime
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_fileinfo_permissions(
    lv_libssh2_fileinfo_t* handle,
    int32_t* permissions
);

/**
 * @}
 */

/**
 * @defgroup Channel API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_create(
    lv_libssh2_session_t* session,
    lv_libssh2_channel_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_destroy(
    lv_libssh2_channel_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_read(
    lv_libssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
);

lv_libssh2_status_t
lv_libssh2_channel_read_stderr(
    lv_libssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_close(
    lv_libssh2_channel_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_direct_tcpip(
    lv_libssh2_session_t* session,
    const char* host,
    int port,
    const char* server_host,
    int server_port,
    lv_libssh2_channel_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_eof(
    lv_libssh2_channel_t* handle,
    int* eof
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_flush(
    lv_libssh2_channel_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_forward_accept(
    lv_libssh2_session_t* session,
    lv_libssh2_listener_t* listener,
    lv_libssh2_channel_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_forward_cancel(
    lv_libssh2_listener_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_forward_listen(
    lv_libssh2_session_t* session,
    const int port,
    lv_libssh2_listener_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_exit_status(
    lv_libssh2_channel_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_set_ignore_mode(
    lv_libssh2_channel_t* handle,
    lv_libssh2_ignore_modes_t mode
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_shell(
    lv_libssh2_channel_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_exec(
    lv_libssh2_channel_t* handle,
    const char* command,
    const size_t command_len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_channel_subsystem(
    lv_libssh2_channel_t* handle,
    const char* subsystem,
    const size_t subsystem_len
);

/**
 * @}
 */

/**
 * @defgroup global Known Hosts API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_create(
    lv_libssh2_session_t* session,
    lv_libssh2_knownhosts_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_destroy(
    lv_libssh2_knownhosts_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_add(
    lv_libssh2_knownhosts_t* handle,
    const char* name,
    const char* salt,
    const char* key,
    const size_t key_len,
    const char* comment,
    const size_t comment_len,
    const int type_mask,
    lv_libssh2_knownhost_t* knownhost
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_type_mask(
    const lv_libssh2_knownhost_name_types_t type,
    const lv_libssh2_knownhost_key_encodings_t encoding,
    const lv_libssh2_knownhost_key_algorithms_t algorithm,
    int* type_mask
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_check(
    lv_libssh2_knownhosts_t* handle,
    const char* host,
    const int port,
    const uint8_t* key,
    const size_t key_len,
    const lv_libssh2_knownhost_name_types_t type,
    const lv_libssh2_knownhost_key_encodings_t encoding,
    lv_libssh2_knownhosts_check_results_t* result
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_check_and_get(
    lv_libssh2_knownhosts_t* handle,
    const char* host,
    const int port,
    const uint8_t* key,
    const size_t key_len,
    const lv_libssh2_knownhost_name_types_t type,
    const lv_libssh2_knownhost_key_encodings_t encoding,
    lv_libssh2_knownhost_t* known_host,
    lv_libssh2_knownhosts_check_results_t* result
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_delete(
    lv_libssh2_knownhosts_t* handle,
    lv_libssh2_knownhost_t* knownhost
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_read_file(
    lv_libssh2_knownhosts_t* handle,
    const char* file,
    size_t* count
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_read_line(
    lv_libssh2_knownhosts_t* handle,
    const char* line,
    const size_t line_len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_write_file(
    lv_libssh2_knownhosts_t* handle,
    const char* file
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_write_line(
    lv_libssh2_knownhosts_t* handle,
    lv_libssh2_knownhost_t* knownhost,
    char* line,
    const size_t line_len,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_first(
    lv_libssh2_knownhosts_t* handle,
    lv_libssh2_knownhost_t* host,
    lv_libssh2_knownhosts_get_results_t* result
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhosts_next(
    lv_libssh2_knownhosts_t* handle,
    lv_libssh2_knownhost_t* prev,
    lv_libssh2_knownhost_t* next,
    lv_libssh2_knownhosts_get_results_t* result
);

/**
 * @}
 */

/**
 * @defgroup global Known Host API
 *
 * @{
 */

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_create(
    lv_libssh2_knownhost_t** handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_destroy(
    lv_libssh2_knownhost_t* handle
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_magic(
    lv_libssh2_knownhost_t* handle,
    int* magic
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_name_len(
    lv_libssh2_knownhost_t* handle,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_name(
    lv_libssh2_knownhost_t* handle,
    uint8_t* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_key_len(
    lv_libssh2_knownhost_t* handle,
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_key(
    lv_libssh2_knownhost_t* handle,
    uint8_t* buffer
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_knownhost_type_mask(
    lv_libssh2_knownhost_t* handle,
    int* type_mask
);

/**
 * @}
 */

/**
 * @defgroup utility Utility API
 *
 * Obtain library-specific and error/status information.
 *
 * @{
 */

/**
 * Gets the library version number in Major.Minor.Patch notation.
 */
LV_LIBSSH2_API const char*
lv_libssh2_version();

/**
 * Gets the library major version number as an integer.
 */
LV_LIBSSH2_API unsigned int
lv_libssh2_version_major();

/**
 * Gets the library minor version number as an integer.
 */
LV_LIBSSH2_API unsigned int
lv_libssh2_version_minor();

/**
 * Gets the library patch version number as an integer.
 */
LV_LIBSSH2_API unsigned int
lv_libssh2_version_patch();

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_internal_version_len(
    size_t* len
);

LV_LIBSSH2_API lv_libssh2_status_t
lv_libssh2_internal_version(
    uint8_t* buffer
);

/**
 * Gets a string representation of the status.
 */
LV_LIBSSH2_API const char*
lv_libssh2_status_string(
    lv_libssh2_status_t status
);

LV_LIBSSH2_API bool
lv_libssh2_status_is_ok(
    lv_libssh2_status_t status
);

LV_LIBSSH2_API bool
lv_libssh2_status_is_err(
    lv_libssh2_status_t status
);

LV_LIBSSH2_API const char*
lv_libssh2_status_message(
    lv_libssh2_status_t status
);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

