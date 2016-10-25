/**
 * @file   tbshare.cpp
 * @brief  tbshare implementation.
 * @author zer0
 * @date   2016-04-17
 * @date   2016-10-14 (Rename: libtbag_test -> libtbtest)
 * @date   2016-10-25 (Rename: libtbtest -> tbshare)
 */

#include <tbshare.h>

int tbshare_add(int v1, int v2)
{
    return v1 + v2;
}

void tbshare_add2(int v1, int v2, int * result)
{
    if (result != nullptr) {
        *result = v1 + v2;
    }
}

