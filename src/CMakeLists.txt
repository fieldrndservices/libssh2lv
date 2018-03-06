set(SOURCE 
    labssh2.c
    labssh2.h
    labssh2-status.c
)

# Shared library build
add_library(shared SHARED ${SOURCE})
set_target_properties(shared PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(shared PRIVATE LABSSH2_BUILD_SHARED)
target_link_libraries(shared ssh2)

