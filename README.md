# TBAG

[![Build Status](https://travis-ci.org/osom8979/tbag.svg?branch=master)](https://travis-ci.org/osom8979/tbag)

## Introduction

Tea-bag is Third party extension utility project.

## Dependencies

- boost 1.62.0 (Boost Software License, Version 1.0)
- boost.stacktrace 41ed839 (Boost Software License, Version 1.0)
- flatbuffers 1.4.0 (Apache License Version 2.0)
- gtest 1.7.0 (BSD 3-clause license)
- icu4c 57.1 (ICU license; http://source.icu-project.org/repos/icu/icu/trunk/LICENSE)
- lmdb bbc15ab (The OpenLDAP Public License)
- lua 5.3.3 (MIT license)
- luabridge 1.0.2 (BSD 3-Clause License)
- lzma 16.04 (Public Domain)
- minizip 1.01 (zlib license)
- sqlite 3.13.0 (Public Domain)
- libuv 1.9.1 (MIT license)
- zlib 1.2.8 (zlib license)

### Inline dependencies (libtbag/3rd)

- demangle (google-glog) 0.3.4 (BSD 3-clause license)
- fmt 3.0.0 (BSD license)
- jsoncpp 1.7.7 (MIT license)
- tinyaes128c f832b41 (Public Domain)
- tinyxml2 3.0.0 (zlib license)

## Programming rules.

- Do not use exception expressions.
 
## How to compile

### Microsoft Visual Studio

MSVC 12 (2013) x86_64 (with Git Bash):
```bash
## Don't use shared library in MSVC12.
$ cmake -G "Visual Studio 12 2013 Win64" ..
```

## License

See the `LICENSE` file for details. In summary, tbag is licensed under the MIT license.

## ASCII Logo

```
*-----------*
|~~~~~|~~~~~|
|    _|_    |
|   /___\   |
|  |     |  |
|  |#####|  |
|  |T-BAG|  |
|  |#####|  |
|  '-----'  |
|    Project|
*-----------*
```

