# LV-LIBSSH2-C: A LabVIEW-Friendly C library for libssh2

The LV-LIBSSH2-C project is a [LabVIEW&trade;](http://www.ni.com/labview)-friendly C "wrapper" library for the [libssh2](http://libssh2.org) library. The library is intended to be used with the [Call Library Function](http://zone.ni.com/reference/en-XX/help/371361P-01/glang/call_library_function/) node. This provides SSH client functionality to LabVIEW as a <abbr title="Dynamic Link Library">DLL</abbr> (Windows), <abbr title="Dynamic Library">Dylib</abbr> (macOS), and/or <abbr title="Shared Object">SO</abbr> (Linux).

[Installation](#installation) | [Build](#build) | [API](https://fieldrndservices.github.io/lv-libssh2-c/) | [Tests](#tests) | [License](#license)

## Installation

A single ZIP archive containing the pre-compiled/built shared libraries for all of the platforms listed in the [Build](#build) section is provided with each [release](https://github.com/fieldrndservices/lv-libssh2/releases). These pre-compiled/built shared libraries include software developed by the OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.rog).

1. Download the ZIP archive for the latest release. Note, this is _not_ the source code ZIP file. The ZIP archive containing the pre-compiled/built shared libraries will be labeled: `lv-libssh2-c_#.#.#.zip`, where `#.#.#` is the version number for the release.
2. Extract, or unzip, the ZIP archive.
3. Copy and paste all or the platform-specific shared libraries to one of the following locations on disk:

| Platform    | Destination           |
|-------------|-----------------------|
| Windows     | `C:\Windows\System32` |
| macOS       | `/usr/local/lib`      |
| Linux       | `/usr/local/lib`      |
| NI Linux RT | `/usr/local/lib`      |

## Build

Ensure all of the following dependencies are installed and up-to-date before proceeding:

- [CMake 3.10.x](https://cmake.org/), or newer
- [Microsoft Visual C++ Build Tools 2017](https://www.visualstudio.com/downloads/#build-tools-for-visual-studio-2017), Windows Only
- [XCode Command Line Tools](https://developer.apple.com/xcode/features/), macOS Only
- [Git](https://git-scm.com/)
- [C/C++ Development Tools for NI Linux Real-Time, Eclipse Edition 2017](http://www.ni.com/download/labview-real-time-module-2017/6731/en/), NI Linux RT only
- [Doxygen](http://www.doxygen.org), Documentation only
- [ActivePerl](https://www.perl.org/), Windows Only 

The [OpenSSL](https://www.openssl.org/) and [libssh2](https://www.libssh2.org/) dependencies will automatically be downloaded and built as part of the build process. This is to ensure the correct version is used and that these dependencies are statically linked to the eventual LV-LIBSSH2-C shared library. Statically linking the dependencies avoids having to distribute separate shared libraries for OpenSSL and libssh2 on each of the supported desktop platform (Windows, macOS, and Linux). 

For NI Linux RT, the libssh2 shared library must be installed on the platform. The `libssh2lv` shared library is _not_ statically compiled with the libssh2 dependencies for the NI Linux RT operating system. Instead, a [libssh2 package is available from a sibling project](https://github.com/fieldrndservices/libssh2-nilrt-ipk) that can be installed using the [opkg](http://www.ni.com/en-us/innovations/white-papers/13/under-the-hood-of-ni-linux-real-time.html#section--222824847) package manager that is shipped with the NI Linux RT operating system.

The first build of the project on a system will take longer because the OpenSSL and libssh2 libraries will be downloaded and built. These dependencies can be found in the `.deps` folder.

### Windows

The [Microsoft Visual C++ Build Tools 2019](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16) should have installed a `x64 Native Build Tools` command prompt. Start the `x64 Native Build Tools` command prompt. This ensures the appropriate C compiler is available to CMake to build the library. Run the following commands to obtain a copy of the source code and build both the 32-bit and 64-bit DLLs with a `Release` configuration:

    C:\> git clone https://github.com/fieldrndservices/lv-libssh2-c.git
    C:\> cd lv-libssh2-c
    C:\> build.bat

The DLLs (libssh2lv.dll and libssh2lv-x64.dll) will be available in the `build32\bin` and `build64\bin` folders. 

### macOS

Ensure the command-line tools for [XCode](https://developer.apple.com/xcode/) have been installed along with [git](https://git-scm.com/) before proceeding.

    $ git clone https://github.com/fieldrndservices/lv-libssh2-c.git
    $ cd lv-libssh2-c
    $ mkdir build && cd build
    $ cmake ..
    $ cmake --build .

The dynamic library (libssh2lv.dylib) will be available in the `bin` folder within the `build` folder.

### Linux

If running on Ubuntu or similar distribution, ensure the [build-essential](https://packages.ubuntu.com/trusty/build-essential), [cmake](https://packages.ubuntu.com/trusty/cmake), and [git](https://packages.ubuntu.com/trusty/git) packages are installed before proceeding. These can be installed with the following command from a terminal:

    $ sudo apt-get install build-essential cmake git

Start a terminal, and run the following commands to obtain a copy of the source code from the repository and build the shared object (so):

    $ git clone https://github.com/fieldrndservices/lv-libssh2-c.git
    $ cd lv-libssh2-c
    $ mkdir build && cd build
    $ cmake ..
    $ cmake --build .

The shared object (libssh2lv.so) will be available in the `bin` folder.

### NI Linux RT

NI provides a cross-compiler for their Real-Time (RT) Linux distribution. Before proceeding, download and install the [C/C++ Development Tools for NI Linux Real-Time, Eclipse Edition 2017](http://www.ni.com/download/labview-real-time-module-2017/6731/en/). It is also best to review the [Getting Stared with C/C++ Development Tools for NI Linux Real-Time, Eclipse Edition](http://www.ni.com/tutorial/14625/en/) guide for more general information about configuring the internal builder.

1. Start NI Eclipse. A _Workspace Launcher_ dialog may appear. Use the default.
2. A welcome screen may appear after the application has loaded. Close the welcome screen.
3. Right-click in the _Project Explorer_ on the left and select _Import_ from the context menu that appears. A new dialog will appear.
4. Select `Git->Projects from Git` from the dialog that appears. Click the _Next >_ button. A new page will appear.
5. Select the `Clone URI` from the list that appears in the new page of the dialog. Click the _Next >_ button. A new page will appear.
6. Enter the URI for the lv-libssh2-c git repository in the _URI:_ field, i.e. `https://github.com/fieldrndservices/lv-libssh2-c.git`. The _Host:_ and _Repository path:_ fields will populate automatically. Click the _Next >_ button. A new page will appear.
7. Ensure only the `master` checkbox is checked in the _Branch Selection_ page of the _Import Projects from Git_ dialog. Click the _Next >_ button. A new page will appear.
8. Browse to the workspace directory for NI Eclipse to populate the _Directory:_ field. Leave all other fields as the defaults. Click the _Next >_ button. A new page will appear.
9. Select the `Import existing projects` radio button from the options under the _Wizard for project import_ section. Click the _Next >_ button. A new page will appear.
10. Click the _Finish_ button. No changes are needed on the _Import Projects_ page. A new `lv-libssh2-c` project should appear in the _Project Explorer_.
11. Right-click in the _Project Explorer_ on the left and select _Import_ from the context menu that appears. A new dialog will appear.
12. Select `Git->Projects from Git` from the dialog that appears. Click the _Next >_ button. A new page will appear.
13. Select the `Clone URI` from the list that appears in the new page of the dialog. Click the _Next >_ button. A new page will appear.
14. Enter the URI for the libssh2 git repository in the _URI:_ field, i.e. `https://github.com/libssh2/libssh2.git`. The _Host:_ and _Repository path:_ fields will populate automatically. Click the _Next >_ button. A new page will appear.
15. Ensure only the `master` checkbox is checked in the _Branch Selection_ page of the _Import Projects from Git_ dialog. Click the _Next >_ button. A new page will appear.
16. Browse to the workspace directory for NI Eclipse to populate the _Directory:_ field. Leave all other fields as the default. Click the _Next >_ button. A new page will appear.
17. Select the `Use the New Project wizard` radio button from the options under the _Wizard for project import_ section. Click the _Finish_ button. A new dialog will appear.
18. Select `C Project` from the _Wizards_ section in the dialog. Click the _Next >_ button. A new page will appear.
19. Enter `libssh2` for the _Project name:_. Click the _Next >_ button. A new page will appear.
20. Click the _Next >_ button. Do not change any of the defaults. A new page will appear.
21. Click the _Finish_ button. All dialogs will disappear and the `libssh2` project should appear in the _Project Explorer_ along with the `lv-libssh2-c` project.
22. Click on the `libssh2` project in the _Project Explorer.
23. Right-click on the `libssh2` project and select `Team->Switch To->Other`. A new dialog will appear.
24. Expand the `Tags` item in the tree that appears in the dialog.
25. Highlight the `libssh2-1.8.2` item and click the _Checkout_ button. An information dialog will appear. Click the _OK_ button. This ensures the correct version of the libssh2 dependency is used to build the lv-libssh2-c project.
26. Click on the `lv-libssh2-c` project in the _Project Explorer.
27. Click the _Build_ toolbar button (icon is a small hammer) to build the NI Linux RT x86_64-based shared object (so).
28. Click the drop-down menu next to the _Build_ toolbar button and select the `ARM` build configuration. This will build the NI Linux RT ARM-based shared object (so).

Note, steps 3-25 only need to be done once to setup the project. A `libssh2lv.so` will be located in the `x86_64` folder under the project's root folder inside the Eclipse workspace folder, and a similar `libssh2lv.so` for the ARM architecture will be located in the `ARM` folder under the project's root folder inside the Eclipse workspace folder.

## Tests

All of the tests are located in the `tests` folder. The tests are organized in "modules", where an executable is created that tests each source "module", i.e. writer, reader, etc. The tests are separated from the source, but the tests are built as part of build for the shared library. Each test executable is located in the `bin\tests` folder of the build directory and they can be run independently.

If the `ctest`, or `make test` on non-Windows systems, commands are used _after_ building the tests to run the tests, then the [ctest](https://cmake.org/Wiki/CMake/Testing_With_CTest) test runner framework is used to run the tests. This provides a very high level summary of the results of running all tests. Since the tests are organized into "modules" and suites, the [ctest](https://cmake.org/cmake/help/v3.9/manual/ctest.1.html) command only indicates that a test within a module and suite has failed. It does _not_ indicate which test has failed. To investigate the failed test, the executable in the `bin\tests` folder for test module should be run. For example, if a test in the writer module failed, the ctest test runner will indicate the "writer" test has failed. The `bin\tests\writer` executable should then be run as a standalone application without the ctest test runner to obtain information about which test and assertion failed.

### Windows

Start a terminal command prompt and navigate to the root folder of the project. Note, if following from the [Build](#build) instructions, a command prompt should already be available at the root folder of the project. Enter the following commands to run the tests:

```text
> ctest -C "Debug"
```

### macOS

Start the Terminal.app. Note, if following from the [Build](#build) instructions, the Terminal.app has already been started and the present working directory (pwd) should already be the root folder of the project. Enter the following commands to run the tests:

    $ ctest

Or,

    $ make test

### Linux

Start a terminal. Note, if following from the [Build](#build) instructions, the terminal has already been started and the present working directory (pwd) should already be the root folder of the project. Enter the following commands to run the tests:

    $ ctest

Or,

    $ make test

## License

The LV-LIBSSH2 project is licensed under the [revised BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause) license. The LV-LIBSSH2 project statically compiles code from the [libssh2](http://libssh2.org) and [OpenSSL](https://www.openssl.org) projects, which are licensed under the [MIT](https://github.com/libssh2/libssh2/blob/master/COPYING) license and [Apache-2.0](https://github.com/openssl/openssl/blob/master/LICENSE) license, respectively. See the [LICENSE](https://github.com/fieldrndservices/lv-libssh2/blob/master/LICENSE) file for more information about licensing and copyright.

