set(SOURCES
    global.c
    status.c
    version.c
)

include_directories(${labssh2_SOURCE_DIR}/src)
link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

foreach(SOURCE ${SOURCES})
    get_filename_component(NAME ${SOURCE} NAME_WE)
    add_executable(${NAME} ${SOURCE})
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests)
    target_link_libraries(${NAME} ${OUTPUT_NAME})
    add_dependencies(${NAME} shared)
    add_test(NAME ${NAME} COMMAND ${NAME})
endforeach(SOURCE)

