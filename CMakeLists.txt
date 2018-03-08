cmake_minimum_required(VERSION 3.9)
project(labssh2 VERSION 0.1.0 LANGUAGES C)

include(ExternalProject)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(EXTERNALS_PREFIX "externals")
set(OPENSSL "openssl")
set(LIBSSH2 "libssh2")

if(WIN32)
    if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        set(OPENSSL_CONFIGURE_COMMAND 
            ${CMAKE_CURRENT_BINARY_DIR}/${EXTERNALS_PREFIX}/src/${OPENSSL}/perl Configure VC-WIN64A
        ) 
    else()
        set(OPENSSL_CONFIGURE_COMMAND 
            ${CMAKE_CURRENT_BINARY_DIR}/${EXTERNALS_PREFIX}/src/${OPENSSL}/perl Configure VC-WIN32
        )
    endif()
else()
    set(OPENSSL_CONFIGURE_COMMAND 
        ${CMAKE_CURRENT_BINARY_DIR}/${EXTERNALS_PREFIX}/src/${OPENSSL}/config no-shared
    )
endif()

ExternalProject_Add(${OPENSSL}
    PREFIX ${EXTERNALS_PREFIX}
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG OpenSSL_1_1_0g
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_COMMAND}
    BUILD_IN_SOURCE YES
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${OPENSSL} BINARY_DIR)
set(OPENSSL_BINARY_DIR ${BINARY_DIR})

ExternalProject_Add(${LIBSSH2}
    PREFIX ${EXTERNALS_PREFIX}
    DEPENDS ${OPENSSL}
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG libssh2-1.8.0
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCRYPTO_BACKEND=OpenSSL -DOPENSSL_ROOT_DIR=${OPENSSL_BINARY_DIR}
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${LIBSSH2} BINARY_DIR)
set(LIBSSH2_BINARY_DIR ${BINARY_DIR}/src)

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

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

