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

#include "lv-libssh2-session-private.h"
#include "lv-libssh2-status-private.h"
#include "lv-libssh2.h"

lv_libssh2_status_t lv_libssh2_userauth_list_len(lv_libssh2_session_t *handle,
                                                 const char *username,
                                                 size_t *len) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (len == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  char *list = libssh2_userauth_list(handle->inner, username,
                                     (unsigned int)strlen(username));
  if (list == NULL) {
    return lv_libssh2_status_from_result(
        libssh2_session_last_errno(handle->inner));
  }
  *len = strlen(list);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_userauth_list(lv_libssh2_session_t *handle,
                                             const char *username,
                                             uint8_t *buffer) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (buffer == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  char *list = libssh2_userauth_list(handle->inner, username,
                                     (unsigned int)strlen(username));
  if (list == NULL) {
    return lv_libssh2_status_from_result(
        libssh2_session_last_errno(handle->inner));
  }
  memcpy(buffer, list, strlen(list));
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_userauth_authenticated(lv_libssh2_session_t *handle,
                                  int *authenticated) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *authenticated = libssh2_userauth_authenticated(handle->inner);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_userauth_hostbased_from_file(
    lv_libssh2_session_t *handle, const char *username, const char *public_key,
    const char *private_key, const char *passphrase, const char *hostname) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (public_key == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (private_key == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (hostname == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_userauth_hostbased_fromfile_ex(
      handle->inner, username, (unsigned int)strlen(username), public_key,
      private_key, passphrase, hostname, (unsigned int)strlen(hostname),
      username, (unsigned int)strlen(username));
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t lv_libssh2_userauth_password(lv_libssh2_session_t *handle,
                                                 const char *username,
                                                 const char *password) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (password == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_userauth_password_ex(
      handle->inner, username, (unsigned int)strlen(username), password,
      (unsigned int)strlen(password), NULL);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t lv_libssh2_userauth_publickey_from_file(
    lv_libssh2_session_t *handle, const char *username,
    const char *public_key_path, const char *private_key_path,
    const char *passphrase) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (public_key_path == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (private_key_path == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_userauth_publickey_fromfile_ex(
      handle->inner, username, (unsigned int)strlen(username), public_key_path,
      private_key_path, passphrase);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t lv_libssh2_userauth_publickey_from_memory(
    lv_libssh2_session_t *handle, const char *username,
    const char *public_key_data, const size_t public_key_data_len,
    const char *private_key_data, const size_t private_key_data_len,
    const char *passphrase) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (public_key_data == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (private_key_data == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_userauth_publickey_frommemory(
      handle->inner, username, (unsigned int)strlen(username), public_key_data,
      public_key_data_len, private_key_data, private_key_data_len, passphrase);
  return lv_libssh2_status_from_result(result);
}
