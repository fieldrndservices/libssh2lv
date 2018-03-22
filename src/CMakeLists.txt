set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
)

find_library(LIBSSH2 NAMES libssh2.a libssh2.lib ssh2 PATHS ${LIBSSH2_LIBRARY_DIR} ${LIBSSH2_BINARY_DIR})
add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared ${LIBSSH2})
include_directories(${LIBSSH2_INCLUDE_DIR})

