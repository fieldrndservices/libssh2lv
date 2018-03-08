cmake_minimum_required(VERSION 3.9)
project(labssh2 VERSION 0.1.0 LANGUAGES C)

include(ExternalProject)

set(CMAKE_C_VISIBILITY_PRESET hidden)

ExternalProject_Add(OPENSSL
    PREFIX openssl
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG OpenSSL_1_1_0g
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/openssl/src/OPENSSL/config no-shared
    BUILD_IN_SOURCE YES
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(OPENSSL BINARY_DIR)
set(OPENSSL_ROOT_DIR ${BINARY_DIR})

ExternalProject_Add(LIBSSH2
    PREFIX libssh2
    DEPENDS OPENSSL
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG libssh2-1.8.0
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCRYPTO_BACKEND=OpenSSL -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(LIBSSH2 BINARY_DIR)
set(LIBSSH2_ROOT_DIR ${BINARY_DIR}/src)

if (WIN32 OR MSVC) 
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
elseif (UNIX)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

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

