set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
)

# Shared library build
find_library(LIBSSH2 libssh2.a)
find_library(LIBSSL libssl.a PATHS ${MACOS_OPENSSL_PATH})
find_library(LIBCRYPTO libcrypto.a PATHS ${MACOS_OPENSSL_PATH})
add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared ${LIBSSH2} ${LIBSSL} ${LIBCRYPTO})

