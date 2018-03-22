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
#include "labssh2-session-private.h"
#include "labssh2-status-private.h"
#include "labssh2-knownhosts-private.h"

static const char* NULL_KNOWNHOSTS = "The known hosts cannot be NULL";

labssh2_knownhosts_t*
labssh2_knownhosts_create(
    labssh2_t* ctx,
    labssh2_session_t* session
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return NULL;
    }
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_knownhosts_create";
        ctx->message = "The session cannot be NULL";
        return NULL;
    }
    LIBSSH2_KNOWNHOSTS* inner = libssh2_knownhost_init(session->inner);
    if (inner != NULL) {
        labssh2_knownhosts_t* knownhosts = malloc(sizeof(labssh2_knownhosts_t));
        if (knownhosts != NULL) {
            knownhosts->inner = inner;
            return knownhosts;
        } else {
            libssh2_knownhost_free(inner);
            ctx->status = LABSSH2_STATUS_ERROR_MEMORY;
            ctx->source = "malloc";
            ctx->message = "Could not allocate memory to create a known hosts";
            return NULL;
        }
    } else {
        ctx->status = LABSSH2_STATUS_ERROR_KNOWNHOSTS;
        ctx->source = "libssh2_knownhost_init";
        ctx->message = "Something went wrong";
        return NULL;
    }
}

void
labssh2_knownhosts_destroy(
    labssh2_t* ctx, 
    labssh2_knownhosts_t* knownhosts
) {
    libssh2_knownhost_free(knownhosts->inner);
    knownhosts->inner = NULL;
    free(knownhosts);
}

void
labssh2_knownhosts_next(
    labssh2_t* ctx,
    labssh2_knownhosts_t* knownhosts,
    labssh2_knownhost_t* knownhost
) {
    assert(ctx);
    if (labssh2_is_err(ctx)) {
        return;
    }
    if (knownhosts == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_NULL_VALUE;
        ctx->source = "labssh2_knownhosts_next";
        ctx->message = NULL_KNOWNHOSTS;
        return; 
    }
}

