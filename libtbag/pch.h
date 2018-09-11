/**
 * @file   pch.h
 * @brief  pch class prototype.
 * @author zer0
 * @date   2018-09-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PCH_H__
#define __INCLUDE_LIBTBAG__LIBTBAG_PCH_H__

/* MS compatible compilers support #pragma once */
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
// [WARNING] Don't use local headers. (rebuild issue)
//# include <libtbag/config.h>
//# include <libtbag/predef.hpp>
//# include <libtbag/Noncopyable.hpp>
//# include <libtbag/Err.hpp>

# include <cstdlib>
# include <cstdio>
# include <cstring>
# include <cstdint>
# include <cmath>

# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include <map>
# include <set>
# include <unordered_map>
# include <unordered_set>
# include <list>
# include <stack>
# include <queue>
# include <atomic>
# include <algorithm>
# include <type_traits>
# include <random>
# include <regex>
# include <new>

# include <thread>
# include <chrono>
# include <mutex>
#endif /* __cplusplus */

#endif /* __INCLUDE_LIBTBAG__LIBTBAG_PCH_H__ */

