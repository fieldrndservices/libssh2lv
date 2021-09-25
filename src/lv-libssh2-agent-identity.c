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
#include "lv-libssh2.h"

lv_libssh2_status_t
lv_libssh2_agent_identity_create(lv_libssh2_agent_identity_t **handle) {
  *handle = NULL;
  /*
    Do NOT allocate `inner` here. libssh2 will take care of this for us.
  */
  lv_libssh2_agent_identity_t *identity =
      malloc(sizeof(lv_libssh2_agent_identity_t));
  if (identity == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  identity->inner = NULL;
  *handle = identity;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_agent_identity_destroy(lv_libssh2_agent_identity_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  /*
    Do NOT free the `inner` here. libssh2 will take care of this for us...I
    think.
  */
  free(handle);
  handle = NULL;
  return LV_LIBSSH2_STATUS_OK;
}
