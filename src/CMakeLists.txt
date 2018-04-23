set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-fileinfo.c
    labssh2-knownhost.c
    labssh2-knownhosts.c
    labssh2-scp.c
    labssh2-session.c
    labssh2-status.c
    labssh2-userauth.c
)

add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
add_dependencies(shared ${LIBSSH2})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_include_directories(shared PRIVATE ${LIBSSH2_INCLUDE_DIR})
if(WIN32)
    # The `ws2_32.lib` is not included automatically with the rest of the
    # Windows SDK libraries (kernal32.lib, etc.). Symbols from this library are
    # needed by libssh2 and libcrypto, which are not included in the static
    # libraries. 
    target_link_libraries(shared
        ${LIBSSH2_ARCHIVE_DIR}/${LIBSSH2}${CMAKE_STATIC_LIBRARY_SUFFIX} 
        ${OPENSSL_BINARY_DIR}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}
        ws2_32
    )
else()
    target_link_libraries(shared 
        ${LIBSSH2_ARCHIVE_DIR}/${LIBSSH2}${CMAKE_STATIC_LIBRARY_SUFFIX} 
        ${OPENSSL_BINARY_DIR}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
endif()

