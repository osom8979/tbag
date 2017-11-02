/**
 * @file   SeqIsEmpty.hpp
 * @brief  SeqIsEmpty preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQISEMPTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQISEMPTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/arithmetic/Decrease.hpp>
#include <libtbag/preprocessor/logical/Boolean.hpp>
#include <libtbag/preprocessor/logical/Complement.hpp>
#include <libtbag/preprocessor/seq/SeqSize.hpp>

#define TBAG_PP_SEQ_IS_EMPTY(seq)            TBAG_PP_COMPL(TBAG_PP_SEQ_IS_NOT_EMPTY(seq))
#define TBAG_PP_SEQ_IS_EMPTY_SIZE(size)      TBAG_PP_COMPL(TBAG_PP_SEQ_IS_NOT_EMPTY_SIZE(size))
#define TBAG_PP_SEQ_IS_NOT_EMPTY(seq)        TBAG_PP_SEQ_IS_NOT_EMPTY_SIZE(TBAG_PP_SEQ_EMPTY_SIZE(seq))
#define TBAG_PP_SEQ_IS_NOT_EMPTY_SIZE(size)  TBAG_PP_BOOL(size)
#define TBAG_PP_SEQ_EMPTY_SIZE(seq)          TBAG_PP_DEC(TBAG_PP_SEQ_SIZE(seq (nil)))

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQISEMPTY_HPP__

