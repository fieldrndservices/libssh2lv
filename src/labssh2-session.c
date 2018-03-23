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

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libssh2.h"

#include "labssh2.h"
#include "labssh2-private.h"
#include "labssh2-status-private.h"
#include "labssh2-session-private.h"

static const char* NULL_SESSION = "The session cannot be NULL";

labssh2_session_t*
labssh2_session_create(
    labssh2_t* ctx
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return NULL;
    }
    LIBSSH2_SESSION* inner = libssh2_session_init_ex(NULL, NULL, NULL, NULL);
    if (inner != NULL) {
        labssh2_session_t* session = malloc(sizeof(labssh2_session_t));
        if (session != NULL) {
            session->inner = inner;
            return session;
        } else {
            libssh2_session_free(inner);
            ctx->status = LABSSH2_STATUS_ERROR_MEMORY;
            ctx->source = "malloc";
            ctx->message = "Could not allocate memory to create session";
            return NULL;
        }
    } else {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_session_init_ex";
        ctx->message = "Errors occurred during the creation of a new session";
        return NULL;
    }
}

void
labssh2_session_destroy(
    labssh2_t* ctx, 
    labssh2_session_t* session
) {
    assert(ctx);
    assert(session);
    libssh2_session_set_blocking(session->inner, LABSSH2_SESSION_MODE_BLOCKING);
    int result = libssh2_session_free(session->inner);
    if (result != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_session_free";
        ctx->message = "Errors occurred during the destruction of a session";
    }
    session->inner = NULL;
    free(session);
}

void 
labssh2_session_connect(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    uintptr_t socket
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_session_connect";
        ctx->message = NULL_SESSION;
        return;
    }
    int result = libssh2_session_handshake(session->inner, socket);
    if (result != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_session_handshake";
        ctx->message = labssh2_status_error_to_message(result);
    }
}

void 
labssh2_session_disconnect(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    const char* description
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_session_disconnect";
        ctx->message = NULL_SESSION;
        return;
    } 
    libssh2_session_set_blocking(session->inner, LABSSH2_SESSION_MODE_BLOCKING);
    libssh2_session_disconnect_ex(session->inner, SSH_DISCONNECT_BY_APPLICATION, description, "");
}

size_t
labssh2_session_hostkey_hash_len(
    labssh2_hostkey_hash_types_t type
) {
    switch (type) {
        case LABSSH2_HOSTKEY_HASH_TYPE_MD5: return 16;
        case LABSSH2_HOSTKEY_HASH_TYPE_SHA1: return 20;
        default:
          assert("Unknown host key hash algorithm");
    }
}

void 
labssh2_session_hostkey_hash(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    labssh2_hostkey_hash_types_t type, 
    uint8_t* buffer
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_session_hostkey_hash";
        ctx->message = NULL_SESSION;
        return;
    }
    const char* hash = libssh2_hostkey_hash(session->inner, type);
    if (hash == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_hostkey_hash";
        ctx->message = "Either the session has not yet been started up, or the requested hash algorithm was not available";
    } else {
        memcpy(buffer, hash, labssh2_session_hostkey_hash_len(type));
    }
}

size_t
labssh2_session_hostkey_len(
    labssh2_t* ctx, 
    labssh2_session_t* session
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return 0;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_session_hostkey_len";
        ctx->message = NULL_SESSION;
        return 0;
    }
    size_t len = 0;
    int type = 0;
    libssh2_session_hostkey(session->inner, &len, &type);
    return len;
}

void
labssh2_session_hostkey(
    labssh2_t* ctx, 
    labssh2_session_t* session, 
    uint8_t* buffer, 
    labssh2_hostkey_types_t* type
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_session_hostkey";
        ctx->message = NULL_SESSION;
        return;
    }
    size_t len = 0;
    int libssh2_type = 0;
    const char* hostkey = libssh2_session_hostkey(session->inner, &len, &libssh2_type);
    if (hostkey == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "labssh2_session_hostkey";
        ctx->message = "Something went wrong";
    } else {
        memcpy(buffer, hostkey, len);
        switch (libssh2_type) {
            case LIBSSH2_HOSTKEY_TYPE_RSA: *type = LABSSH2_HOSTKEY_TYPE_RSA; break;
            case LIBSSH2_HOSTKEY_TYPE_DSS: *type = LABSSH2_HOSTKEY_TYPE_DSS; break;
            default: *type = LABSSH2_HOSTKEY_TYPE_UNKNOWN;
        }
    }
}

