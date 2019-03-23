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

/**
 * @file
 *
 * Includes the full LabSSH2 API.
 */

#ifndef LABSSH2_H
#define LABSSH2_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "libssh2.h"
#include "libssh2_sftp.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LABSSH2_API
#  ifdef _WIN32
#     if defined(LABSSH2_BUILD_SHARED) /* build dll */
#         define LABSSH2_API __declspec(dllexport)
#     elif !defined(LABSSH2_BUILD_STATIC) /* use dll */
#         define LABSSH2_API __declspec(dllimport)
#     else /* static library */
#         define LABSSH2_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define LABSSH2_API __attribute__((visibility("default")))
#     else
#         define LABSSH2_API
#     endif
#  endif
#endif

/**
 * Status
 */
typedef enum _labssh2_status {
    LABSSH2_STATUS_MISMATCH = 4,
    LABSSH2_STATUS_MATCH = 3,
    LABSSH2_STATUS_NOT_FOUND = 2,
    LABSSH2_STATUS_END_OF_HOSTS = 1,

    LABSSH2_STATUS_OK = 0,

    LABSSH2_STATUS_ERROR_GENERIC = -1,
    LABSSH2_STATUS_ERROR_MALLOC = -2,
    LABSSH2_STATUS_ERROR_FREE = -3,
    LABSSH2_STATUS_ERROR_NULL_VALUE = -4,
    LABSSH2_STATUS_ERROR_SOCKET_NONE = -5,
    LABSSH2_STATUS_ERROR_BANNER_RECV = -6,
    LABSSH2_STATUS_ERROR_BANNER_SEND = -7,
    LABSSH2_STATUS_ERROR_INVALID_MAC = -8,
    LABSSH2_STATUS_ERROR_KEX_FAILURE = -9,
    LABSSH2_STATUS_ERROR_SOCKET_SEND = -10,
    LABSSH2_STATUS_ERROR_KEY_EXCHANGE_FAILURE = -11,
    LABSSH2_STATUS_ERROR_TIMEOUT = -12,
    LABSSH2_STATUS_ERROR_HOST_KEY_INITIALIZE = -13,
    LABSSH2_STATUS_ERROR_HOST_KEY_SIGNATURE = -14,
    LABSSH2_STATUS_ERROR_DECRYPTION = -15,
    LABSSH2_STATUS_ERROR_SOCKET_DISCONNECT = -16,
    LABSSH2_STATUS_ERROR_PROTOCOL = -17,
    LABSSH2_STATUS_ERROR_PASSWORD_EXPIRED = -18,
    LABSSH2_STATUS_ERROR_FILE = -19,
    LABSSH2_STATUS_ERROR_NONE = -20,
    LABSSH2_STATUS_ERROR_AUTHENTICATION = -21,
    LABSSH2_STATUS_ERROR_PUBLIC_KEY_UNVERIFIED = -22,
    LABSSH2_STATUS_ERROR_CHANNEL_OUT_OF_ORDER = -23,
    LABSSH2_STATUS_ERROR_CHANNEL_FAILURE = -24,
    LABSSH2_STATUS_ERROR_CHANNEL_REQUEST_DENIED = -25,
    LABSSH2_STATUS_ERROR_CHANNEL_UNKNOWN = -26,
    LABSSH2_STATUS_ERROR_CHANNEL_WINDOW_EXCEEDED = -27,
    LABSSH2_STATUS_ERROR_CHANNEL_PACKET_EXCEEDED = -28,
    LABSSH2_STATUS_ERROR_CHANNEL_CLOSED = -29,
    LABSSH2_STATUS_ERROR_CHANNEL_EOF_SENT = -30,
    LABSSH2_STATUS_ERROR_SCP_PROTOCOL = -31,
    LABSSH2_STATUS_ERROR_ZLIB = -32,
    LABSSH2_STATUS_ERROR_SOCKET_TIMEOUT = -33,
    LABSSH2_STATUS_ERROR_SFTP_PROTOCOL = -34,
    LABSSH2_STATUS_ERROR_REQUEST_DENIED = -35,
    LABSSH2_STATUS_ERROR_METHOD_NOT_SUPPORTED = -36,
    LABSSH2_STATUS_ERROR_INVALID = -37,
    LABSSH2_STATUS_ERROR_INVALID_POLL_TYPE = -38,
    LABSSH2_STATUS_ERROR_PUBLIC_KEY_PROTOCOL = -39,
    LABSSH2_STATUS_ERROR_EXECUTE_AGAIN = -40,
    LABSSH2_STATUS_ERROR_BUFFER_TOO_SMALL = -41,
    LABSSH2_STATUS_ERROR_BAD_USE = -42,
    LABSSH2_STATUS_ERROR_COMMPRESS = -43,
    LABSSH2_STATUS_ERROR_OUT_OF_BOUNDARY = -44,
    LABSSH2_STATUS_ERROR_AGENT_PROTOCOL = -45,
    LABSSH2_STATUS_ERROR_SOCKET_RECV = -46,
    LABSSH2_STATUS_ERROR_ENCRYPTION = -47,
    LABSSH2_STATUS_ERROR_BAD_SOCKET = -48,
    LABSSH2_STATUS_ERROR_KNOWN_HOSTS = -49,
    LABSSH2_STATUS_ERROR_UNKNOWN_HASH_ALGORITHM = -50,
    LABSSH2_STATUS_ERROR_HASH_UNAVAILABLE = -51,
    LABSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE = -52,
    LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING = -53,
    LABSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM = -54,
    LABSSH2_STATUS_ERROR_UNKNOWN_MODE = -55,
    LABSSH2_STATUS_ERROR_UNKNOWN_BLOCK_DIRECTION = -56,
    LABSSH2_STATUS_ERROR_UNKNOWN_SESSION_OPTION = -57,
    LABSSH2_STATUS_ERROR_SESSION_NOT_STARTED = -58,
    LABSSH2_STATUS_ERROR_VERSION_TOO_OLD = -59,
    LABSSH2_STATUS_ERROR_UNKNOWN_IGNORE_MODE = -60
} labssh2_status_t;

