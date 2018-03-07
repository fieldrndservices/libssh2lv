set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
)

# Shared library build
find_static_library(ssh2 LIBSSH2 ${LIBSSH2_ROOT_DIR}/bin)
find_static_library(ssl LIBSSL ${OPENSSL_ROOT_DIR}/lib)
find_static_library(crypto LIBCRYPTO ${OPENSSL_ROOT_DIR}/lib)
add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared ${LIBSSH2} ${LIBSSL} ${LIBCRYPTO})

