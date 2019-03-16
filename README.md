# TBAG

[![](https://travis-ci.org/osom8979/tbag.svg?branch=master)](https://travis-ci.org/osom8979/tbag "TravisCI Build Status")
[![](https://ci.appveyor.com/api/projects/status/github/osom8979/tbag?branch=master&svg=true)](https://ci.appveyor.com/project/osom8979/tbag "AppVeyor Build Status")
[![](https://codecov.io/gh/osom8979/tbag/branch/master/graph/badge.svg)](https://codecov.io/gh/osom8979/tbag "Codecov Coverage Status")
[![](https://images.microbadger.com/badges/version/osom8979/tbag.svg)](https://hub.docker.com/r/osom8979/tbag "DockerHub Container Version")
[![](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE "MIT License")

## Introduction

Tea-bag is Third party extension utility project.

## Dep target

- flatbuffers 1.10.0 (Apache License, Version 2.0)
- gtest 1.7.0 (BSD 3-clause license)
- http-parser 2.8.1 (MIT license)
- icu4c 57.1 (ICU license; http://source.icu-project.org/repos/icu/icu/trunk/LICENSE)
- lmdb bbc15ab (The OpenLDAP Public License)
- minizip 1.1 (zlib license)
- simdpp 2.1 (Boost Software License, Version 1.0)
- sqlite 3.24.0 (Public Domain)
- stb master-e6afb9c (Public Domain or MIT license)
- date master-4f27361 (MIT license)
- demangle (google-glog) 0.3.4 (BSD 3-clause license)
- fmt 3.0.1 (BSD license)
- jsoncpp 1.7.7 (MIT license)
- tinyxml2 3.0.0 (zlib license)
- entt 2.1.0 (MIT license)
- sol2 2.20.6 (MIT license)
- lmdb++ master-0b43ca8 (0.9.14.1) (Public Domain)
- sqlite_orm master-fecdc19 (BSD 3-clause license)
- lemon 1.3.1 (Boost Software License, Version 1.0)
- nanosvg master-2b08dee (zlib license)
- lfds 7.1.1 (Public domain)
- termbox 1.1.2 (MIT license)
- Canadadry/luaSFML master-f55fdd4 (MIT license)

## External target

- libressl 2.5.1 (Dual license; OpenSSL & SSLeay)
- zlib 1.2.11 (zlib license)
- libuv 1.22.0 (MIT license)
- freetype 2.9.1 (FreeType License)
- ogg 1.3.3 (BSD license)
- flac 1.3.2 (BSD license)
- vorbis 1.3.6 (BSD license)
- sfml 2.5.1 (zlib/png license)
- LuaJIT 2.0.5 (MIT license)
  - LuaDist/LuaJIT-CMakeLists.txt 2.0.3 (MIT license)
- bzip2 1.0.5 (BSD-like license)
- xz/liblzma 5.2.4 (Public domain)
- libarchive 3.3.3 (New BSD license)
- CMake 3.13.4 (BSD 3-Clause license)
  - external/archive/build/Modules/CMakePushCheckState.cmake
- TGUI 0.8.4 (zlib license)
- gperftools 2.7 (BSD 3-Clause license)
- Box2D master-ef96a4f (zlib license)
- SoLoud master-f2ec233 (zlib license)

## Resources

- NanumGothicCoding 2.5 (SIL Open Font License, Version 1.1)

## Dependencies

```bash
## OpenCL
sudo apt-get install ocl-icd-opencl-dev

## OpenGL
sudo apt-get install libglu1-mesa-dev

## UDev
sudo apt-get install libudev-dev

## x11, xRandr
sudo apt-get install xorg-dev

## xcb-image
sudo apt-get install libxcb-image0-dev
```

## Programming rules.

- Do not use exception expressions.
 
## CMake build options.

The main build options you will want to configure are as follows:

* `USE_CCACHE`: Enable `ccache` compile.
* `USE_GOLD`: Enable `ld.gold` linker if `gcc` compiler.
* `USE_PCH`: Enable PCH compile.
* `USE_OPENGL`: Enable `OpenGL` library.
* `USE_OPENCL`: Enable `OpenCL` library.
* `USE_CUDA`: Enable `CUDA` compile.
* `USE_DOXYGEN`: Enable `doxygen` documentation.
* `USE_GUI`: Enable GUI module.
* `USE_RTTI`: Enable RTTI flag.
* `DISABLE_TDLOG`: Disable tbag default logger.
* `DISABLE_LUAJIT_EXE_FLAGS`: Disable LuaJIT Embedding flags if apple.

## Tpot

### Environment variables.

* `TPOT_MODE`: If this value is `lua`, run on the luac mode.

## License

See the `LICENSE` file for details. In summary, tbag is licensed under the MIT license.

## ASCII Logo

```
+--------------------------+
|    __  __                |
|   / /_/ /_  ____ _____ _ |
|  / __/ __ \/ __ `/ __ `/ |
| / /_/ /_/ / /_/ / /_/ /  |
| \__/_.___/\__,_/\__, /   |
|                /____/    |
| Third party libs package |
+--------------------------+
```

