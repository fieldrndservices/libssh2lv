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
#include <libssh2.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif

#include "labssh2-private.h"

static labssh2_t OUT_OF_MEMORY_CONTEXT = {
    NULL,                                            // Hostname
    0,                                               // Hostname length
    NULL,                                            // Username
    0,                                               // Username length
    LABSSH2_STATUS_ERROR_OUT_OF_MEMORY,              // Status
    "Not enough memory available to create context", // Status message
    -1,                                              // Error code
}

static void
labssh2_init(labssh2_t* ctx)
{
    assert(ctx);
    ctx->hostname = NULL;
    ctx->hostname_len = 0;
    ctx->username = NULL;
    ctx->username_len = 0;
    ctx->status = LABSSH2_STATUS_OK;
    ctx->status_message = labssh2_status_string(ctx->status);
    ctx->error_code = 0;
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
    return context
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

uint32_t
labssh2_hostname_len(labssh2_t* ctx)
{
    assert(ctx);
    return (uint32_t)ctx->hostname_len;
}

void
labssh2_hostname(labssh2_t* ctx, char* hostname)
{
    assert(ctx);
    memcpy(hostname, ctx->hostname, ctx->hostname_len);
}

uint32_t
labssh2_username_len(labssh2_t* ctx)
{
    assert(ctx);
    return (uint32_t)ctx->username_len;
}

void
labssh2_username(labssh2_t* ctx, char* username)
{
    assert(ctx);
    memcpy(username, ctx->username, ctx->username_len);
}

void
labssh2_connect(labssh2_t* ctx)
{
    const char *commandline = "uptime";
    const char *password    = "password";
    unsigned long host_addr;
    int sock;
    struct sockaddr_in sin;
    const char *fingerprint;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int rc;
    int exitcode;
    char *exitsignal=(char *)"none";
    int bytecount = 0;
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;
    int type;
#ifdef WIN32
    WSADATA wsadata;
    int err;
 
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_WSASTARTUP;
        ctx->status_message = "WSAStartup failed, check the cause for the 'WSAStartup' function";
        ctx->status_cause = err;
        return;
    }
#endif
    rc = libssh2_init(0);
    if (rc != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_LIBSSH2_INITIALIZATION;
        ctx->status_message = "libssh2 initialization failed, check the cause for the 'libssh2_init' function";
        ctx->status_cause = rc;
        return;
    }
    host_addr = inet_addr(ctx->hostname);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = host_addr;
    int rc = connect(
        sock, 
        (struct sockaddr*)(&sin),
        sizeof(struct sockaddr_in)
    ); 
    if (rc != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_CONNECTION;
        ctx->status_message = "Failed to connect, check the cause for the 'connect' function";
        ctx->status_cause = rc;
        return;
    }
    /* Create a session instance */ 
    session = libssh2_session_init();
    if (session == NULL) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION_INITIALIZATION;
        ctx->status_message = "Failed to initialize the session";
        ctx->status_cause = 0;
        return;
    }
    /* tell libssh2 we want it all done non-blocking */ 
    libssh2_session_set_blocking(session, 0);
    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */ 
    while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);
    if (rc != 0) {
        ctx->status = LABSSH2_STATUS_ERROR_SESSION_ESTABLISHMENT;
        ctx->status_message = "Failed to establish a SSH session, check the cause for the 'libssh2_session_handshake' function";
        ctx->status_cause = rc;
        return
    }
    nh = libssh2_knownhost_init(session);
    if (!nh) {
        // TODO: Add freeing/closing session
        // TODO: Add error handling
        return 2;
    }
    /* read all hosts from here */ 
    libssh2_knownhost_readfile(
        nh, 
        "known_hosts",
        LIBSSH2_KNOWNHOST_FILE_OPENSSH
    );
    /* store all known hosts to here */ 
    libssh2_knownhost_writefile(
        nh, 
        "dumpfile",
        LIBSSH2_KNOWNHOST_FILE_OPENSSH
    );
    fingerprint = libssh2_session_hostkey(session, &len, &type);
    if (fingerprint) {
        struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(
            nh, 
            hostname, 
            22,
            fingerprint, 
            len,
            LIBSSH2_KNOWNHOST_TYPE_PLAIN|
            LIBSSH2_KNOWNHOST_KEYENC_RAW,
            &host
        );
        fprintf(
            stderr, 
            "Host check: %d, key: %s\n", 
            check,
            (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH) ? host->key : "<none>"
        );
        /*****
         * At this point, we could verify that 'check' tells us the key is
         * fine or bail out.
         *****/ 
    } else {
        /* eeek, do cleanup here */ 
        return 3;
    }
    libssh2_knownhost_free(nh);
    if (strlen(password) != 0) {
        /* We could authenticate via password */ 
        while ((rc = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
        if (rc) {
            fprintf(stderr, "Authentication by password failed.\n");
            goto shutdown;
        }
    }
    else {
        /* Or by public key */ 
        while ((rc = libssh2_userauth_publickey_fromfile(
                session, 
                username,
                "/home/user/" // TODO: Change to context field
                ".ssh/id_rsa.pub", // TODO: Change to context field
                "/home/user/" // TODO: Change to context field
                ".ssh/id_rsa", // TODO: Change to context field
                password
            )) == LIBSSH2_ERROR_EAGAIN);
        if (rc != 0) {
            // TODO: Add error handling
            fprintf(stderr, "\tAuthentication by public key failed\n");
            goto shutdown;
        }
    }
    /* Exec non-blocking on the remove host */ 
    while ((channel = libssh2_channel_open_session(session)) == NULL && libssh2_session_last_error(session,NULL,NULL,0) == 
           LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
    }
    if (channel == NULL) {
        fprintf(stderr,"Error\n");
        exit(1);
    }
    while ((rc = libssh2_channel_exec(channel, commandline)) == LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
    }
    if (rc != 0) {
        fprintf(stderr,"Error\n");
        exit(1);
    }
    for ( ;; ) {
        /* loop until we block */ 
        int rc;
        do {
            char buffer[0x4000];
            rc = libssh2_channel_read(channel, buffer, sizeof(buffer));
            if (rc > 0) {
                int i;
                bytecount += rc;
                fprintf(stderr, "We read:\n");
                for (i = 0; i < rc; ++i) {
                    fputc( buffer[i], stderr);
                }
                fprintf(stderr, "\n");
            } else {
                if (rc != LIBSSH2_ERROR_EAGAIN) {
                    /* no need to output this for the EAGAIN case */ 
                    fprintf(stderr, "libssh2_channel_read returned %d\n", rc);
                }
            }
        }
        while (rc > 0);
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */ 
        if (rc == LIBSSH2_ERROR_EAGAIN) {
            waitsocket(sock, session);
        }
        else {
            break;
        }
    }
    exitcode = 127;
    while((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
    }
    if(rc == 0) {
        exitcode = libssh2_channel_get_exit_status(channel);
        libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
    }
    if (exitsignal)
        fprintf(stderr, "\nGot signal: %s\n", exitsignal);
    else
        fprintf(stderr, "\nEXIT: %d bytecount: %d\n", exitcode, bytecount);
    libssh2_channel_free(channel);
    channel = NULL;
shutdown:
    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    libssh2_exit();
    return 0;
}
