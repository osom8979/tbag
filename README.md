# TBAG

[![Build Status](https://travis-ci.org/osom8979/tbag.svg?branch=master)](https://travis-ci.org/osom8979/tbag)

## Introduction

Tea-bag is Third party extension utility project.

## Dependencies

- gtest 1.7.0 (BSD 3-clause license)
- icu4c 57.1 (ICU license; http://source.icu-project.org/repos/icu/icu/trunk/LICENSE)
- jsoncpp 1.7.7 (MIT license)
- lmdb bbc15ab (The OpenLDAP Public License)
- lua 5.3.3 (MIT license)
- luabridge 1.0.2 (BSD 3-Clause License)
- lzma 16.04 (Public Domain)
- sqlite 3.13.0 (Public Domain): libtbag/database/sqlite3
- libuv 1.9.1 (MIT license)

### Inline dependencies

- google-glog 0.3.4 (BSD 3-clause license): libtbag/debug/demangle
- TinyXML2 3.0.0 (zlib license): libtbag/dom/tinyxml2
- fmt 3.0.0 (BSD license): libtbag/string/fmt
 
## How to compile

### Microsoft Visual Studio

MSVC 12 (2013) x86_64 (with Git Bash):
```bash
## Don't use shared library in MSVC12.
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -G "Visual Studio 12 2013 Win64" ..
```

## license

See the `LICENSE` file for details. In summary, tbag is licensed under the MIT license.

## ascii logo

```
+------------------------------------------------------------+
| This file is part of the:                                  |
|     ____  __    ___   ________ __ __  ______  __    ______ |
|    / __ )/ /   /   | / ____/ //_// / / / __ \/ /   / ____/ |
|   / __  / /   / /| |/ /   / ,<  / /_/ / / / / /   / __/    |
|  / /_/ / /___/ ___ / /___/ /| |/ __  / /_/ / /___/ /___    |
| /_____/_____/_/  |_\____/_/ |_/_/ /_/\____/_____/_____/    |
|                                                   PROJECT. |
+------------------------------------------------------------+
```

