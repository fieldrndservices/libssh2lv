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
    LABSSH2_STATUS_OK,

    LABSSH2_STATUS_ERROR_OUT_OF_MEMORY,
    LABSSH2_STATUS_ERROR_NULL_VALUE,
} labssh2_status_t;

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
LABSSH2_API const char* labssh2_version();

/**
 * Gets the library major version number as an integer.
 */
LABSSH2_API unsigned int labssh2_version_major();

/**
 * Gets the library minor version number as an integer.
 */
LABSSH2_API unsigned int labssh2_version_minor();

/**
 * Gets the library patch version number as an integer.
 */
LABSSH2_API unsigned int labssh2_version_patch();

/**
 * Gets an integer representation of the status. 
 *
 * Errors are negative values, warnings are positive values, and zero (0) is no
 * error or warning, i.e. "OK".
 */
LABSSH2_API int labssh2_status_code(labssh2_status_t status);

/**
 * Gets a string representation of the status.
 */
LABSSH2_API const char* labssh2_status_string(labssh2_status_t status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

