## VERSION

- fmt 3.0.1

## LICENSE

- BSD license

## SETUP

0. Rename files.
 - format.cc  > format.cpp.h
 - ostream.cc > ostream.cpp.h
 - posix.cc   > posix.cpp
1. Fix the include preprocessor.
 - <code>#include "format.h"</code> to <code>#include <libtbag/3rd/fmt/format.h></code>
 - <code># include "format.cc"</code> to <code># include <libtbag/3rd/fmt/format.cpp.h></code>
 - <code>#include "ostream.h"</code> to <code>#include <libtbag/string/fmt/ostream.h></code>
 - <code># include "ostream.cc"</code> to <code># include <libtbag/string/fmt/ostream.cpp.h></code>
 - <code>#include "posix.h"</code> to <code>#include <libtbag/3rd/fmt/posix.h></code>
2. Insert FMT_HEADER_ONLY define in <code>format.h</code>, <code>ostream.h</code>
```cpp
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
```

