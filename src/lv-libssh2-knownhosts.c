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

#include "lv-libssh2-knownhost-private.h"
#include "lv-libssh2-knownhosts-private.h"
#include "lv-libssh2-session-private.h"
#include "lv-libssh2-status-private.h"
#include "lv-libssh2.h"

lv_libssh2_status_t
lv_libssh2_knownhosts_create(lv_libssh2_session_t *session,
                             lv_libssh2_knownhosts_t **handle) {
  *handle = NULL;
  if (session == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  LIBSSH2_KNOWNHOSTS *inner = libssh2_knownhost_init(session->inner);
  if (inner == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_GENERIC;
  }
  lv_libssh2_knownhosts_t *knownhosts = malloc(sizeof(lv_libssh2_knownhosts_t));
  if (knownhosts == NULL) {
    libssh2_knownhost_free(inner);
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  knownhosts->inner = inner;
  *handle = knownhosts;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_knownhosts_destroy(lv_libssh2_knownhosts_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  libssh2_knownhost_free(handle->inner);
  handle->inner = NULL;
  free(handle);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_knownhosts_add(lv_libssh2_knownhosts_t *handle, const char *name,
                          const char *salt, const char *key,
                          const size_t key_len, const char *comment,
                          const size_t comment_len, const int type_mask,
                          lv_libssh2_knownhost_t *knownhost) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (knownhost == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result =
      libssh2_knownhost_addc(handle->inner, name, salt, key, key_len, comment,
                             comment_len, type_mask, &knownhost->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t lv_libssh2_knownhosts_type_mask(
    const lv_libssh2_knownhost_name_types_t type,
    const lv_libssh2_knownhost_key_encodings_t encoding,
    const lv_libssh2_knownhost_key_algorithms_t algorithm, int *type_mask) {
  *type_mask = 0;
  switch (type) {
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_PLAIN:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_TYPE_PLAIN;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_SHA1:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_TYPE_SHA1;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_CUSTOM:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_TYPE_CUSTOM;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE;
  }
  switch (encoding) {
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_RAW:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_KEYENC_RAW;
    break;
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_BASE64:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_KEYENC_BASE64;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING;
  }
  switch (algorithm) {
  case LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_RSA1:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_KEY_RSA1;
    break;
  case LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_SSHRSA:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_KEY_SSHRSA;
    break;
  case LV_LIBSSH2_KNOWNHOST_KEY_ALGORITHM_SSHDSS:
    *type_mask = *type_mask | LIBSSH2_KNOWNHOST_KEY_SSHDSS;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ALGORITHM;
  }
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_knownhosts_check(lv_libssh2_knownhosts_t *handle, const char *host,
                            const int port, const uint8_t *key,
                            const size_t key_len,
                            const lv_libssh2_knownhost_name_types_t type,
                            const lv_libssh2_knownhost_key_encodings_t encoding,
                            lv_libssh2_knownhosts_check_results_t *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (host == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (key == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (result == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int type_mask = 0;
  switch (type) {
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_PLAIN:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_PLAIN;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_SHA1:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_SHA1;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_CUSTOM:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_CUSTOM;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE;
  }
  switch (encoding) {
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_RAW:
    type_mask = type_mask | LIBSSH2_KNOWNHOST_KEYENC_RAW;
    break;
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_BASE64:
    type_mask = type_mask | LIBSSH2_KNOWNHOST_KEYENC_BASE64;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING;
  }
  int inner_result = libssh2_knownhost_checkp(
      handle->inner, host, port, (char *)key, key_len, type_mask, NULL);
  switch (inner_result) {
  case LIBSSH2_KNOWNHOST_CHECK_FAILURE:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_FAILURE;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_NOTFOUND:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_NOT_FOUND;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_MATCH:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MATCH;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_MISMATCH:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MISMATCH;
    break;
  default:
    return lv_libssh2_status_from_result(inner_result);
  }
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_knownhosts_check_and_get(
    lv_libssh2_knownhosts_t *handle, const char *host, const int port,
    const uint8_t *key, const size_t key_len,
    const lv_libssh2_knownhost_name_types_t type,
    const lv_libssh2_knownhost_key_encodings_t encoding,
    lv_libssh2_knownhost_t *known_host,
    lv_libssh2_knownhosts_check_results_t *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (host == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (key == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (known_host == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (result == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int type_mask = 0;
  switch (type) {
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_PLAIN:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_PLAIN;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_SHA1:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_SHA1;
    break;
  case LV_LIBSSH2_KNOWNHOST_NAME_TYPE_CUSTOM:
    type_mask = LIBSSH2_KNOWNHOST_TYPE_CUSTOM;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_NAME_TYPE;
  }
  switch (encoding) {
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_RAW:
    type_mask = type_mask | LIBSSH2_KNOWNHOST_KEYENC_RAW;
    break;
  case LV_LIBSSH2_KNOWNHOST_KEY_ENCODING_BASE64:
    type_mask = type_mask | LIBSSH2_KNOWNHOST_KEYENC_BASE64;
    break;
  default:
    return LV_LIBSSH2_STATUS_ERROR_UNKNOWN_KEY_ENCODING;
  }
  int inner_result =
      libssh2_knownhost_checkp(handle->inner, host, port, (char *)key, key_len,
                               type_mask, &known_host->inner);
  switch (inner_result) {
  case LIBSSH2_KNOWNHOST_CHECK_FAILURE:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_FAILURE;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_NOTFOUND:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_NOT_FOUND;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_MATCH:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MATCH;
    break;
  case LIBSSH2_KNOWNHOST_CHECK_MISMATCH:
    *result = LV_LIBSSH2_KNOWNHOSTS_CHECK_RESULT_MISMATCH;
    break;
  default:
    return lv_libssh2_status_from_result(inner_result);
  }
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_knownhosts_delete(lv_libssh2_knownhosts_t *handle,
                             lv_libssh2_knownhost_t *knownhost) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (knownhost == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_knownhost_del(handle->inner, knownhost->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_knownhosts_read_file(lv_libssh2_knownhosts_t *handle,
                                const char *file, size_t *count) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (file == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_knownhost_readfile(handle->inner, file,
                                          LIBSSH2_KNOWNHOST_FILE_OPENSSH);
  if (result < 0) {
    return lv_libssh2_status_from_result(result);
  }
  *count = result;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_knownhosts_read_line(lv_libssh2_knownhosts_t *handle,
                                const char *line, const size_t line_len) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (line == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_knownhost_readline(handle->inner, line, line_len,
                                          LIBSSH2_KNOWNHOST_FILE_OPENSSH);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_knownhosts_write_file(lv_libssh2_knownhosts_t *handle,
                                 const char *file) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (file == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_knownhost_writefile(handle->inner, file,
                                           LIBSSH2_KNOWNHOST_FILE_OPENSSH);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_knownhosts_write_line(lv_libssh2_knownhosts_t *handle,
                                 lv_libssh2_knownhost_t *knownhost, char *line,
                                 const size_t line_len, size_t *len) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (knownhost == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_knownhost_writeline(handle->inner, knownhost->inner,
                                           line, line_len, len,
                                           LIBSSH2_KNOWNHOST_FILE_OPENSSH);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_knownhosts_first(lv_libssh2_knownhosts_t *handle,
                            lv_libssh2_knownhost_t *host,
                            lv_libssh2_knownhosts_get_results_t *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (host == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (result == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int inner_result = libssh2_knownhost_get(handle->inner, &host->inner, NULL);
  if (inner_result == 0) {
    *result = LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_SUCCESS;
    return LV_LIBSSH2_STATUS_OK;
  } else if (inner_result == 1) {
    *result = LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_END_OF_HOSTS;
    return LV_LIBSSH2_STATUS_OK;
  } else {
    return lv_libssh2_status_from_result(inner_result);
  }
}

lv_libssh2_status_t lv_libssh2_knownhosts_next(
    lv_libssh2_knownhosts_t *handle, lv_libssh2_knownhost_t *prev,
    lv_libssh2_knownhost_t *next, lv_libssh2_knownhosts_get_results_t *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (prev == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (next == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (result == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int inner_result =
      libssh2_knownhost_get(handle->inner, &next->inner, prev->inner);
  if (inner_result == 0) {
    *result = LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_SUCCESS;
    return LV_LIBSSH2_STATUS_OK;
  } else if (inner_result == 1) {
    *result = LV_LIBSSH2_KNOWNHOSTS_GET_RESULT_END_OF_HOSTS;
    return LV_LIBSSH2_STATUS_OK;
  } else {
    return lv_libssh2_status_from_result(inner_result);
  }
}
