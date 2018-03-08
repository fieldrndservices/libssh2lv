set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
)

# Shared library build
find_library(LIBSSH2 NAMES libssh2.a ssh2 PATHS ${LIBSSH2_ROOT_DIR})
find_library(LIBSSL NAMES libssl.a ssl PATHS ${OPENSSL_ROOT_DIR})
find_library(LIBCRYPTO NAMES libcrypto.a crypto PATHS ${OPENSSL_ROOT_DIR})
add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared ${LIBSSH2} ${LIBSSL} ${LIBCRYPTO})

