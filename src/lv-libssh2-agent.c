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

#include "lv-libssh2-agent-identity-private.h"
#include "lv-libssh2-agent-private.h"
#include "lv-libssh2-session-private.h"
#include "lv-libssh2-status-private.h"
#include "lv-libssh2.h"

lv_libssh2_status_t lv_libssh2_agent_create(lv_libssh2_session_t *session,
                                            lv_libssh2_agent_t **handle) {
  *handle = NULL;
  if (session == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  LIBSSH2_AGENT *inner = libssh2_agent_init(session->inner);
  if (inner == NULL) {
    return lv_libssh2_status_from_result(
        libssh2_session_last_errno(session->inner));
  }
  lv_libssh2_agent_t *agent = malloc(sizeof(lv_libssh2_agent_t));
  if (agent == NULL) {
    libssh2_agent_free(inner);
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  agent->inner = inner;
  *handle = agent;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_agent_destroy(lv_libssh2_agent_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  libssh2_agent_free(handle->inner);
  handle->inner = NULL;
  free(handle);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_agent_connect(lv_libssh2_agent_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_agent_connect(handle->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t lv_libssh2_agent_disconnect(lv_libssh2_agent_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_agent_disconnect(handle->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_agent_request_identities(lv_libssh2_agent_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_agent_list_identities(handle->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_agent_authenticate(lv_libssh2_agent_t *handle, const char *username,
                              lv_libssh2_agent_identity_t *identity) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (username == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (identity == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int result = libssh2_agent_userauth(handle->inner, username, identity->inner);
  return lv_libssh2_status_from_result(result);
}

lv_libssh2_status_t
lv_libssh2_agent_first_identity(lv_libssh2_agent_t *handle,
                                lv_libssh2_agent_identity_t *identity,
                                lv_libssh2_agent_identity_results_t *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (identity == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (result == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  int inner_result =
      libssh2_agent_get_identity(handle->inner, &identity->inner, NULL);
  if (inner_result == 0) {
    *result = LV_LIBSSH2_AGENT_IDENTITY_RESULT_SUCCESS;
    return LV_LIBSSH2_STATUS_OK;
  } else if (inner_result == 1) {
    *result = LV_LIBSSH2_AGENT_IDENTITY_RESULT_END_OF_IDENTITIES;
    return LV_LIBSSH2_STATUS_OK;
  } else {
    return lv_libssh2_status_from_result(inner_result);
  }
}

lv_libssh2_status_t
lv_libssh2_agent_next_identity(lv_libssh2_agent_t *handle,
                               lv_libssh2_agent_identity_t *prev,
                               lv_libssh2_agent_identity_t *next,
                               lv_libssh2_agent_identity_results_t *result) {
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
      libssh2_agent_get_identity(handle->inner, &next->inner, prev->inner);
  if (inner_result == 0) {
    *result = LV_LIBSSH2_AGENT_IDENTITY_RESULT_SUCCESS;
    return LV_LIBSSH2_STATUS_OK;
  } else if (inner_result == 1) {
    *result = LV_LIBSSH2_AGENT_IDENTITY_RESULT_END_OF_IDENTITIES;
    return LV_LIBSSH2_STATUS_OK;
  } else {
    return lv_libssh2_status_from_result(inner_result);
  }
}