typedef enum _labssh2_session_modes {
    LABSSH2_SESSION_MODE_NONBLOCKING = 0,
    LABSSH2_SESSION_MODE_BLOCKING = 1
} labssh2_session_modes_t;

typedef enum _labssh2_hostkey_hash_types {
    LABSSH2_HOSTKEY_HASH_TYPE_MD5 = 0,
    LABSSH2_HOSTKEY_HASH_TYPE_SHA1 = 1
} labssh2_hostkey_hash_types_t;

typedef enum _labssh2_hostkey_types {
    LABSSH2_HOSTKEY_TYPE_RSA = 0,
    LABSSH2_HOSTKEY_TYPE_DSS = 1,
    LABSSH2_HOSTKEY_TYPE_UNKNOWN = 2
} labssh2_hostkey_types_t;

typedef enum _labssh2_knownhost_name_types {
    LABSSH2_KNOWNHOST_NAME_TYPE_PLAIN = 0,
    LABSSH2_KNOWNHOST_NAME_TYPE_SHA1 = 1,
    LABSSH2_KNOWNHOST_NAME_TYPE_CUSTOM = 2,
} labssh2_knownhost_name_types_t;

typedef enum _labssh2_knownhost_key_encodings {
    LABSSH2_KNOWNHOST_KEY_ENCODING_RAW = 0,
    LABSSH2_KNOWNHOST_KEY_ENCODING_BASE64 = 1,
} labssh2_knownhost_key_encodings_t;

typedef enum _labssh2_knownhost_key_algorithms {
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_RSA1 = 0,
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_SSHRSA = 1,
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_SSHDSS = 2,
} labssh2_knownhost_key_algorithms_t;

