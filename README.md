# [LibreMaple](https://libremaple.org/) Client: a fork/continuation of JourneyClient

[![License](https://img.shields.io/badge/license-AGPL%20v3.0%2B-brightgreen.svg)](https://www.gnu.org/licenses/agpl-3.0.html)
[![C++ standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![clang++ version](https://img.shields.io/badge/clang%2B%2B-6.0.0+-dd66aa.svg)](http://clang.llvm.org/)
[![Maplestory version](https://img.shields.io/badge/Maplestory-GMS_v83-orange.svg)](https://en.wikipedia.org/wiki/MapleStory)

## JourneyClient

JourneyClient is a custom, made-from-scratch game client for
[Maplestory](https://en.wikipedia.org/wiki/MapleStory).
JourneyClient is written in [C++](https://en.wikipedia.org/wiki/C%2B%2B) and
was created by Daniel Allendorf in 2015.

The [LibreMaple](https://libremaple.org/) project aims to make a version of
Maplestory that is
[100% free and open](https://www.fsf.org/about/what-is-free-software) on both
the server and client sides, to preserve the freedom of its users and enable
creativity and collaboration around the game. As such, the LibreMaple project
takes the JourneyClient as a starting point.

LibreMaple aims to make the client
[cross-platform](https://en.wikipedia.org/wiki/Cross-platform)
([GNU+Linux](https://en.wikipedia.org/wiki/Linux)/[macOS](https://en.wikipedia.org/wiki/MacOS)/[Windows NT](https://en.wikipedia.org/wiki/Windows_NT)/[FreeBSD](https://en.wikipedia.org/wiki/FreeBSD)/etc.)
as well, so decisions are made with a cross-platform mindset as well as a free
software one.

## Supported versions

The client is currently compatible with GMS version 83 servers. For the UI file
(UI.nx), one from GMS version 154+ should be used.

## How to compile/install

### [Unix-like systems](https://en.wikipedia.org/wiki/Unix-like) excluding macOS (GNU+Linux, FreeBSD, etc.) (may also work for Unix-like Windows subsystems like [Cygwin](https://en.wikipedia.org/wiki/Cygwin), [MinGW](https://en.wikipedia.org/wiki/MinGW), or [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux))

#### Dependencies

* [clang](http://clang.llvm.org/) (version 6+)
* [lld](https://lld.llvm.org/) (version 6+)
* [llvm](https://llvm.org/) (version 6+, including any "tools" or "runtime")
* [cmake](https://cmake.org/) (version 3.10+)
* [make](https://www.gnu.org/software/make/) (GNU Make version 4+)
* [git](https://git-scm.com/)
* [wget](https://www.gnu.org/software/wget/)
* [tar](https://www.gnu.org/software/tar/)
* unzip
* [autoconf](https://www.gnu.org/software/autoconf/autoconf.html)
* [libtool](https://www.gnu.org/software/libtool/)
* [sh or bash](https://en.wikipedia.org/wiki/Bourne_shell)
* [glut](http://freeglut.sourceforge.net/) / freeglut / freeglut3,
  freeglut3-dev (an implementation of GLUT, including development files,
  version 3)
* libXmu / libxmu (including the "development" version, if that's separate)
* libXi / libxi (including the "development" version)
* libgl-dev / libgl1-mesa-dev / libgl / mesa / libGL-devel (implementation of
  OpenGL with development files)
* libosmesa-dev / libosmesa / osmesa (skip this if your package manager doesn't
  have it)
* xorg / xorg-dev / xorg-server-devel (Xorg development libraries)
* build-essential (for users of Debian-based distibutions only)

(**Note for [Solus](https://solus-project.com/) users:**
`sudo eopkg it -c system.devel`)

Most of these dependencies should be available from your package manager
(`pacman`, `apt`, `dnf`, `zypper`, `pkg`, `xbps-install`, `eopkg`, `yum`,
etc.), but for all dependencies listed with a version number, **make sure
to check that the version your package manager gives you is high enough**.
If your package manager doesn't have something or doesn't have a high enough
version, you may have to install it using instructions from the relevant
website and/or use auxilliary package management (`yaourt`, PPAs, `copr`,
etc.).

#### Instructions

```bash
$ git clone https://github.com/Libre-Maple/LibreMaple-Client.git
$ git clone https://github.com/NoLifeDev/NoLifeNx.git nlnx

# The next command is probably best replaced by navigating to
# https://sourceforge.net/projects/asio/files/latest/download
# and downloading from there to get the latest stable version of ASIO.
$ wget https://downloads.sourceforge.net/project/asio/asio/1.12.1%20%28Stable%29/asio-1.12.1.tar.bz2
$ mkdir asio
$ tar xf asio-* --strip-components=1 -C asio/
$ rm asio-*

$ git clone https://github.com/glfw/glfw.git

# Again, the following command can be replaced by just going to
# https://sourceforge.net/projects/glew/files/glew
# and downloading the latest version of GLEW.
$ wget https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz
$ mkdir glew
$ tar xf glew-* --strip-components=1 -C glew/
$ rm glew-*

$ git clone git://git.sv.nongnu.org/freetype/freetype2.git freetype

# www.un4seen.com
$ wget http://www.un4seen.com/files/bass24-linux.zip
$ mkdir bass
$ unzip bass2* -d bass/
$ rm bass2*

$ git clone https://github.com/lz4/lz4.git

# Again, the following command can be replaced by just going to
# https://www.boost.org/users/download/
# and downloading the latest version of Boost.
$ wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
$ tar xf boost*
$ rm boost*.tar.gz
$ mv boost*/ boost/

$ export CC='/usr/bin/clang'
$ export CXX='/usr/bin/clang++'

$ cd freetype
$ sh autogen.sh
$ ./configure
$ make
$ cd ..

$ cd glew
$ make
$ cd ..

$ cd glfw
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../..

$ cd lz4
$ make
$ cd ..

$ cd LibreMaple-Client
$ mkdir build
$ cd build
# Build type may also be `Release`, `RelWithDebInfo`, or `MinSizeRel`.
# Additionally,pass in the argument `-D NATIVE_BUILD=true` if you desire a
# native-only build (`-march=native`).
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
# Or `make -jN` with N being the number of CPU cores you wish to utilize.
$ make
# For `Release` and `MinSizeRel` builds only:
$ strip JourneyClient
```

If all this is successful, you should have the executable in your current
directory (`LibreMaple-Client/build`).

### macOS

#### Dependencies

* macOS Command Line Tools (can be installed using `xcode-select --install`,
  includes clang, git, make, and some things needed for basic clang
  functionality)
* [Homebrew](https://brew.sh/)
* [llvm](https://llvm.org/) (`brew install llvm`)
* [cmake](https://cmake.org/) (`brew install cmake`)
* [wget](https://www.gnu.org/software/wget/) (`brew install wget`)
* [autoconf](https://www.gnu.org/software/autoconf/autoconf.html)
  (`brew install autoconf`)
* [libtool](https://www.gnu.org/software/libtool/) (`brew install libtool`; the
  macOS CL tools come with a libtool, but this installs the GNU version with
  'g' prefix e.g. `glibtoolize`)
* [freeglut](http://freeglut.sourceforge.net/) (`brew install freeglut`; may
  require you to separately install XQuartz, one way to do this is
  `brew cask install xquartz`)
* [freetype](https://www.freetype.org/) (`brew install freetype`)

#### Instructions

```bash
$ git clone https://github.com/Libre-Maple/LibreMaple-Client.git
$ git clone https://github.com/NoLifeDev/NoLifeNx.git nlnx

# The next command is probably best replaced by navigating to
# https://sourceforge.net/projects/asio/files/latest/download
# and downloading from there to get the latest stable version of ASIO.
$ wget https://downloads.sourceforge.net/project/asio/asio/1.12.1%20%28Stable%29/asio-1.12.1.tar.bz2
$ mkdir asio
$ tar xf asio-* --strip-components=1 -C asio/
$ rm asio-*

$ git clone https://github.com/glfw/glfw.git

# Again, the following command can be replaced by just going to
# https://sourceforge.net/projects/glew/files/glew
# and downloading the latest version of GLEW.
$ wget https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz
$ mkdir glew
$ tar xf glew-* --strip-components=1 -C glew/
$ rm glew-*

# www.un4seen.com
$ wget http://www.un4seen.com/files/bass24-osx.zip
$ mkdir bass
$ unzip bass2* -d bass/
$ rm bass2*

$ git clone https://github.com/lz4/lz4.git

# Again, the following command can be replaced by just going to
# https://www.boost.org/users/download/
# and downloading the latest version of Boost.
$ wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
$ tar xf boost*
$ rm boost*.tar.gz
$ mv boost*/ boost/

$ export CC='clang'
$ export CXX='clang++'

$ cd glew
$ make
$ cd ..

$ cd glfw
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../..

$ cd lz4
$ make
$ cd ..

$ cd LibreMaple-Client
$ mkdir build
$ cd build
# Build type may also be `Release`, `RelWithDebInfo`, or `MinSizeRel`.
# Additionally,pass in the argument `-D NATIVE_BUILD=true` if you desire a
# native-only build (`-march=native`).
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
# Or `make -jN` with N being the number of CPU cores you wish to utilize.
$ make
# For `Release` and `MinSizeRel` builds only:
$ strip JourneyClient
```

If all this is successful, you should have the executable in your current
directory (`LibreMaple-Client/build`). However, it probably will not run right
away, since the paths that the executable looks for dylibs on will be wrong.

In order to fix this, gather up the following files, copying them to wherever
you find most appropriate if desired (you may want to put them all into the
same directory as the executable for ease of use). All paths shown here are
relative to the base directory where you cloned all of the repos and extracted
the tarballs:

* `libbass.dylib` (found as `bass/libbass.dylib`)
* `libGLEW.dylib` (found as `glew/lib/libGLEW.dylib`, this should be a symlink
  to `libGLEW.2.1.0.dylib` or something like that, so be aware)
* `liblz4.dylib` (found as `lz4/lib/liblz4.dylib`, this should be a symlink to
  `liblz4.1.8.2.dylib` or something like that, so be aware)

Now that you have the locations of these dylibs, you can change the executable
to point to them. Like this, for example:

```bash
$ install_name_tool -change @loader_path/libbass.dylib ./libbass.dylib JourneyClient
$ install_name_tool -change /usr/local/lib/libGLEW.2.1.0.dylib ./libGLEW.2.1.0.dylib JourneyClient
$ install_name_tool -change /usr/local/lib/liblz4.1.dylib ./liblz4.1.8.2.dylib JourneyClient
```

### Windows NT (Windows 7, 8, 10+) (64-bit only, i.e. amd64/x86_64)

#### Dependencies

* [Visual Studio 2017](https://www.visualstudio.com/downloads/) (the Community Edition works just fine, make sure to enable Visual C++ development when installing)
* [.NET Framework 4](https://www.microsoft.com/en-us/download/details.aspx?id=17718) (hopefully already installed, but just in case)
* [Visual C++ Redistributable for Visual Studio 2017](https://go.microsoft.com/fwlink/?LinkId=746572) (probably already installed, but again, just in case)
* [clang for windows](https://releases.llvm.org/download.html) (version 6+; download and install **Pre-Built Binaries:** -> Clang for Windows (64-bit))
* [cmake](https://cmake.org/download/) (latest version, 3.11.X as of this writing)
* [git](https://git-scm.com/)
* [7-Zip](https://www.7-zip.org/)

#### Instructions

```bat
$ git clone https://github.com/Libre-Maple/LibreMaple-Client.git
$ git clone https://github.com/NoLifeDev/NoLifeNx.git nlnx

$ git clone https://github.com/ubawurinna/freetype-windows-binaries.git freetype
```

Navigate to
[https://sourceforge.net/projects/asio/files/latest/download](https://sourceforge.net/projects/asio/files/latest/download)
and download `asio-X.Y.Z.zip`, whatever the latest version is
(`asio-1.12.1.zip` as of this writing).

Use 7-Zip to extract the contents of the asio ZIP file to a directory called
`asio`.

Navigate to
[http://www.glfw.org/download.html](http://www.glfw.org/download.html) and
download the 64-bit Windows binaries (called `glfw-3.2.1.bin.WIN64.zip` as of
this writing).

Use 7-Zip to extract the contents of the glfw ZIP file into a directory called
`glfw`.

Navigate to [http://glew.sourceforge.net/](http://glew.sourceforge.net/) and
download the "Binaries: Windows 32-bit and 64-bit" ZIP file (called
`glew-2.1.0-win32.zip` as of this writing).

Use 7-Zip to extract the contents of the glew ZIP file into a directory called
`glew` (make sure that the contents are at the top level of `glew`, you want
`glew\bin`, not `glew\glew-2.1.0\bin`).

Navigate to [https://www.un4seen.com/](https://www.un4seen.com/) and download
the latest version of BASS for Windows (called `bass24.zip` as of this
writing).

Use 7-Zip to extract the contents of the bass ZIP file into a directory called
`bass`.

Navigate to
[https://github.com/lz4/lz4/releases](https://github.com/lz4/lz4/releases) and
download the latest version of LZ4 for "win64" (called `lz4_v1_8_2_win64.zip`
as of this writing).

Use 7-Zip to extract the contents of the LZ4 ZIP file into a directory called
`lz4`.

Navigate to
[https://www.boost.org/users/download/](https://www.boost.org/users/download/)
and download the latest version of Boost for Windows (called `boost_1_67_0.7z`
as of this writing).

Use 7-Zip to extract the contents of the Boost 7z file into a directory called
`boost`.

Notice here that we assume the installation directory of LLVM is the default
one. If you don't use the default installation directory, you will have to
tweak `CMakeLists.txt` yourself:

```bat
$ SET CC="C:\Program Files\LLVM\bin\clang.exe"
$ SET CXX="C:\Program Files\LLVM\bin\clang++.exe"

$ cd LibreMaple-Client
$ md build
$ cd build
```

Use the following two commands if `CMAKE_BUILD_TYPE` is to be `Debug` (you may
wish to add an additional `/mN` flag, with `N` being the number of CPU cores
you wish to utilize for compilation):

```bat
$ cmake -G "Visual Studio 15 2017 Win64" -T "LLVM-vs2017" -D CMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -D CMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -D CMAKE_BUILD_TYPE=Debug ..
$ cmake --build . -- /v:d /property:Configuration=Debug /property:Platform=x64
```

`CMAKE_BUILD_TYPE` here may also be `Release`, `RelWithDebInfo`, or
`MinSizeRel`. If you want to use one of these build types, use these commands
instead, setting `CMAKE_BUILD_TYPE` to the desired value (we use `Release`
here as an example):

```bat
$ cmake -G "Visual Studio 15 2017 Win64" -T "LLVM-vs2017" -D CMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -D CMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -D CMAKE_BUILD_TYPE=Release ..
$ cmake --build . -- /v:d /property:Configuration=Release /property:Platform=x64
```

If all this is successful, you should have the executable (`JourneyClient.exe`)
in `LibreMaple-Client\build\Debug` for debug builds, or
`LibreMaple-Client\build\Release` for release build types. However, it
probably will not run right away, since the paths that the executable looks
for DLLs on will be wrong.

In order to fix this, gather up the following files, copying them to be right
next to the executable. All paths shown here are relative to the base directory
where you cloned all of the repos and extracted the ZIP files:

* `freetype.dll` (found as `freetype\win64\freetype.dll`)
* `bass.dll` (found as `bass\x64\bass.dll`)
* `glew32.dll` (found as `glew\bin\Release\x64\glew32.dll`)
* `liblz4.dll` (found as `lz4\dll\liblz4.so.1.8.2.dll`, the version number may
  be different and you will have to rename your copied version to `liblz4.dll`)

## Compiler

LibreMaple is built against the latest version of
[Clang](http://clang.llvm.org/) (6.0.X as of this writing), again for
cross-platform considerations and efficiency. [The GCC](https://gcc.gnu.org/)
(version 7.3.X as of this writing) is another excellent choice, especially for
those on GNU+Linux operating systems (or other OSs which default to GCC), but
GCC is **not** officially supported by LibreMaple.

That being said, don't be shy to file issues involving GCC or to make PRs to
improve ease of use/compatibility with it.

## C++ details

JourneyClient was originally developed under
[the C++14 standard](https://en.wikipedia.org/wiki/C%2B%2B14). However,
**LibreMaple strictly uses**
**[the C++17 standard](https://en.wikipedia.org/wiki/C%2B%2B17) and makes use**
**of C++17 features**.

In addition, **this project follows**
**[the ISO C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)**,
so any code that does not comply with said guidelines can be considered a bug,
unless of course the transgression has been justified and thoroughly commented.
[A complete copy of the guidelines is available here](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md).

## Configuration

The build can be configured by editing the Journey.h file.
The following options are available:

* `JOURNEY_USE_CRYPTO` : Use cryptography when communicating for the server.
* `JOURNEY_USE_XXHASH` : Use xxhash for file check (additional dependency)
* `JOURNEY_USE_ASIO`   : Use Asio for networking (additional dependency)

LibreMaple uses crypto (`JOURNEY_USE_CRYPTO`), and also uses ASIO
(`JOURNEY_USE_ASIO`) to maintain cross-platform compatibility.

## Dependencies

| **Category**      | **Dependency**                                             | **License**         | **Depends on** | **Header only?** | **Optional?** |
|-------------------|------------------------------------------------------------|---------------------|----------------|------------------|---------------|
| (De)compression   | [LZ4](https://github.com/lz4/lz4)                          | BSD 2-Clause/GPL v2 | -              | No               | No            |
| Error checking    | [xxHash](https://github.com/Cyan4973/xxHash)               | BSD 2-Clause        | -              | No               | Yes           |
| NX library        | [NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)          | ???\*               | LZ4            | No               | No            |
| Graphics          | [GLFW3](http://www.glfw.org/download.html)                 | zlib/libpng         | OpenGL support | No               | No            |
| Graphics          | [GLEW](http://glew.sourceforge.net/)                       | Modified BSD/MIT    | OpenGL support | No               | No            |
| Graphics          | [FreeType](http://www.freetype.org/)                       | FreeType            | -              | No               | No            |
| Audio             | [Bass](http://www.un4seen.com/)                            | Proprietary(!)      | -              | No               | No            |
| Networking        | [Asio](http://think-async.com/) (standalone; no Boost)     | Boost               | -              | Yes              | No            |
| General           | [Boost](https://www.boost.org/) (Bimap, Container)         | Boost               | -              | Yes              | No            |

\*NoLifeNx does not provide a license, but it is used for NoLifeStory (AGPL
v3), so it must be compatible with the AGPL v3 (likely BSD 3-Clause like nx-rs,
or even AGPL v3 itself).
