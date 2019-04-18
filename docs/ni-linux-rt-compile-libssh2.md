# Building libssh2 for a National Instrument's (NI) Linux Real-Time (RT) operating system

## Enabling Secure Shell Server (SSH)

This only has to be done once.

1. Connect the CompactRIO (cRIO) to a computer via Ethernet.
2. Power on the cRIO.
3. Start National Instrument's (NI) Measurement & Automation Explorer (NI-MAX) application.
4. Expand the __Remote Systems__ tree item on the left-hand side of the main application window.
5. Locate and select the cRIO in the list of devices under the __Remote Systems__ tree item.
6. You may need to log in to the cRIO as an adiministrator (the default username is `admin` and password is blank).
7. On the __System Settings__ tab for the cRIO in NI-MAX, under the __Startup Settings__ section, check the __Enable Secure Shell Server (sshd)__ checkbox.
8. Save the changes by clicking on the __Save__ button at the top of the __System Settings__ tab.
9. Reboot the cRIO.

## Setup Build Environment

This only has to be done once, and after building the shared object file (.so), each package can be removed using the `opkg remove <package_name>` from the cRIO.

1. Download the following packages from [NI's opkg feed](http://download.ni.com/ni-linux-rt/feeds/) for the architecture (ARM or x86_64) of the cRIO:

   - binutils
   - cmake (v3.4)
   - cpp
   - cpp-symlinks
   - gcc
   - gcc-symlinks
   - libbfd
   - libcurl4
   - libc6
   - libc6-dev
   - libexpat1
   - libgcc-s-dev
   - libgcc1
   - libgmp10
   - libgnutls30
   - liblzma5
   - liblzo2
   - libmpc3
   - libmpfr4
   - libstdc++6
   - libunistring2
   - libxml2
   - libz1
   - linux-libc-headers-dev
   - make
   - nettle
   - openssl
   - openssl-dev

   The packages are listed in alphabetical order. This is __not__ the installation order. Many of these packages are dependencies for other packages and must be installed before the dependent package is installed.
2. Download the [source code](https://github.com/libssh2/libssh2/archive/libssh2-1.8.2.tar.gz) for the [libssh2](https://www.libssh2.org) C library as `tar.gz` archive file.
3. Connect the cRIO to the host computer via Ethernet.
4. Power up the cRIO.
5. Transfer each of the IPK files that were downloaded in Step 1 to the cRIO's `/tmp` directory. This can be using the [scp](https://en.wikipedia.org/wiki/Secure_copy) command with the suitable SSH client, a WebDAV client, or FTP if the FTP server component is installed on the cRIO. For example, assuming all of the needed IPK files have been downloaded from the NI repository to the host computer and are in the same directory, the following command can be used on the host computer:

   ```
   $ scp *.ipk admin@XXX.XXX.XXX.XXX:/tmp/
   ```
   
   where `XXX.XXX.XXX.XXX` is the IP address or hostname of the cRIO.
6. Transfer the libssh2 archive file (`.tar.gz`) to the cRIO using a similar mechanism from Step 5. For example, if using the [scp](https://en.wikipedia.org/wiki/Secure_copy) application, the following command can be used:

   ``` 
   $ scp libssh2-1.8.2.tar.gz admin@XXX.XXX.XXX.XXX:/tmp/
   ```
   
   where `XXX.XXX.XXX.XXX` is the IP address or hostname of the cRIO.
7. Start a suitable SSH client on the host computer.
8. Login via SSH to the cRIO using the host's SSH client.
9. Navigate to the cRIO's `/tmp` directory:

   ```
   $ cd /tmp
   ```
   
   Note, the contents of the `/tmp` directory are removed every time the cRIO is restarted.
10. Install each of the packages in the following order with the [opkg](https://openwrt.org/docs/guide-user/additional-software/opkg) application. Note, the asterisk, `*`, is a wildcard to mean "any" file. The IPK files will have the version number and architecture, which is dependent on which cRIO is available.

    ```
    $ opkg install make*.ipk
    $ opkg install libgmp10*.ipk
    $ opkg install libunistring2*.ipk
    $ opkg install nettle*.ipk
    $ opkg install libgnutls30*.ipk
    $ opkg install libcurl4*.ipk
    $ opkg install cmake_3.4.3-r0.3*.ipk
    $ opkg install libmpfr4*.ipk
    $ opkg install libmpc3*.ipk
    $ opkg install cpp_*.ipk
    $ opkg install cpp-symlinks*.ipk
    $ opkg install gcc_*.ipk
    $ opkg install gcc-symlinks*.ipk
    $ opkg install libgcc-s-dev*.ipk
    $ opkg install libbfd*.ipk
    $ opkg install binutils*.ipk
    $ opkg install linux-libc-headers-dev*.ipk
    $ opkg install libc6-dev*.ipk
    $ opkg install openssl-dev*.ipk
    ```

    On older cRIOs, the `opkg` package requires the `libarchive13` package, while newer versions of the `cmake` package require the `libarchive` package. These are the same packages but the `libarchive` package was renamed. The opkg package does not recognize the name change and prevents installation of the newer libarchive package for newer version of cmake.
11. Extract the libssh2 source code.

    ```
    $ tar -xvzf libssh2-1.8.2.tar.gz
    ```
    
12. Navigate to the libssh2 source code root folder.

    ```
    $ cd /libssh2
    ```
    
13. Create a build output folder.

    ```
    $ mkdir bin
    ```

14. Nagivate into the build output folder.

    ```
    cd bin
    ```
    
13. Generate build files cmake.

    ```
    $ cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DCRYPTO_BACKEND=OpenSSL ..
    ```

14. Build using cmake.

    ```
    $ cmake --build .
    ```

## Install libssh2 on cRIO

Following from the previous section for building the library, this step is optional, but a shared object library for libssh2 will need to be present in the library search path (`/lib`, `/usr/lib`, `/usr/local/lib`, `/usr/local/natinst/lib`) for the lv-libssh2 library to work. The preferred location is either `/usr/lib` or `/usr/local/lib`.

1. Install libssh2 using cmake.

   ```
   $ cmake --build . --target install
   ```

   This will put the shared object library (libssh2.so) into the `/usr/local/lib` directory on the cRIO.