typedef enum _labssh2_knownhosts_check_result {
    LABSSH2_KNOWNHOSTS_CHECK_RESULT_FAILURE = 0,
    LABSSH2_KNOWNHOSTS_CHECK_RESULT_NOT_FOUND = 1,
    LABSSH2_KNOWNHOSTS_CHECK_RESULT_MATCH = 2,
    LABSSH2_KNOWNHOSTS_CHECK_RESULT_MISMATCH = 3,
} labssh2_knownhosts_check_result_t;

typedef enum _labssh2_session_block_directions {
    LABSSH2_SESSION_BLOCK_DIRECTIONS_READ = 0,
    LABSSH2_SESSION_BLOCK_DIRECTIONS_WRITE = 1,
    LABSSH2_SESSION_BLOCK_DIRECTIONS_BOTH = 2,
} labssh2_session_block_directions_t;

typedef enum _labssh2_session_options {
    LABSSH2_SESSION_OPTIONS_SIGPIPE = 0,
    LABSSH2_SESSION_OPTIONS_COMPRESS = 1,
} labssh2_session_options_t;

typedef enum _labssh2_methods {
    LABSSH2_METHOD_KEX = 0,
    LABSSH2_METHOD_HOSTKEY = 1,
    LABSSH2_METHOD_CRYPT_CS = 2,
    LABSSH2_METHOD_CRYPT_SC = 3,
    LABSSH2_METHOD_MAC_CS = 4,
    LABSSH2_METHOD_MAC_SC = 5,
    LABSSH2_METHOD_COMP_CS = 6,
    LABSSH2_METHOD_COMP_SC = 7,
    LABSSH2_METHOD_LANG_CS = 8,
    LABSSH2_METHOD_LANG_SC = 9,
} labssh2_methods_t;

typedef enum _labssh2_ignore_modes {
    LABSSH2_IGNORE_MODES_NORMAL = 0,
    LABSSH2_IGNORE_MODES_MERGE = 1,
    LABSSH2_IGNORE_MODES_IGNORE = 3,
} labssh2_ignore_modes_t;

typedef enum _labssh2_sftp_file_types {
    LABSSH2_SFTP_FILE_TYPES_MASK = 0170000,
    LABSSH2_SFTP_FILE_TYPES_NAMED_PIPE = 0010000,
    LABSSH2_SFTP_FILE_TYPES_SPECIAL_CHARACTER = 0020000,
    LABSSH2_SFTP_FILE_TYPES_DIRECTORY = 0040000,
    LABSSH2_SFTP_FILE_TYPES_SPECIAL_BLOCK = 0060000,
    LABSSH2_SFTP_FILE_TYPES_REGULAR = 0100000,
    LABSSH2_SFTP_FILE_TYPES_SYMBOLIC_LINK = 0120000,
    LABSSH2_SFTP_FILE_TYPES_SOCKET = 0140000,
} labssh2_sftp_file_types_t;

typedef enum _labssh2_sftp_file_modes {
    LABSSH2_SFTP_FILE_MODES_OWNER_MASK = 00000700,
    LABSSH2_SFTP_FILE_MODES_OWNER_READ = 00000400,
    LABSSH2_SFTP_FILE_MODES_OWNER_WRITE = 00000200,
    LABSSH2_SFTP_FILE_MODES_OWNER_EXECUTE = 00000100,
    LABSSH2_SFTP_FILE_MODES_GROUP_MASK = 00000070,
    LABSSH2_SFTP_FILE_MODES_GROUP_READ = 00000040,
    LABSSH2_SFTP_FILE_MODES_GROUP_WRITE = 00000020,
    LABSSH2_SFTP_FILE_MODES_GROUP_EXECUTE = 00000010,
    LABSSH2_SFTP_FILE_MODES_OTHER_MASK = 00000007,
    LABSSH2_SFTP_FILE_MODES_OTHER_READ = 00000004,
    LABSSH2_SFTP_FILE_MODES_OTHER_WRITE = 00000002,
    LABSSH2_SFTP_FILE_MODES_OTHER_EXECUTE = 00000001,
} labssh2_sftp_file_modes_t;

