set(SOURCE
    lv-libssh2.c
    lv-libssh2.h
    lv-libssh2-agent.c
    lv-libssh2-agent-identity.c
    lv-libssh2-channel.c
    lv-libssh2-fileinfo.c
    lv-libssh2-knownhost.c
    lv-libssh2-knownhosts.c
    lv-libssh2-scp.c
    lv-libssh2-session.c
    lv-libssh2-sftp.c
    lv-libssh2-sftp-attributes.c
    lv-libssh2-status.c
    lv-libssh2-userauth.c)

add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME} SOVERSION ${ABI_MAJOR_VERSION} VERSION ${ABI_VERSION})
add_dependencies(shared ${LIBSSH2})
target_compile_definitions(shared PRIVATE LV_LIBSSH2_BUILD_SHARED)
target_include_directories(shared PRIVATE ${LIBSSH2_INCLUDE_DIR})
if(WIN32)
    # The `ws2_32.lib` is not included automatically with the rest of the
    # Windows SDK libraries (kernal32.lib, etc.). Symbols from this library are
    # needed by libssh2 and libcrypto, which are not included in the static
    # libraries.
    target_link_libraries(shared
        ${LIBSSH2_ARCHIVE_DIR}/${LIBSSH2}${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${OPENSSL_BINARY_DIR}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}
        ws2_32)
else()
  if(BUILD_DEPS)
    target_link_libraries(shared
        ${LIBSSH2_ARCHIVE_DIR}/${LIBSSH2}${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${OPENSSL_BINARY_DIR}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX})
  else()
    target_link_libraries(shared ssh2 crypto)
  endif()
endif()

