/**
 * @file   VariadicSeqToSeq.hpp
 * @brief  VariadicSeqToSeq class prototype.
 * @author zer0
 * @date   2017-10-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_VARIADICSEQTOSEQ_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_VARIADICSEQTOSEQ_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Cat.hpp>

#define TBAG_PP_VARIADIC_SEQ_TO_SEQ(vseq)   TBAG_PP_CAT(TBAG_PP_VARIADIC_SEQ_TO_SEQ_A vseq, 0)
#define TBAG_PP_VARIADIC_SEQ_TO_SEQ_A(...)  ((__VA_ARGS__)) TBAG_PP_VARIADIC_SEQ_TO_SEQ_B
#define TBAG_PP_VARIADIC_SEQ_TO_SEQ_B(...)  ((__VA_ARGS__)) TBAG_PP_VARIADIC_SEQ_TO_SEQ_A
#define TBAG_PP_VARIADIC_SEQ_TO_SEQ_A0
#define TBAG_PP_VARIADIC_SEQ_TO_SEQ_B0

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_VARIADICSEQTOSEQ_HPP__

