/**
 * @file   libtbtest.cpp
 * @brief  libtbtest implementation.
 * @author zer0
 * @date   2016-04-17
 * @date   2016-10-14 (Rename: libtbag_test -> libtbtest)
 */

#include <libtbtest/libtbtest.h>

int libtbtest_add(int v1, int v2)
{
    return v1 + v2;
}

void libtbtest_add2(int v1, int v2, int * result)
{
    if (result != nullptr) {
        *result = v1 + v2;
    }
}

