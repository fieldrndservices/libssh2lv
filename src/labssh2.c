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

int
labssh2_init()
{
    return libssh2_init(0);
}

void
labssh2_exit()
{
    libssh2_exit();
}

int
labssh2_connect(char* server_ip)
{
    struct sockaddr_in sin;
    socklen_t sinlen;
#ifdef WIN32
    char sockopt;
    SOCKET sock = INVALID_SOCKET;
    SOCKET listensock = INVALID_SOCKET, forwardsock = INVALID_SOCKET;
    WSADATA wsadata;
    int err;
 
    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        return err;
    }
#else
    int sockopt, sock = -1;
    int listensock = -1, forwardsock = -1;
#endif    
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef WIN32
    if (sock == INVALID_SOCKET) {
        return -1;
    }
#else
    if (sock == -1) {
        perror("socket");
        return -1;
    }
#endif
    sin.sin_family = AF_INET;
    if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(server_ip))) {
        perror("inet_addr");
        return -1;
    }
    sin.sin_port = htons(22);
    if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
        return -1;
    }
}

