# TBAG

[![TravisCI Build Status](https://travis-ci.org/osom8979/tbag.svg?branch=master)](https://travis-ci.org/osom8979/tbag)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/osom8979/tbag?branch=master&svg=true)](https://ci.appveyor.com/project/osom8979/tbag)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Introduction

Tea-bag is Third party extension utility project.

## Dependencies

- flatbuffers 1.7.1 (Apache License, Version 2.0)
- gtest 1.7.0 (BSD 3-clause license)
- http-parser 2.7.1 (MIT license)
- icu4c 57.1 (ICU license; http://source.icu-project.org/repos/icu/icu/trunk/LICENSE)
- lmdb bbc15ab (The OpenLDAP Public License)
- lua 5.3.3 (MIT license)
- minizip 1.1 (zlib license)
- simdpp 2.0-rc2 (Boost Software License, Version 1.0)
- sqlite 3.13.0 (Public Domain)
- stb master-c711058 (Public Domain or MIT license)
- libuv 1.9.1 (MIT license)
- date master-4f27361 (MIT license)
- demangle (google-glog) 0.3.4 (BSD 3-clause license)
- fmt 3.0.1 (BSD license)
- jsoncpp 1.7.7 (MIT license)
- tinyxml2 3.0.0 (zlib license)
- entt 2.1.0 (MIT license)
- sol2 2.18.5 (MIT license)

### External projects
- libressl 2.5.1 (Dual license; OpenSSL & SSLeay)
- zlib 1.2.11 (zlib license)

### Resources
- ProggyClean.ttf (MIT license)

## Programming rules.

- Do not use exception expressions.
 
## How to compile

### Microsoft Visual Studio

MSVC 12 (2013) x86_64 (with Git Bash):
```bash
## Don't use shared library in MSVC12.
$ cmake -G "Visual Studio 12 2013 Win64" ..
```

### Without LibReSSL
```bash
$ cmake -DWITH_RESSL=OFF ..
```

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

