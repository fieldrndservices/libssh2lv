set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
    labssh2-session.c
)

add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES PREFIX "" OUTPUT_NAME ${OUTPUT_NAME})
add_dependencies(shared ${LIBSSH2})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared 
    ${LIBSSH2_ARCHIVE_DIR}/${LIBSSH2}${CMAKE_STATIC_LIBRARY_SUFFIX} 
    ${OPENSSL_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}
)

