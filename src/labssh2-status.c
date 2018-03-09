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

#include "labssh2.h"

static const char* UNKNOWN_STATUS = "Unknown status";

int
labssh2_status_code(labssh2_status_t status) {
    switch (status) {
        case LABSSH2_STATUS_OK: return 0;
        case LABSSH2_STATUS_ERROR_OUT_OF_MEMORY: return -1;
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return -2;
        case LABSSH2_STATUS_ERROR_WSASTARTUP: return -3;                                     
        case LABSSH2_STATUS_ERROR_LIBSSH2_INITIALIZATION: return -4;
        case LABSSH2_STATUS_ERROR_CONNECTION: return -5;
        case LABSSH2_STATUS_ERROR_SESSION_INITIALIZATION: return -6;
        case LABSSH2_STATUS_ERROR_SESSION_ESTABLISHMEN: return -7;
        default: assert(UNKNOWN_STATUS);
    }
    return 1;
}

const char*
labssh2_status_string(labssh2_status_t status) {
    switch (status) {
        case LABSSH2_STATUS_OK: return "No Error";
        case LABSSH2_STATUS_ERROR_OUT_OF_MEMORY: return "Out of Memory Error";
        case LABSSH2_STATUS_ERROR_NULL_VALUE: return "Null Value Error";
        case LABSSH2_STATUS_ERROR_WSASTARTUP: return "WSAStartup Error";
        case LABSSH2_STATUS_ERROR_LIBSSH2_INITIALIZATION: "libssh2 Inititalization Error";
        case LABSSH2_STATUS_ERROR_CONNECTION: return "Connection Error";
        case LABSSH2_STATUS_ERROR_SESSION_INITIALIZATION: return "Session Initialization Error";
        case LABSSH2_STATUS_ERROR_SESSION_ESTABLISHMEN: return "Session Establishment Error";
        default: assert(UNKNOWN_STATUS);
    }
    return UNKNOWN_STATUS;
}