typedef enum _labssh2_sftp_open_flags {
    LABSSH2_SFTP_OPEN_FLAGS_READ = LIBSSH2_FXF_READ,
    LABSSH2_SFTP_OPEN_FLAGS_WRITE = LIBSSH2_FXF_WRITE,
    LABSSH2_SFTP_OPEN_FLAGS_APPEND = LIBSSH2_FXF_APPEND,
    LABSSH2_SFTP_OPEN_FLAGS_CREATE = LIBSSH2_FXF_CREAT,
    LABSSH2_SFTP_OPEN_FLAGS_TRUNCATE = LIBSSH2_FXF_TRUNC,
    LABSSH2_SFTP_OPEN_FLAGS_EXCLUDE = LIBSSH2_FXF_EXCL,
} labssh2_sftp_open_flags_t;

typedef enum _labssh2_sftp_open_types {
    LABSSH2_SFTP_OPEN_TYPES_FILE = LIBSSH2_SFTP_OPENFILE,
    LABSSH2_SFTP_OPEN_TYPES_DIRECTORY = LIBSSH2_SFTP_OPENDIR,
} labssh2_sftp_open_types_t;

/**
 * The session
 */
typedef struct _labssh2_session labssh2_session_t;

/**
 * The known hosts
 */
typedef struct _labssh2_knownhosts labssh2_knownhosts_t;

/**
 * The known host
 */
typedef struct _labssh2_knownhost labssh2_knownhost_t;

/**
 * The SSH channel
 */
typedef struct _labssh2_channel labssh2_channel_t;

/**
 * The File Information structure, or stat info.
 */
typedef struct _labssh2_fileinfo labssh2_fileinfo_t;

/**
 * The listener.
 */
typedef struct _labssh2_listener labssh2_listener_t;

/**
 * The SFTP
 */
typedef struct _labssh2_sftp labssh2_sftp_t;

/**
  * The SFTP File
  */
typedef struct _labssh2_sftp_file labssh2_sftp_file_t;

/**
 * The SFTP Directory
 */
typedef struct _labssh2_sftp_directory labssh2_sftp_directory_t;

/**
 * The SFTP file/directory attributes
 */
typedef struct _labssh2_sftp_attributes labssh2_sftp_attributes_t;

/**
 * @defgroup global Global API
 *
 * Initialize and shutdown the library.
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_initialize();

LABSSH2_API labssh2_status_t
labssh2_shutdown();

/**
 * @}
 */

