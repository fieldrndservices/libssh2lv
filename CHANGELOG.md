# libssh2lv: Change Log

All notable changes to this project will be documented in this file, which is written in plain text [Github Flavored Markdown (GFM)](https://help.github.com/articles/github-flavored-markdown/) lightweight markup language. This project adheres to [Semantic Versioning](http://semver.org).

## [Unreleased]

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

