cmake_minimum_required(VERSION 3.10)
project(labssh2 VERSION 0.1.0 LANGUAGES C)
set(PROJECT_URL "https://github.com/fieldrndservices/labssh2-c")
set(PROJECT_DESCRIPTION "A LabVIEW-Friendly C library for libssh2")

include(ExternalProject)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(DEPS_DIR ${PROJECT_SOURCE_DIR}/.deps)
set(OPENSSL "openssl")
set(LIBSSH2 "libssh2")

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

if(WIN32)
    if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        set(OUTPUT_NAME ${CMAKE_PROJECT_NAME}-x64)
        set(OPENSSL_CONFIGURE_COMMAND perl ./Configure VC-WIN64A no-shared no-asm no-stdio)
        set(LIBSSH2_CMAKE_GENERATOR "Visual Studio 15 2017 Win64")
    else()
        set(OUTPUT_NAME ${CMAKE_PROJECT_NAME})
        set(OPENSSL_CONFIGURE_COMMAND perl ./Configure VC-WIN32 no-shared no-asm no-stdio)
        set(LIBSSH2_CMAKE_GENERATOR "Visual Studio 15 2017 Win64")
    endif()
    set(OPENSSL_BUILD_COMMAND nmake)
else()
    set(OPENSSL_CONFIGURE_COMMAND ./config no-shared no-stdio)
    set(OPENSSL_BUILD_COMMAND ${MAKE})
    set(OUTPUT_NAME ${CMAKE_PROJECT_NAME})
endif()

ExternalProject_Add(${OPENSSL}
    PREFIX ${DEPS_DIR}/${OPENSSL}
    URL https://github.com/openssl/openssl/archive/OpenSSL_1_1_0g.zip
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_COMMAND}
    BUILD_IN_SOURCE YES
    BUILD_COMMAND ${OPENSSL_BUILD_COMMAND}
    TEST_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${OPENSSL} BINARY_DIR)
set(OPENSSL_BINARY_DIR ${BINARY_DIR})

ExternalProject_Add(${LIBSSH2}
    PREFIX ${DEPS_DIR}/${LIBSSH2}
    DEPENDS ${OPENSSL}
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG libssh2-1.8.0
    UPDATE_DISCONNECTED YES
    UPDATE_COMMAND ""
    CMAKE_GENERATOR ${LIBSSH2_CMAKE_GENERATOR}
    CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DCRYPTO_BACKEND=OpenSSL -DOPENSSL_ROOT_DIR=${OPENSSL_BINARY_DIR}
    BUILD_IN_SOURCE NO
    TEST_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(${LIBSSH2} BINARY_DIR)
set(LIBSSH2_BINARY_DIR ${BINARY_DIR})
ExternalProject_Get_Property(${LIBSSH2} SOURCE_DIR)
set(LIBSSH2_SOURCE_DIR ${SOURCE_DIR})
set(LIBSSH2_ARCHIVE_DIR ${LIBSSH2_BINARY_DIR}/src)
set(LIBSSH2_INCLUDE_DIR ${LIBSSH2_SOURCE_DIR}/include)

add_definitions(
    -DVERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
)

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