/**
 * @defgroup global Session API
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_session_create(
    labssh2_session_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_session_destroy(
    labssh2_session_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_session_connect(
    labssh2_session_t* handle,
    const uintptr_t socket
);

LABSSH2_API labssh2_status_t
labssh2_session_disconnect(
    labssh2_session_t* handle,
    const char* description
);

LABSSH2_API labssh2_status_t
labssh2_session_hostkey_hash_len(
    const labssh2_hostkey_hash_types_t type,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_session_hostkey_hash(
    labssh2_session_t* handle,
    const labssh2_hostkey_hash_types_t type,
    uint8_t* buffer
);

LABSSH2_API labssh2_status_t
labssh2_session_hostkey_len(
    labssh2_session_t* handle,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_session_hostkey(
    labssh2_session_t* handle,
    uint8_t* buffer,
    labssh2_hostkey_types_t* type
);

LABSSH2_API labssh2_status_t
labssh2_session_set_mode(
    labssh2_session_t* handle,
    labssh2_session_modes_t mode
);

LABSSH2_API labssh2_status_t
labssh2_session_mode(
    labssh2_session_t* handle,
    labssh2_session_modes_t* mode
);

LABSSH2_API labssh2_status_t
labssh2_session_banner_len(
    labssh2_session_t* handle,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_session_banner(
    labssh2_session_t* handle,
    char* buffer
);

LABSSH2_API labssh2_status_t
labssh2_session_set_banner(
    labssh2_session_t* handle,
    const char* banner
);

LABSSH2_API labssh2_status_t
labssh2_session_block_direction(
    labssh2_session_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_session_enable_option(
    labssh2_session_t* handle,
    labssh2_session_options_t option
);

LABSSH2_API labssh2_status_t
labssh2_session_disable_option(
    labssh2_session_t* handle,
    labssh2_session_options_t option
);

LABSSH2_API labssh2_status_t
labssh2_session_timeout(
    labssh2_session_t* handle,
    long* milliseconds
);

LABSSH2_API labssh2_status_t
labssh2_session_set_timeout(
    labssh2_session_t* handle,
    const long milliseconds
);

LABSSH2_API labssh2_status_t
labssh2_session_last_error_code(
    labssh2_session_t* handle,
    int* code
);

LABSSH2_API labssh2_status_t
labssh2_session_last_error_len(
    labssh2_session_t* handle,
    int32_t* len
);

LABSSH2_API labssh2_status_t
labssh2_session_last_error(
    labssh2_session_t* handle,
    char* buffer
);

LABSSH2_API labssh2_status_t
labssh2_session_set_last_error(
    labssh2_session_t* handle,
    int code,
    const char* message
);

LABSSH2_API labssh2_status_t
labssh2_session_set_method_pref(
    labssh2_session_t* handle,
    labssh2_methods_t method,
    const char* prefs
);

LABSSH2_API labssh2_status_t
labssh2_session_method_len(
    labssh2_session_t* handle,
    labssh2_methods_t method,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_session_method(
    labssh2_session_t* handle,
    labssh2_methods_t method,
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

LABSSH2_API labssh2_status_t
labssh2_userauth_list_len(
    labssh2_session_t* handle,
    const char* username,
    size_t username_len,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_userauth_list(
    labssh2_session_t* handle,
    const char* username,
    size_t username_len,
    uint8_t* buffer
);

LABSSH2_API labssh2_status_t
labssh2_userauth_authenticated(
    labssh2_session_t* handle,
    int* authenticated
);

LABSSH2_API labssh2_status_t
labssh2_userauth_hostbased_from_file(
    labssh2_session_t* handle,
    const char* username,
    const size_t username_len,
    const char* public_key,
    const char* private_key,
    const char* passphrase,
    const char* hostname,
    const size_t hostname_len,
    const char* local_username,
    const size_t local_username_len
);

LABSSH2_API labssh2_status_t
labssh2_userauth_password(
    labssh2_session_t* handle,
    const char* username,
    const size_t username_len,
    const char* password,
    const size_t password_len
);

LABSSH2_API labssh2_status_t
labssh2_userauth_publickey_from_file(
    labssh2_session_t* handle,
    const char* username,
    const size_t username_len,
    const char* public_key_path,
    const char* private_key_path,
    const char* passphrase
);

LABSSH2_API labssh2_status_t
labssh2_userauth_publickey_from_memory(
    labssh2_session_t* handle,
    const char* username,
    const size_t username_len,
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

LABSSH2_API labssh2_status_t
labssh2_scp_send(
    labssh2_session_t* session,
    const char* path,
    int mode,
    size_t file_size,
    labssh2_channel_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_scp_receive(
    labssh2_session_t* session,
    const char* path,
    labssh2_fileinfo_t* file_info,
    labssh2_channel_t** handle
);

/**
 * @}
 */

