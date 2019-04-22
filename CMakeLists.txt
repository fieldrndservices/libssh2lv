cmake_minimum_required(VERSION 3.4)
project(LV-LIBSSH2 VERSION 0.1.0 LANGUAGES C)
set(PROJECT_URL "https://github.com/fieldrndservices/lv-libssh2")
set(PROJECT_DESCRIPTION "A LabVIEW-Friendly C library for LIBSSH2")

include(ExternalProject)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(OPENSSL "openssl")
set(LIBSSH2 "libssh2")
set(UNIX_LIBRARY_NAME "ssh2lv")
set(WINDOWS_LIBRARY_NAME "libssh2lv")

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
        set(DEPS_DIR ${PROJECT_SOURCE_DIR}/.deps/x64)
        set(OUTPUT_NAME "${WINDOWS_LIBRARY_NAME}-x64")
        set(OPENSSL_CONFIGURE_COMMAND perl ./Configure VC-WIN64A no-shared no-asm no-stdio)
        set(LIBSSH2_CMAKE_GENERATOR "Visual Studio 16 2019 Win64")
    else()
        set(DEPS_DIR ${PROJECT_SOURCE_DIR}/.deps/x86)
        set(OUTPUT_NAME ${WINDOWS_LIBRARY_NAME})
        set(OPENSSL_CONFIGURE_COMMAND perl ./Configure VC-WIN32 no-shared no-asm no-stdio)
        set(LIBSSH2_CMAKE_GENERATOR "Visual Studio 16 2019")
    endif()
    set(OPENSSL_BUILD_COMMAND nmake)
else()
    set(DEPS_DIR ${PROJECT_SOURCE_DIR}/.deps)
    set(OPENSSL_CONFIGURE_COMMAND ./config no-shared no-stdio)
    set(OPENSSL_BUILD_COMMAND ${MAKE})
    set(OUTPUT_NAME ${UNIX_LIBRARY_NAME})
endif()

ExternalProject_Add(${OPENSSL}
    PREFIX ${DEPS_DIR}/${OPENSSL}
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG OpenSSL_1_1_0j
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
    GIT_TAG libssh2-1.8.2
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
set(LIBSSH2_INCLUDE_DIR ${LIBSSH2_SOURCE_DIR}/include)
set(LIBSSH2_ARCHIVE_DIR ${LIBSSH2_BINARY_DIR}/src/$<CONFIGURATION>)

add_definitions(
    -DVERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
)

add_subdirectory(src)
enable_testing()
#add_subdirectory(tests)

