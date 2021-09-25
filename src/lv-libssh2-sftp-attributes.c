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

#include <stdlib.h>

#include "libssh2_sftp.h"

#include "lv-libssh2-sftp-attributes-private.h"
#include "lv-libssh2-status-private.h"
#include "lv-libssh2.h"

lv_libssh2_status_t
lv_libssh2_sftp_attributes_create(lv_libssh2_sftp_attributes_t **handle) {
  *handle = NULL;
  lv_libssh2_sftp_attributes_t *attributes =
      malloc(sizeof(lv_libssh2_sftp_attributes_t));
  if (attributes == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  LIBSSH2_SFTP_ATTRIBUTES *inner = malloc(sizeof(LIBSSH2_SFTP_ATTRIBUTES));
  if (inner == NULL) {
    free(attributes);
    return LV_LIBSSH2_STATUS_ERROR_MALLOC;
  }
  attributes->inner = inner;
  *handle = attributes;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_destroy(lv_libssh2_sftp_attributes_t *handle) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  free(handle->inner);
  handle->inner = NULL;
  free(handle);
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_flags(lv_libssh2_sftp_attributes_t *handle,
                                 uint32_t *flags) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (flags == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *flags = handle->inner->flags;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_file_size(lv_libssh2_sftp_attributes_t *handle,
                                     uint64_t *file_size) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (file_size == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *file_size = handle->inner->filesize;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_uid(lv_libssh2_sftp_attributes_t *handle,
                               uint32_t *uid) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (uid == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *uid = handle->inner->uid;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_gid(lv_libssh2_sftp_attributes_t *handle,
                               uint32_t *gid) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (gid == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *gid = handle->inner->gid;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_permissions(lv_libssh2_sftp_attributes_t *handle,
                                       uint32_t *permissions) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (permissions == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *permissions = handle->inner->permissions;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_atime(lv_libssh2_sftp_attributes_t *handle,
                                 uint32_t *atime) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (atime == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *atime = handle->inner->atime;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_mtime(lv_libssh2_sftp_attributes_t *handle,
                                 uint32_t *mtime) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (mtime == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  *mtime = handle->inner->mtime;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_permissions(lv_libssh2_sftp_attributes_t *handle,
                                           const uint32_t permissions) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  handle->inner->permissions = permissions;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_uid(lv_libssh2_sftp_attributes_t *handle,
                                   const uint32_t uid) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  handle->inner->uid = uid;
  return LV_LIBSSH2_STATUS_OK;
}

lv_libssh2_status_t
lv_libssh2_sftp_attributes_set_gid(lv_libssh2_sftp_attributes_t *handle,
                                   const uint32_t gid) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  handle->inner->gid = gid;
  return LV_LIBSSH2_STATUS_OK;
}

LV_LIBSSH2_API lv_libssh2_status_t lv_libssh2_sftp_attributes_file_type(
    lv_libssh2_sftp_attributes_t *handle, lv_libssh2_file_types_t *type) {
  if (handle == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (type == NULL) {
    return LV_LIBSSH2_STATUS_ERROR_NULL_VALUE;
  }
  if (LIBSSH2_SFTP_S_ISLNK(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_SYMLINK;
  } else if (LIBSSH2_SFTP_S_ISREG(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_REGULAR;
  } else if (LIBSSH2_SFTP_S_ISDIR(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_DIRECTORY;
  } else if (LIBSSH2_SFTP_S_ISCHR(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_CHAR_DEVICE;
  } else if (LIBSSH2_SFTP_S_ISBLK(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_BLOCK_DEVICE;
  } else if (LIBSSH2_SFTP_S_ISFIFO(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_FIFO;
  } else if (LIBSSH2_SFTP_S_ISSOCK(handle->inner->permissions)) {
    *type = LV_LIBSSH2_FILE_TYPE_SOCKET;
  } else {
    *type = LV_LIBSSH2_FILE_TYPE_UNKNOWN;
  }
  return LV_LIBSSH2_STATUS_OK;
}
