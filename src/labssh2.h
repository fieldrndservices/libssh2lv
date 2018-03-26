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
    LABSSH2_STATUS_OK = 0,

    LABSSH2_STATUS_ERROR_MEMORY,
    LABSSH2_STATUS_ERROR_NULL_VALUE,
    LABSSH2_STATUS_ERROR_SESSION,
    LABSSH2_STATUS_ERROR_KNOWNHOSTS,
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

typedef enum _labssh2_knownhost_types {
    LABSSH2_KNOWNHOST_TYPE_PLAIN = 0,
    LABSSH2_KNOWNHOST_TYPE_SHA1 = 1,
    LABSSH2_KNOWNHOST_TYPE_CUSTOM = 2,
} labssh2_knownhost_types_t;

typedef enum _labssh2_knownhost_key_encodings {
    LABSSH2_KNOWNHOST_KEY_ENCODING_RAW = 0,
    LABSSH2_KNOWNHOST_KEY_ENCODING_BASE64 = 1,
} labssh2_knownhost_key_encodings_t;

typedef enum _labssh2_knownhost_key_algorithms {
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_RSA1 = 0,
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_SSHRSA = 1,
    LABSSH2_KNOWNHOST_KEY_ALGORITHM_SSHDSS = 2,
} labssh2_knownhost_key_algorithms_t;

/**
 * The context
 */
typedef struct _labssh2 labssh2_t;

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
 * @defgroup global Global API
 *
 * Initialize and shutdown the library.
 *
 * @{
 */

LABSSH2_API labssh2_t* 
labssh2_create();

LABSSH2_API void 
labssh2_destroy(
    labssh2_t* ctx
);

LABSSH2_API labssh2_status_t 
labssh2_status(
    labssh2_t* ctx
);

LABSSH2_API const char* 
labssh2_source(
    labssh2_t* ctx
);

LABSSH2_API const char* 
labssh2_message(
    labssh2_t* ctx
);

LABSSH2_API bool 
labssh2_is_ok(
    labssh2_t* ctx
);

LABSSH2_API bool 
labssh2_is_err(
    labssh2_t* ctx
);

/**
 * @}
 */

/**
 * @defgroup global Session API
 *
 * @{
 */

LABSSH2_API labssh2_session_t* 
labssh2_session_create(
    labssh2_t* ctx
);

LABSSH2_API void 
labssh2_session_destroy(
    labssh2_t* ctx, 
    labssh2_session_t* session
);

LABSSH2_API void 
labssh2_session_connect(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    uintptr_t socket
);

LABSSH2_API void 
labssh2_session_disconnect(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    const char* description
);

LABSSH2_API size_t 
labssh2_session_hostkey_hash_len(
    labssh2_hostkey_hash_types_t type
);

LABSSH2_API void 
labssh2_session_hostkey_hash(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    labssh2_hostkey_hash_types_t type,
    uint8_t* buffer
);

LABSSH2_API size_t
labssh2_session_hostkey_len(
    labssh2_t* ctx,
    labssh2_session_t* session
);

LABSSH2_API void
labssh2_session_hostkey(
    labssh2_t* ctx,
    labssh2_session_t* session,
    uint8_t* buffer,
    labssh2_hostkey_types_t* type
);

/**
 * @}
 */

/**
 * @defgroup global Known Hosts API
 *
 * @{
 */

LABSSH2_API labssh2_knownhosts_t* 
labssh2_knownhosts_create(
    labssh2_t* ctx,
    labssh2_session_t* session
);

LABSSH2_API void 
labssh2_knownhosts_destroy(
    labssh2_knownhosts_t* knownhosts
);

LABSSH2_API bool
labssh2_knownhosts_get(
    labssh2_t* ctx,
    labssh2_knownhosts_t* knownhosts,
    labssh2_knownhost_t* knownhost
);

LABSSH2_API void
labssh2_knownhosts_add(
    labssh2_t* ctx,
    labssh2_knownhosts_t* knownhosts,
    const char* name,
    const char* salt,
    const char* key,
    const size_t key_len,
    const char* comment,
    const size_t comment_len,
    int type_mask
);

LABSSH2_API int
labssh2_knownhosts_type_mask(
    labssh2_knownhost_types_t type,
    labssh2_knownhost_key_encodings_t encoding,
    labssh2_knownhost_key_algorithms_t algorithm,
);
 
/**
 * @}
 */

/**
 * @defgroup global Known Host API
 *
 * @{
 */

LABSSH2_API labssh2_knownhost_t* 
labssh2_knownhost_create(
    labssh2_t* ctx
);

LABSSH2_API void 
labssh2_knownhost_destroy(
    labssh2_knownhost_t* knownhosts
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

/**
 * Gets an integer representation of the status. 
 *
 * Errors are negative values, warnings are positive values, and zero (0) is no
 * error or warning, i.e. "OK".
 */
LABSSH2_API int 
labssh2_status_code(
    labssh2_status_t status
);

/**
 * Gets a string representation of the status.
 */
LABSSH2_API const char* 
labssh2_status_string(
    labssh2_status_t status
);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

