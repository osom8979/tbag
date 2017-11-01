/**
 * @file   Preprocessor.hpp
 * @brief  Preprocessor preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PREPROCESSOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PREPROCESSOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/preprocessor/arithmetic/Decrease.hpp>
#include <libtbag/preprocessor/arithmetic/Increase.hpp>

#include <libtbag/preprocessor/logical/Boolean.hpp>
#include <libtbag/preprocessor/logical/Complement.hpp>

#include <libtbag/preprocessor/details/AutoRec.hpp>
#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/details/Comma.hpp>
#include <libtbag/preprocessor/details/CommaIf.hpp>
#include <libtbag/preprocessor/details/Empty.hpp>
#include <libtbag/preprocessor/details/Error.hpp>
#include <libtbag/preprocessor/details/Even.hpp>
#include <libtbag/preprocessor/details/Expend.hpp>
#include <libtbag/preprocessor/details/ExpendParams.hpp>
#include <libtbag/preprocessor/details/If.hpp>
#include <libtbag/preprocessor/details/Odd.hpp>
#include <libtbag/preprocessor/details/OddEvenIf.hpp>
#include <libtbag/preprocessor/details/Overload.hpp>
#include <libtbag/preprocessor/details/Repeat.hpp>
#include <libtbag/preprocessor/details/Stringize.hpp>

#include <libtbag/preprocessor/seq/SeqSize.hpp>
#include <libtbag/preprocessor/seq/VariadicSeqToSeq.hpp>

#include <libtbag/preprocessor/tuple/TupleElem.hpp>
#include <libtbag/preprocessor/tuple/TupleEat.hpp>
#include <libtbag/preprocessor/tuple/TupleSize.hpp>
#include <libtbag/preprocessor/tuple/TupleToSeq.hpp>

#include <libtbag/preprocessor/variadic/VariadicElem.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>
#include <libtbag/preprocessor/variadic/VariadicToSeq.hpp>
#include <libtbag/preprocessor/variadic/VariadicToTuple.hpp>

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PREPROCESSOR_HPP__

