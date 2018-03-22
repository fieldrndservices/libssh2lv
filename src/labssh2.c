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

static labssh2_t OUT_OF_MEMORY_CONTEXT = {
    LABSSH2_STATUS_ERROR_OUT_OF_MEMORY,              // Status
    "Not enough memory available to create context", // Status message
    -1                                               // Status cause
};

static void
labssh2_init(labssh2_t* ctx)
{
    assert(ctx);
    ctx->status = LABSSH2_STATUS_OK;
    ctx->status_message = labssh2_status_string(ctx->status);
    ctx->status_cause = 0;
}

const char*
labssh2_version()
{
    return VERSION;
}

unsigned int
labssh2_version_major()
{
    return VERSION_MAJOR;
}

unsigned int
labssh2_version_minor()
{
    return VERSION_MINOR;
}

unsigned int
labssh2_version_patch()
{
    return VERSION_PATCH;
}

labssh2_t*
labssh2_create()
{
    labssh2_t* context = malloc(sizeof(labssh2_t));
    if (context == NULL) {
        return &OUT_OF_MEMORY_CONTEXT;
    }
    labssh2_init(context);
    return context;
}

void
labssh2_destroy(labssh2_t* ctx)
{
    free(ctx);
}

labssh2_status_t
labssh2_status(labssh2_t* ctx)
{
    assert(ctx);
    return ctx->status;
}

const char*
labssh2_status_message(labssh2_t* ctx)
{
    assert(ctx);
    return ctx->status_message;
}

int
labssh2_status_cause(labssh2_t* ctx)
{
    assert(ctx);
    return ctx->status_cause;
}

bool
labssh2_is_ok(labssh2_t* ctx)
{
    assert(ctx);
    return labssh2_status(ctx) == LABSSH2_STATUS_OK;
}

