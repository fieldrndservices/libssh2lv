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

#include <stdbool.h>
#include <stdlib.h>

#include "libssh2.h"

#include "labssh2.h"
#include "labssh2-knownhost-private.h"

labssh2_status_t
labssh2_knownhost_create(
    labssh2_knownhost_t** handle
) {
    *handle = NULL;
    struct libssh2_knownhost* inner = malloc(sizeof(struct libssh2_knownhost));
    if (inner == NULL) {
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    labssh2_knownhost_t* knownhost = malloc(sizeof(labssh2_knownhost_t));
    if (knownhost == NULL) {
        free(inner);
        return LABSSH2_STATUS_ERROR_MALLOC;
    }
    knownhost->inner = inner;
    *handle = knownhost;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_destroy(
    labssh2_knownhost_t* handle
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    free(handle->inner);
    handle->inner = NULL;
    free(handle);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_magic(
    labssh2_knownhost_t* handle,
    int* magic
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *magic = handle->inner->magic;
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_name_len(
    labssh2_knownhost_t* handle,
    size_t* len
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (handle->inner->name == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *len = strlen(handle->inner->name);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_name(
    labssh2_knownhost_t* handle,
    char* name
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (name == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (handle->inner->name == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    memcpy(name, handle->inner->name, strlen(handle->inner->name));
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_key_len(
    labssh2_knownhost_t* handle,
    size_t* len
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (handle->inner->key == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *len = strlen(handle->inner->key);
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_key(
    labssh2_knownhost_t* handle,
    char* key
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (handle->inner->key == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    if (key == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    memcpy(key, handle->inner->key, strlen(handle->inner->key));
    return LABSSH2_STATUS_OK;
}

labssh2_status_t
labssh2_knownhost_type_mask(
    labssh2_knownhost_t* handle,
    int* type_mask
) {
    if (handle == NULL) {
        return LABSSH2_STATUS_ERROR_NULL_VALUE;
    }
    *type_mask = handle->inner->typemask;
    return LABSSH2_STATUS_OK;
}

