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

#include <libtbag/preprocessor/control/If.hpp>

#include <libtbag/preprocessor/details/AutoRec.hpp>
#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/details/Error.hpp>
#include <libtbag/preprocessor/details/Stringize.hpp>

#include <libtbag/preprocessor/facilities/Empty.hpp>
#include <libtbag/preprocessor/facilities/Expend.hpp>
#include <libtbag/preprocessor/facilities/IsEmpty.hpp>
#include <libtbag/preprocessor/facilities/Overload.hpp>

#include <libtbag/preprocessor/logical/BitAnd.hpp>
#include <libtbag/preprocessor/logical/BitNor.hpp>
#include <libtbag/preprocessor/logical/BitOr.hpp>
#include <libtbag/preprocessor/logical/BitXor.hpp>
#include <libtbag/preprocessor/logical/Boolean.hpp>
#include <libtbag/preprocessor/logical/Complement.hpp>

#include <libtbag/preprocessor/numeric/Even.hpp>
#include <libtbag/preprocessor/numeric/Odd.hpp>
#include <libtbag/preprocessor/numeric/OddOrEven.hpp>

#include <libtbag/preprocessor/punctuation/Comma.hpp>
#include <libtbag/preprocessor/punctuation/CommaIf.hpp>
#include <libtbag/preprocessor/punctuation/IsBeginParens.hpp>

#include <libtbag/preprocessor/repetition/Repeat2.hpp>

#include <libtbag/preprocessor/seq/SeqIsEmpty.hpp>
#include <libtbag/preprocessor/seq/SeqSize.hpp>
#include <libtbag/preprocessor/seq/VariadicSeqToSeq.hpp>

#include <libtbag/preprocessor/tuple/TupleEat.hpp>
#include <libtbag/preprocessor/tuple/TupleElem.hpp>
#include <libtbag/preprocessor/tuple/TupleSize.hpp>
#include <libtbag/preprocessor/tuple/TupleToSeq.hpp>

#include <libtbag/preprocessor/variadic/VariadicElem.hpp>
#include <libtbag/preprocessor/variadic/VariadicIsEmpty.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>
#include <libtbag/preprocessor/variadic/VariadicToSeq.hpp>
#include <libtbag/preprocessor/variadic/VariadicToTuple.hpp>

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PREPROCESSOR_HPP__