/**
 * @defgroup SFTP API
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_sftp_create(
    labssh2_session_t* session,
    labssh2_sftp_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_destroy(
    labssh2_sftp_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_last_error(
    labssh2_sftp_t* handle,
    uint32_t* code
);

LABSSH2_API labssh2_status_t
labssh2_sftp_open_file(
    labssh2_sftp_t* sftp,
    const char* path,
    uint32_t flags,
    int32_t permissions,
    labssh2_sftp_file_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_close_file(
    labssh2_sftp_file_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_open_directory(
    labssh2_sftp_t* sftp,
    const char* path,
    labssh2_sftp_directory_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_close_directory(
    labssh2_sftp_directory_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_read_file(
    labssh2_sftp_file_t* handle,
    uint8_t* buffer,
    size_t buffer_max_length,
    ssize_t* read_count
);

LABSSH2_API labssh2_status_t
labssh2_sftp_read_directory(
    labssh2_sftp_directory_t* handle,
    uint8_t* buffer,
    size_t buffer_max_length,
    labssh2_sftp_attributes_t* attributes,
    ssize_t* read_count
);

LABSSH2_API labssh2_status_t
labssh2_sftp_write_file(
    labssh2_sftp_file_t* handle,
    uint8_t* buffer,
    size_t count
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_sync(
    labssh2_sftp_file_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_seek(
    labssh2_sftp_file_t* handle,
    size_t offest
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_rewind(
    labssh2_sftp_file_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_position(
    labssh2_sftp_file_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_status(
    labssh2_sftp_file_t* handle,
    labssh2_sftp_attributes_t* attributes
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_set_status(
    labssh2_sftp_file_t* handle,
    labssh2_sftp_attributes_t* attributes
);

LABSSH2_API labssh2_status_t
labssh2_sftp_link_status(
    labssh2_sftp_file_t* handle,
    labssh2_sftp_attributes_t* attributes
);

LABSSH2_API labssh2_status_t
labssh2_sftp_file_rename(
    labssh2_sftp_file_t* handle,
    const char* source_path,
    const char* destination_path
);

LABSSH2_API labssh2_status_t
labssh2_sftp_unlink(
    labssh2_sftp_t* handle,
    const char* file_path
);

LABSSH2_API labssh2_status_t
labssh2_sftp_create_directory(
    labssh2_sftp_t* handle,
    const char* directory_path,
    int32_t mode
);

LABSSH2_API labssh2_status_t
labssh2_sftp_remove_directory(
    labssh2_sftp_t* handle,
    const char* directory_path
);

LABSSH2_API labssh2_status_t
labssh2_sftp_create_link(
    labssh2_sftp_t* handle,
    const char* source_path,
    const char* link_path
);

LABSSH2_API labssh2_status_t
labssh2_sftp_resolve_symbolic_link(
    labssh2_sftp_t* handle,
    const char* link_path,
    char* source_path,
    uint32_t source_path_max_length
);

LABSSH2_API labssh2_status_t
labssh2_sftp_resolve_real_link(
    labssh2_sftp_t* handle,
    const char* link_path,
    char* source_path,
    uint32_t source_path_max_length
);

LABSSH2_API labssh2_status_t
labssh2_sftp_attributes_create(
    labssh2_sftp_attributes_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_sftp_attributes_destroy(
    labssh2_sftp_attributes_t* handle
);

/**
 * @}
 */

/**
 * @defgroup File Information API
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_fileinfo_create(
    labssh2_fileinfo_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_fileinfo_destroy(
    labssh2_fileinfo_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_fileinfo_size(
    labssh2_fileinfo_t* handle,
    uint64_t* size
);

/**
 * @}
 */

