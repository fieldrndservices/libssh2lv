cmake_minimum_required(VERSION 3.9)
project(labssh2 VERSION 0.1.0 LANGUAGES C)

include(ExternalProject)

set(CMAKE_C_VISIBILITY_PRESET hidden)

# First for the generic no-config case (e.g. with mingw)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach(OUTPUT_CONFIG CMAKE_CONFIGURATION_TYPES)

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(OUTPUT_NAME ${CMAKE_PROJECT_NAME}-x64)
    set(OUTPUT_NAME ${CMAKE_PROJECT_NAME}-x64)
else()
    set(OUTPUT_NAME ${CMAKE_PROJECT_NAME})
endif()

add_definitions(
    -DVERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
)

function(find_static_library LIB_NAME OUT)
    if (WIN32 OR MSVC) 
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
    elseif (UNIX)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    endif()
    find_library(FOUND_${LIB_NAME}_STATIC ${LIB_NAME} PATHS ${ARGV2})
    if (FOUND_${LIB_NAME}_STATIC)
        get_filename_component(ABS_FILE ${FOUND_${LIB_NAME}_STATIC} ABSOLUTE)
    else()
        message(SEND_ERROR "Unable to find library ${LIB_NAME}, even at ${ARGV2}")
    endif()
    set(${OUT} ${ABS_FILE} PARENT_SCOPE)
endfunction()

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

