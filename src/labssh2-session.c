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

#include "libssh2.h"

#include "labssh2.h"
#include "labssh2-private.h"
#include "labssh2-session-private.h"

labssh2_session_t*
labssh2_session_create(labssh2_t* ctx)
{
    assert(ctx);
    LIBSSH2_SESSION* inner = libssh2_session_init_ex(NULL, NULL, NULL, NULL);
    if (inner != NULL) {
        labssh2_session_t* session = malloc(sizeof(labssh2_session_t));
        if (session != NULL) {
            session->inner = inner;
            return session;
        } else {
            libssh2_session_free(inner);
            ctx->status = LABSSH2_STATUS_ERROR_SESSION;
            ctx->source = "malloc";
            ctx->message = "Could not allocate memory to create session";
            return NULL;
        }
    } else {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_session_init_ex";
        ctx->message = "Errors occurred during creation of a new session";
        return NULL;
    }
}

void
labssh2_session_destroy(labssh2_t* ctx, labssh2_session_t* session)
{
    int result = 0;
    libssh2_session_set_blocking(session->inner, LABSSH2_SESSION_BLOCKING);
    result = libssh2_session_free(session->inner);
    if (result != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION;
        ctx->source = "libssh2_session_free";
        ctx->message = "Errors occurred during destruction of a session";
    }
    session->inner = NULL;
    free(session);
}