/**
 * @defgroup Channel API
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_channel_create(
    labssh2_session_t* session,
    labssh2_channel_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_destroy(
    labssh2_channel_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_read(
    labssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
);

labssh2_status_t
labssh2_channel_read_stderr(
    labssh2_channel_t* handle,
    char* buffer,
    const size_t buffer_len,
    size_t* byte_count
);

LABSSH2_API labssh2_status_t
labssh2_channel_close(
    labssh2_channel_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_direct_tcpip(
    labssh2_session_t* session,
    const char* host,
    int port,
    const char* server_host,
    int server_port,
    labssh2_channel_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_eof(
    labssh2_channel_t* handle,
    int* eof
);

LABSSH2_API labssh2_status_t
labssh2_channel_flush(
    labssh2_channel_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_forward_accept(
    labssh2_session_t* session,
    labssh2_listener_t* listener,
    labssh2_channel_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_forward_cancel(
    labssh2_listener_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_forward_listen(
    labssh2_session_t* session,
    const int port,
    labssh2_listener_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_exit_status(
    labssh2_channel_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_set_ignore_mode(
    labssh2_channel_t* handle,
    labssh2_ignore_modes_t mode
);

LABSSH2_API labssh2_status_t
labssh2_channel_shell(
    labssh2_channel_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_channel_exec(
    labssh2_channel_t* handle,
    const char* command,
    const size_t command_len
);

LABSSH2_API labssh2_status_t
labssh2_channel_subsystem(
    labssh2_channel_t* handle,
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

LABSSH2_API labssh2_status_t
labssh2_knownhosts_create(
    labssh2_session_t* session,
    labssh2_knownhosts_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_destroy(
    labssh2_knownhosts_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_get(
    labssh2_knownhosts_t* handle,
    labssh2_knownhost_t* knownhost
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_add(
    labssh2_knownhosts_t* handle,
    const char* name,
    const char* salt,
    const char* key,
    const size_t key_len,
    const char* comment,
    const size_t comment_len,
    const int type_mask,
    labssh2_knownhost_t* knownhost
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_type_mask(
    const labssh2_knownhost_name_types_t type,
    const labssh2_knownhost_key_encodings_t encoding,
    const labssh2_knownhost_key_algorithms_t algorithm,
    int* type_mask
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_check(
    labssh2_knownhosts_t* handle,
    const char* host,
    const int port,
    const char* key,
    const size_t key_len,
    const int type_mask,
    labssh2_knownhost_t* knownhost
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_delete(
    labssh2_knownhosts_t* handle,
    labssh2_knownhost_t* knownhost
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_read_file(
    labssh2_knownhosts_t* handle,
    const char* file,
    size_t* count
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_read_line(
    labssh2_knownhosts_t* handle,
    const char* line,
    const size_t line_len
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_write_file(
    labssh2_knownhosts_t* handle,
    const char* file
);

LABSSH2_API labssh2_status_t
labssh2_knownhosts_write_line(
    labssh2_knownhosts_t* handle,
    labssh2_knownhost_t* knownhost,
    char* line,
    const size_t line_len,
    size_t* len
);

/**
 * @}
 */

/**
 * @defgroup global Known Host API
 *
 * @{
 */

LABSSH2_API labssh2_status_t
labssh2_knownhost_create(
    labssh2_knownhost_t** handle
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_destroy(
    labssh2_knownhost_t* handle
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_magic(
    labssh2_knownhost_t* handle,
    int* magic
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_name_len(
    labssh2_knownhost_t* handle,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_name(
    labssh2_knownhost_t* handle,
    char* buffer
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_key_len(
    labssh2_knownhost_t* handle,
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_key(
    labssh2_knownhost_t* handle,
    char* buffer
);

LABSSH2_API labssh2_status_t
labssh2_knownhost_type_mask(
    labssh2_knownhost_t* handle,
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
LABSSH2_API const char*
labssh2_version();

/**
 * Gets the library major version number as an integer.
 */
LABSSH2_API unsigned int
labssh2_version_major();

/**
 * Gets the library minor version number as an integer.
 */
LABSSH2_API unsigned int
labssh2_version_minor();

/**
 * Gets the library patch version number as an integer.
 */
LABSSH2_API unsigned int
labssh2_version_patch();

LABSSH2_API labssh2_status_t
labssh2_internal_version_len(
    size_t* len
);

LABSSH2_API labssh2_status_t
labssh2_internal_version(
    uint8_t* buffer
);

/**
 * Gets a string representation of the status.
 */
LABSSH2_API const char*
labssh2_status_string(
    labssh2_status_t status
);

LABSSH2_API bool
labssh2_status_is_ok(
    labssh2_status_t status
);

LABSSH2_API bool
labssh2_status_is_err(
    labssh2_status_t status
);

LABSSH2_API const char*
labssh2_status_message(
    labssh2_status_t status
);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

