/**
 * @file   libtbag_test.cpp
 * @brief  libtbag_test implementation.
 * @author zer0
 * @date   2016-04-17
 */

#include <libtbag_test/libtbag_test.h>

int __libtbag_test__add__(int v1, int v2)
{
    return v1 + v2;
}

void __libtbag_test__add2__(int v1, int v2, int * result)
{
    if (result != nullptr) {
        *result = v1 + v2;
    }
}

