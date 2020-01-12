/**
 * @file   OffsetOf.hpp
 * @brief  OffsetOf class prototype.
 * @author zer0
 * @date   2020-01-12
 *
 * @see <https://en.wikipedia.org/wiki/Offsetof>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OFFSETOF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OFFSETOF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>

#ifndef _TBAG_OFFSET_OF
#define _TBAG_OFFSET_OF(s, m)       \
    (                               \
        (size_t)(                   \
            (                       \
                (char*)(            \
                    &(((s*)0)->m)   \
                )                   \
            )                       \
            - ((char*)0)            \
        )                           \
    )
#endif

#ifndef TBAG_OFFSET_OF
# if defined(offsetof)
#  define TBAG_OFFSET_OF(s, m) offsetof(s, m)
# else
#  define TBAG_OFFSET_OF(s, m) _TBAG_OFFSET_OF(s, m)
# endif
#endif

/**
 * Which allows something like a <strong>mixin</strong> type to find the structure that contains it.
 *
 * @code{.c}
 * struct RootNode {
 *     char const * name;
 *     struct ListNode list;
 * };
 *
 * extern struct ListNode * list_next(struct ListNode *);
 *
 * struct ListNode *current = ...
 *
 * while (current != nullptr) {
 *     struct RootNode * element = TBAG_CONTAINER_OF(current, struct RootNode, list);
 *     printf("%s\n", element->name);
 *     current = list_next(&element->list);
 * }
 * @endcode
 */
#ifndef TBAG_CONTAINER_OF
#define TBAG_CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - TBAG_OFFSET_OF(type, member)))
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OFFSETOF_HPP__

