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
target_link_libraries(shared ssh2)

