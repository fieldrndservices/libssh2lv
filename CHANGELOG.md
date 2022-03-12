# libssh2lv: Change Log

All notable changes to this project will be documented in this file, which is written in plain text [Github Flavored Markdown (GFM)](https://help.github.com/articles/github-flavored-markdown/) lightweight markup language. This project adheres to [Semantic Versioning](http://semver.org).

## [Unreleased]

## [0.2.4] - 2022-03-12

### Changed

- The OpenSSL dependency from v1.1.1l to v3.0.1

## [0.2.3] - 2021-10-18

### Added

- The `lv_libssh2_trace` function
- The `lv_libssh2_trace_options_t` enum type definition

### Changed

- The libssh2 dependency from v1.9.0 to v1.10.0
- The OpenSSL dependency from v1.1.1f to v1.1.1l

## [0.2.2] - 2021-09-25

### Added

- Strawberry Perl as an option for Windows builds

### Changed

- Code formatting style to ccls (clang-based LSP) default format

### Fixed

- Error handling for the `lv_libssh2_scp_receive` function
- Error handling for the `lv_libssh2_scp_send` function

## [0.2.1] - 2020-03-31

### Changed

- The libssh2 dependency from v1.8.2 to v1.9.0
- The OpenSSL dependency from v1.1.0j to v1.1.1f

## [0.2.0] - 2019-07-13

### Added

- All libssh2 library macro definitions as enum values where appropriate

### Changed

- Enum variant values to match values from the libssh2 library macro definitions

## [0.1.2] - 2019-05-28

### Added

- The `overwrite` parameter to the `lv_libssh2_sftp_file_rename` function

### Fixed

- Error handling for SFTP API

## [0.1.1] - 2019-05-26

### Added

- Getter function to SFTP attributes for the file type

## [0.1.0] - 2019-05-08

### Added

- CMake-based build procedure
- Dependency management via CMake ExternalProject module
- Build instructions for Windows, macOS, Linux, and NI Linux RT
- Initial public Application Programming Interface (API)
- Tests and [minunit](https://github.com/siu/minunit) testing framework
- The LICENSE file
- The README file
- The ignore list
