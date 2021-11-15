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
#include "lv-libssh2-trace-private.h"
#include "lv-libssh2.h"

lv_libssh2_status_t
lv_libssh2_trace_node_create(lv_libssh2_trace_node_t **handle, const char *msg,
                             const size_t len) {
  *handle = NULL;
  lv_libssh2_trace_node_t *node = malloc(sizeof(lv_libssh2_trace_node_t));
  if (node == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  node->message = malloc(sizeof(char) * len);
  if (node->message == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  node->message_length = len;
  strncpy(node->message, msg, len);
  node->next = NULL;
  *handle = node;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_trace_node_destroy(lv_libssh2_trace_node_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  free(handle->message);
  handle->message_length = 0;
  handle->next = NULL;
  free(handle);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_trace_context_create(lv_libssh2_trace_context_t **handle) {
  *handle = NULL;
  lv_libssh2_trace_context_t *context =
      malloc(sizeof(lv_libssh2_trace_context_t));
  if (context == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  context->node_count = 0;
  context->head = NULL;
  context->last_handler_result = LV_LIBSSH2_STATUS_OK;
  *handle = context;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_trace_context_destroy(lv_libssh2_trace_context_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  lv_libssh2_trace_node_t *current = handle->head;
  lv_libssh2_trace_node_t *previous = NULL;
  while (current != NULL) {
    previous = current;
    current = previous->next;
    lv_libssh2_trace_node_destroy(previous);
    handle->node_count = handle->node_count - 1;
  }
  handle->head = NULL;
  free(handle);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_trace_write(lv_libssh2_trace_context_t *handle,
                                           const char *msg, size_t length) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  lv_libssh2_trace_node_t *node = NULL;
  lv_libssh2_status_t status = lv_libssh2_trace_node_create(&node, msg, length);
  if (lv_libssh2_status_is_err(status)) {
    return status;
  }
  if (handle->head == NULL) {
    handle->head = node;
  } else {
    lv_libssh2_trace_node_t *current = handle->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = node;
  }
  handle->node_count = handle->node_count + 1;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_trace_has_messages(lv_libssh2_session_t *handle,
                                                  bool *result) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *result = handle->trace_context->head == NULL;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_trace_message_len(lv_libssh2_session_t *handle,
                                                 size_t *len) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *len = handle->trace_context->head->message_length;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_trace_message(lv_libssh2_session_t *handle,
                                             uint8_t *buffer) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (buffer == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  memcpy(buffer, handle->trace_context->head->message,
         handle->trace_context->head->message_length);
  lv_libssh2_trace_node_t *read_node = handle->trace_context->head;
  handle->trace_context->head = read_node->next;
  return lv_libssh2_trace_node_destroy(read_node);
}

lv_libssh2_status_t
lv_libssh2_trace_last_handler_result(lv_libssh2_session_t *handle) {
  return handle->trace_context->last_handler_result;
}

void lv_libssh2_trace_handler(LIBSSH2_SESSION *session, void *context,
                              const char *msg, size_t length) {
  lv_libssh2_trace_context_t *handle = (lv_libssh2_trace_context_t *)context;
  lv_libssh2_trace_node_t *node = NULL;
  handle->last_handler_result =
      lv_libssh2_trace_node_create(&node, msg, length);
  if (handle->head == NULL) {
    handle->head = node;
  } else {
    lv_libssh2_trace_node_t *current = handle->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = node;
  }
  handle->node_count = handle->node_count + 1;
}

lv_libssh2_status_t lv_libssh2_trace_begin(lv_libssh2_session_t *session,
                                           int bitmask) {
  if (session == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  libssh2_trace(session->inner, bitmask);
  lv_libssh2_status_t status =
      lv_libssh2_trace_context_create(&session->trace_context);
  if (lv_libssh2_status_is_err(status)) {
    return status;
  }
  int result = libssh2_trace_sethandler(session->inner, session->trace_context,
                                        lv_libssh2_trace_handler);
  if (result != 0) {
    lv_libssh2_trace_context_destroy(session->trace_context);
    return lv_libssh2_status_from_result(result);
  }
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t lv_libssh2_trace_end(lv_libssh2_session_t *session) {
  return lv_libssh2_trace_context_destroy(session->trace_context);
}
