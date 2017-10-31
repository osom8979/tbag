/**
 * @file   SeqSize.hpp
 * @brief  SeqSize preprocessor prototype.
 * @author zer0
 * @date   2017-10-31
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQSIZE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQSIZE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/preprocessor/details/Cat.hpp>

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_SEQ_SIZE(seq)   TBAG_PP_SEQ_SIZE_I(seq)
# define TBAG_PP_SEQ_SIZE_I(seq) TBAG_PP_CAT(TBAG_PP_SEQ_SIZE_, TBAG_PP_SEQ_SIZE_0 seq)
#else
# define TBAG_PP_SEQ_SIZE(seq)   TBAG_PP_CAT(TBAG_PP_SEQ_SIZE_, TBAG_PP_SEQ_SIZE_0 seq)
#endif

#define TBAG_PP_SEQ_SIZE_0(_)    TBAG_PP_SEQ_SIZE_1
#define TBAG_PP_SEQ_SIZE_1(_)    TBAG_PP_SEQ_SIZE_2
#define TBAG_PP_SEQ_SIZE_2(_)    TBAG_PP_SEQ_SIZE_3
#define TBAG_PP_SEQ_SIZE_3(_)    TBAG_PP_SEQ_SIZE_4
#define TBAG_PP_SEQ_SIZE_4(_)    TBAG_PP_SEQ_SIZE_5
#define TBAG_PP_SEQ_SIZE_5(_)    TBAG_PP_SEQ_SIZE_6
#define TBAG_PP_SEQ_SIZE_6(_)    TBAG_PP_SEQ_SIZE_7
#define TBAG_PP_SEQ_SIZE_7(_)    TBAG_PP_SEQ_SIZE_8
#define TBAG_PP_SEQ_SIZE_8(_)    TBAG_PP_SEQ_SIZE_9
#define TBAG_PP_SEQ_SIZE_9(_)    TBAG_PP_SEQ_SIZE_10
#define TBAG_PP_SEQ_SIZE_10(_)   TBAG_PP_SEQ_SIZE_11
#define TBAG_PP_SEQ_SIZE_11(_)   TBAG_PP_SEQ_SIZE_12
#define TBAG_PP_SEQ_SIZE_12(_)   TBAG_PP_SEQ_SIZE_13
#define TBAG_PP_SEQ_SIZE_13(_)   TBAG_PP_SEQ_SIZE_14
#define TBAG_PP_SEQ_SIZE_14(_)   TBAG_PP_SEQ_SIZE_15
#define TBAG_PP_SEQ_SIZE_15(_)   TBAG_PP_SEQ_SIZE_16
#define TBAG_PP_SEQ_SIZE_16(_)   TBAG_PP_SEQ_SIZE_17
#define TBAG_PP_SEQ_SIZE_17(_)   TBAG_PP_SEQ_SIZE_18
#define TBAG_PP_SEQ_SIZE_18(_)   TBAG_PP_SEQ_SIZE_19
#define TBAG_PP_SEQ_SIZE_19(_)   TBAG_PP_SEQ_SIZE_20
#define TBAG_PP_SEQ_SIZE_20(_)   TBAG_PP_SEQ_SIZE_21
#define TBAG_PP_SEQ_SIZE_21(_)   TBAG_PP_SEQ_SIZE_22
#define TBAG_PP_SEQ_SIZE_22(_)   TBAG_PP_SEQ_SIZE_23
#define TBAG_PP_SEQ_SIZE_23(_)   TBAG_PP_SEQ_SIZE_24
#define TBAG_PP_SEQ_SIZE_24(_)   TBAG_PP_SEQ_SIZE_25
#define TBAG_PP_SEQ_SIZE_25(_)   TBAG_PP_SEQ_SIZE_26
#define TBAG_PP_SEQ_SIZE_26(_)   TBAG_PP_SEQ_SIZE_27
#define TBAG_PP_SEQ_SIZE_27(_)   TBAG_PP_SEQ_SIZE_28
#define TBAG_PP_SEQ_SIZE_28(_)   TBAG_PP_SEQ_SIZE_29
#define TBAG_PP_SEQ_SIZE_29(_)   TBAG_PP_SEQ_SIZE_30
#define TBAG_PP_SEQ_SIZE_30(_)   TBAG_PP_SEQ_SIZE_31
#define TBAG_PP_SEQ_SIZE_31(_)   TBAG_PP_SEQ_SIZE_32
#define TBAG_PP_SEQ_SIZE_32(_)   TBAG_PP_SEQ_SIZE_33
#define TBAG_PP_SEQ_SIZE_33(_)   TBAG_PP_SEQ_SIZE_34
#define TBAG_PP_SEQ_SIZE_34(_)   TBAG_PP_SEQ_SIZE_35
#define TBAG_PP_SEQ_SIZE_35(_)   TBAG_PP_SEQ_SIZE_36
#define TBAG_PP_SEQ_SIZE_36(_)   TBAG_PP_SEQ_SIZE_37
#define TBAG_PP_SEQ_SIZE_37(_)   TBAG_PP_SEQ_SIZE_38
#define TBAG_PP_SEQ_SIZE_38(_)   TBAG_PP_SEQ_SIZE_39
#define TBAG_PP_SEQ_SIZE_39(_)   TBAG_PP_SEQ_SIZE_40
#define TBAG_PP_SEQ_SIZE_40(_)   TBAG_PP_SEQ_SIZE_41
#define TBAG_PP_SEQ_SIZE_41(_)   TBAG_PP_SEQ_SIZE_42
#define TBAG_PP_SEQ_SIZE_42(_)   TBAG_PP_SEQ_SIZE_43
#define TBAG_PP_SEQ_SIZE_43(_)   TBAG_PP_SEQ_SIZE_44
#define TBAG_PP_SEQ_SIZE_44(_)   TBAG_PP_SEQ_SIZE_45
#define TBAG_PP_SEQ_SIZE_45(_)   TBAG_PP_SEQ_SIZE_46
#define TBAG_PP_SEQ_SIZE_46(_)   TBAG_PP_SEQ_SIZE_47
#define TBAG_PP_SEQ_SIZE_47(_)   TBAG_PP_SEQ_SIZE_48
#define TBAG_PP_SEQ_SIZE_48(_)   TBAG_PP_SEQ_SIZE_49
#define TBAG_PP_SEQ_SIZE_49(_)   TBAG_PP_SEQ_SIZE_50
#define TBAG_PP_SEQ_SIZE_50(_)   TBAG_PP_SEQ_SIZE_51
#define TBAG_PP_SEQ_SIZE_51(_)   TBAG_PP_SEQ_SIZE_52
#define TBAG_PP_SEQ_SIZE_52(_)   TBAG_PP_SEQ_SIZE_53
#define TBAG_PP_SEQ_SIZE_53(_)   TBAG_PP_SEQ_SIZE_54
#define TBAG_PP_SEQ_SIZE_54(_)   TBAG_PP_SEQ_SIZE_55
#define TBAG_PP_SEQ_SIZE_55(_)   TBAG_PP_SEQ_SIZE_56
#define TBAG_PP_SEQ_SIZE_56(_)   TBAG_PP_SEQ_SIZE_57
#define TBAG_PP_SEQ_SIZE_57(_)   TBAG_PP_SEQ_SIZE_58
#define TBAG_PP_SEQ_SIZE_58(_)   TBAG_PP_SEQ_SIZE_59
#define TBAG_PP_SEQ_SIZE_59(_)   TBAG_PP_SEQ_SIZE_60
#define TBAG_PP_SEQ_SIZE_60(_)   TBAG_PP_SEQ_SIZE_61
#define TBAG_PP_SEQ_SIZE_61(_)   TBAG_PP_SEQ_SIZE_62
#define TBAG_PP_SEQ_SIZE_62(_)   TBAG_PP_SEQ_SIZE_63
#define TBAG_PP_SEQ_SIZE_63(_)   TBAG_PP_SEQ_SIZE_64
#define TBAG_PP_SEQ_SIZE_64(_)   TBAG_PP_SEQ_SIZE_65
#define TBAG_PP_SEQ_SIZE_65(_)   TBAG_PP_SEQ_SIZE_66
#define TBAG_PP_SEQ_SIZE_66(_)   TBAG_PP_SEQ_SIZE_67
#define TBAG_PP_SEQ_SIZE_67(_)   TBAG_PP_SEQ_SIZE_68
#define TBAG_PP_SEQ_SIZE_68(_)   TBAG_PP_SEQ_SIZE_69
#define TBAG_PP_SEQ_SIZE_69(_)   TBAG_PP_SEQ_SIZE_70
#define TBAG_PP_SEQ_SIZE_70(_)   TBAG_PP_SEQ_SIZE_71
#define TBAG_PP_SEQ_SIZE_71(_)   TBAG_PP_SEQ_SIZE_72
#define TBAG_PP_SEQ_SIZE_72(_)   TBAG_PP_SEQ_SIZE_73
#define TBAG_PP_SEQ_SIZE_73(_)   TBAG_PP_SEQ_SIZE_74
#define TBAG_PP_SEQ_SIZE_74(_)   TBAG_PP_SEQ_SIZE_75
#define TBAG_PP_SEQ_SIZE_75(_)   TBAG_PP_SEQ_SIZE_76
#define TBAG_PP_SEQ_SIZE_76(_)   TBAG_PP_SEQ_SIZE_77
#define TBAG_PP_SEQ_SIZE_77(_)   TBAG_PP_SEQ_SIZE_78
#define TBAG_PP_SEQ_SIZE_78(_)   TBAG_PP_SEQ_SIZE_79
#define TBAG_PP_SEQ_SIZE_79(_)   TBAG_PP_SEQ_SIZE_80
#define TBAG_PP_SEQ_SIZE_80(_)   TBAG_PP_SEQ_SIZE_81
#define TBAG_PP_SEQ_SIZE_81(_)   TBAG_PP_SEQ_SIZE_82
#define TBAG_PP_SEQ_SIZE_82(_)   TBAG_PP_SEQ_SIZE_83
#define TBAG_PP_SEQ_SIZE_83(_)   TBAG_PP_SEQ_SIZE_84
#define TBAG_PP_SEQ_SIZE_84(_)   TBAG_PP_SEQ_SIZE_85
#define TBAG_PP_SEQ_SIZE_85(_)   TBAG_PP_SEQ_SIZE_86
#define TBAG_PP_SEQ_SIZE_86(_)   TBAG_PP_SEQ_SIZE_87
#define TBAG_PP_SEQ_SIZE_87(_)   TBAG_PP_SEQ_SIZE_88
#define TBAG_PP_SEQ_SIZE_88(_)   TBAG_PP_SEQ_SIZE_89
#define TBAG_PP_SEQ_SIZE_89(_)   TBAG_PP_SEQ_SIZE_90
#define TBAG_PP_SEQ_SIZE_90(_)   TBAG_PP_SEQ_SIZE_91
#define TBAG_PP_SEQ_SIZE_91(_)   TBAG_PP_SEQ_SIZE_92
#define TBAG_PP_SEQ_SIZE_92(_)   TBAG_PP_SEQ_SIZE_93
#define TBAG_PP_SEQ_SIZE_93(_)   TBAG_PP_SEQ_SIZE_94
#define TBAG_PP_SEQ_SIZE_94(_)   TBAG_PP_SEQ_SIZE_95
#define TBAG_PP_SEQ_SIZE_95(_)   TBAG_PP_SEQ_SIZE_96
#define TBAG_PP_SEQ_SIZE_96(_)   TBAG_PP_SEQ_SIZE_97
#define TBAG_PP_SEQ_SIZE_97(_)   TBAG_PP_SEQ_SIZE_98
#define TBAG_PP_SEQ_SIZE_98(_)   TBAG_PP_SEQ_SIZE_99
#define TBAG_PP_SEQ_SIZE_99(_)   TBAG_PP_SEQ_SIZE_100
#define TBAG_PP_SEQ_SIZE_100(_)  TBAG_PP_SEQ_SIZE_101
#define TBAG_PP_SEQ_SIZE_101(_)  TBAG_PP_SEQ_SIZE_102
#define TBAG_PP_SEQ_SIZE_102(_)  TBAG_PP_SEQ_SIZE_103
#define TBAG_PP_SEQ_SIZE_103(_)  TBAG_PP_SEQ_SIZE_104
#define TBAG_PP_SEQ_SIZE_104(_)  TBAG_PP_SEQ_SIZE_105
#define TBAG_PP_SEQ_SIZE_105(_)  TBAG_PP_SEQ_SIZE_106
#define TBAG_PP_SEQ_SIZE_106(_)  TBAG_PP_SEQ_SIZE_107
#define TBAG_PP_SEQ_SIZE_107(_)  TBAG_PP_SEQ_SIZE_108
#define TBAG_PP_SEQ_SIZE_108(_)  TBAG_PP_SEQ_SIZE_109
#define TBAG_PP_SEQ_SIZE_109(_)  TBAG_PP_SEQ_SIZE_110
#define TBAG_PP_SEQ_SIZE_110(_)  TBAG_PP_SEQ_SIZE_111
#define TBAG_PP_SEQ_SIZE_111(_)  TBAG_PP_SEQ_SIZE_112
#define TBAG_PP_SEQ_SIZE_112(_)  TBAG_PP_SEQ_SIZE_113
#define TBAG_PP_SEQ_SIZE_113(_)  TBAG_PP_SEQ_SIZE_114
#define TBAG_PP_SEQ_SIZE_114(_)  TBAG_PP_SEQ_SIZE_115
#define TBAG_PP_SEQ_SIZE_115(_)  TBAG_PP_SEQ_SIZE_116
#define TBAG_PP_SEQ_SIZE_116(_)  TBAG_PP_SEQ_SIZE_117
#define TBAG_PP_SEQ_SIZE_117(_)  TBAG_PP_SEQ_SIZE_118
#define TBAG_PP_SEQ_SIZE_118(_)  TBAG_PP_SEQ_SIZE_119
#define TBAG_PP_SEQ_SIZE_119(_)  TBAG_PP_SEQ_SIZE_120
#define TBAG_PP_SEQ_SIZE_120(_)  TBAG_PP_SEQ_SIZE_121
#define TBAG_PP_SEQ_SIZE_121(_)  TBAG_PP_SEQ_SIZE_122
#define TBAG_PP_SEQ_SIZE_122(_)  TBAG_PP_SEQ_SIZE_123
#define TBAG_PP_SEQ_SIZE_123(_)  TBAG_PP_SEQ_SIZE_124
#define TBAG_PP_SEQ_SIZE_124(_)  TBAG_PP_SEQ_SIZE_125
#define TBAG_PP_SEQ_SIZE_125(_)  TBAG_PP_SEQ_SIZE_126
#define TBAG_PP_SEQ_SIZE_126(_)  TBAG_PP_SEQ_SIZE_127
#define TBAG_PP_SEQ_SIZE_127(_)  TBAG_PP_SEQ_SIZE_128
#define TBAG_PP_SEQ_SIZE_128(_)  TBAG_PP_SEQ_SIZE_129
#define TBAG_PP_SEQ_SIZE_129(_)  TBAG_PP_SEQ_SIZE_130
#define TBAG_PP_SEQ_SIZE_130(_)  TBAG_PP_SEQ_SIZE_131
#define TBAG_PP_SEQ_SIZE_131(_)  TBAG_PP_SEQ_SIZE_132
#define TBAG_PP_SEQ_SIZE_132(_)  TBAG_PP_SEQ_SIZE_133
#define TBAG_PP_SEQ_SIZE_133(_)  TBAG_PP_SEQ_SIZE_134
#define TBAG_PP_SEQ_SIZE_134(_)  TBAG_PP_SEQ_SIZE_135
#define TBAG_PP_SEQ_SIZE_135(_)  TBAG_PP_SEQ_SIZE_136
#define TBAG_PP_SEQ_SIZE_136(_)  TBAG_PP_SEQ_SIZE_137
#define TBAG_PP_SEQ_SIZE_137(_)  TBAG_PP_SEQ_SIZE_138
#define TBAG_PP_SEQ_SIZE_138(_)  TBAG_PP_SEQ_SIZE_139
#define TBAG_PP_SEQ_SIZE_139(_)  TBAG_PP_SEQ_SIZE_140
#define TBAG_PP_SEQ_SIZE_140(_)  TBAG_PP_SEQ_SIZE_141
#define TBAG_PP_SEQ_SIZE_141(_)  TBAG_PP_SEQ_SIZE_142
#define TBAG_PP_SEQ_SIZE_142(_)  TBAG_PP_SEQ_SIZE_143
#define TBAG_PP_SEQ_SIZE_143(_)  TBAG_PP_SEQ_SIZE_144
#define TBAG_PP_SEQ_SIZE_144(_)  TBAG_PP_SEQ_SIZE_145
#define TBAG_PP_SEQ_SIZE_145(_)  TBAG_PP_SEQ_SIZE_146
#define TBAG_PP_SEQ_SIZE_146(_)  TBAG_PP_SEQ_SIZE_147
#define TBAG_PP_SEQ_SIZE_147(_)  TBAG_PP_SEQ_SIZE_148
#define TBAG_PP_SEQ_SIZE_148(_)  TBAG_PP_SEQ_SIZE_149
#define TBAG_PP_SEQ_SIZE_149(_)  TBAG_PP_SEQ_SIZE_150
#define TBAG_PP_SEQ_SIZE_150(_)  TBAG_PP_SEQ_SIZE_151
#define TBAG_PP_SEQ_SIZE_151(_)  TBAG_PP_SEQ_SIZE_152
#define TBAG_PP_SEQ_SIZE_152(_)  TBAG_PP_SEQ_SIZE_153
#define TBAG_PP_SEQ_SIZE_153(_)  TBAG_PP_SEQ_SIZE_154
#define TBAG_PP_SEQ_SIZE_154(_)  TBAG_PP_SEQ_SIZE_155
#define TBAG_PP_SEQ_SIZE_155(_)  TBAG_PP_SEQ_SIZE_156
#define TBAG_PP_SEQ_SIZE_156(_)  TBAG_PP_SEQ_SIZE_157
#define TBAG_PP_SEQ_SIZE_157(_)  TBAG_PP_SEQ_SIZE_158
#define TBAG_PP_SEQ_SIZE_158(_)  TBAG_PP_SEQ_SIZE_159
#define TBAG_PP_SEQ_SIZE_159(_)  TBAG_PP_SEQ_SIZE_160
#define TBAG_PP_SEQ_SIZE_160(_)  TBAG_PP_SEQ_SIZE_161
#define TBAG_PP_SEQ_SIZE_161(_)  TBAG_PP_SEQ_SIZE_162
#define TBAG_PP_SEQ_SIZE_162(_)  TBAG_PP_SEQ_SIZE_163
#define TBAG_PP_SEQ_SIZE_163(_)  TBAG_PP_SEQ_SIZE_164
#define TBAG_PP_SEQ_SIZE_164(_)  TBAG_PP_SEQ_SIZE_165
#define TBAG_PP_SEQ_SIZE_165(_)  TBAG_PP_SEQ_SIZE_166
#define TBAG_PP_SEQ_SIZE_166(_)  TBAG_PP_SEQ_SIZE_167
#define TBAG_PP_SEQ_SIZE_167(_)  TBAG_PP_SEQ_SIZE_168
#define TBAG_PP_SEQ_SIZE_168(_)  TBAG_PP_SEQ_SIZE_169
#define TBAG_PP_SEQ_SIZE_169(_)  TBAG_PP_SEQ_SIZE_170
#define TBAG_PP_SEQ_SIZE_170(_)  TBAG_PP_SEQ_SIZE_171
#define TBAG_PP_SEQ_SIZE_171(_)  TBAG_PP_SEQ_SIZE_172
#define TBAG_PP_SEQ_SIZE_172(_)  TBAG_PP_SEQ_SIZE_173
#define TBAG_PP_SEQ_SIZE_173(_)  TBAG_PP_SEQ_SIZE_174
#define TBAG_PP_SEQ_SIZE_174(_)  TBAG_PP_SEQ_SIZE_175
#define TBAG_PP_SEQ_SIZE_175(_)  TBAG_PP_SEQ_SIZE_176
#define TBAG_PP_SEQ_SIZE_176(_)  TBAG_PP_SEQ_SIZE_177
#define TBAG_PP_SEQ_SIZE_177(_)  TBAG_PP_SEQ_SIZE_178
#define TBAG_PP_SEQ_SIZE_178(_)  TBAG_PP_SEQ_SIZE_179
#define TBAG_PP_SEQ_SIZE_179(_)  TBAG_PP_SEQ_SIZE_180
#define TBAG_PP_SEQ_SIZE_180(_)  TBAG_PP_SEQ_SIZE_181
#define TBAG_PP_SEQ_SIZE_181(_)  TBAG_PP_SEQ_SIZE_182
#define TBAG_PP_SEQ_SIZE_182(_)  TBAG_PP_SEQ_SIZE_183
#define TBAG_PP_SEQ_SIZE_183(_)  TBAG_PP_SEQ_SIZE_184
#define TBAG_PP_SEQ_SIZE_184(_)  TBAG_PP_SEQ_SIZE_185
#define TBAG_PP_SEQ_SIZE_185(_)  TBAG_PP_SEQ_SIZE_186
#define TBAG_PP_SEQ_SIZE_186(_)  TBAG_PP_SEQ_SIZE_187
#define TBAG_PP_SEQ_SIZE_187(_)  TBAG_PP_SEQ_SIZE_188
#define TBAG_PP_SEQ_SIZE_188(_)  TBAG_PP_SEQ_SIZE_189
#define TBAG_PP_SEQ_SIZE_189(_)  TBAG_PP_SEQ_SIZE_190
#define TBAG_PP_SEQ_SIZE_190(_)  TBAG_PP_SEQ_SIZE_191
#define TBAG_PP_SEQ_SIZE_191(_)  TBAG_PP_SEQ_SIZE_192
#define TBAG_PP_SEQ_SIZE_192(_)  TBAG_PP_SEQ_SIZE_193
#define TBAG_PP_SEQ_SIZE_193(_)  TBAG_PP_SEQ_SIZE_194
#define TBAG_PP_SEQ_SIZE_194(_)  TBAG_PP_SEQ_SIZE_195
#define TBAG_PP_SEQ_SIZE_195(_)  TBAG_PP_SEQ_SIZE_196
#define TBAG_PP_SEQ_SIZE_196(_)  TBAG_PP_SEQ_SIZE_197
#define TBAG_PP_SEQ_SIZE_197(_)  TBAG_PP_SEQ_SIZE_198
#define TBAG_PP_SEQ_SIZE_198(_)  TBAG_PP_SEQ_SIZE_199
#define TBAG_PP_SEQ_SIZE_199(_)  TBAG_PP_SEQ_SIZE_200
#define TBAG_PP_SEQ_SIZE_200(_)  TBAG_PP_SEQ_SIZE_201
#define TBAG_PP_SEQ_SIZE_201(_)  TBAG_PP_SEQ_SIZE_202
#define TBAG_PP_SEQ_SIZE_202(_)  TBAG_PP_SEQ_SIZE_203
#define TBAG_PP_SEQ_SIZE_203(_)  TBAG_PP_SEQ_SIZE_204
#define TBAG_PP_SEQ_SIZE_204(_)  TBAG_PP_SEQ_SIZE_205
#define TBAG_PP_SEQ_SIZE_205(_)  TBAG_PP_SEQ_SIZE_206
#define TBAG_PP_SEQ_SIZE_206(_)  TBAG_PP_SEQ_SIZE_207
#define TBAG_PP_SEQ_SIZE_207(_)  TBAG_PP_SEQ_SIZE_208
#define TBAG_PP_SEQ_SIZE_208(_)  TBAG_PP_SEQ_SIZE_209
#define TBAG_PP_SEQ_SIZE_209(_)  TBAG_PP_SEQ_SIZE_210
#define TBAG_PP_SEQ_SIZE_210(_)  TBAG_PP_SEQ_SIZE_211
#define TBAG_PP_SEQ_SIZE_211(_)  TBAG_PP_SEQ_SIZE_212
#define TBAG_PP_SEQ_SIZE_212(_)  TBAG_PP_SEQ_SIZE_213
#define TBAG_PP_SEQ_SIZE_213(_)  TBAG_PP_SEQ_SIZE_214
#define TBAG_PP_SEQ_SIZE_214(_)  TBAG_PP_SEQ_SIZE_215
#define TBAG_PP_SEQ_SIZE_215(_)  TBAG_PP_SEQ_SIZE_216
#define TBAG_PP_SEQ_SIZE_216(_)  TBAG_PP_SEQ_SIZE_217
#define TBAG_PP_SEQ_SIZE_217(_)  TBAG_PP_SEQ_SIZE_218
#define TBAG_PP_SEQ_SIZE_218(_)  TBAG_PP_SEQ_SIZE_219
#define TBAG_PP_SEQ_SIZE_219(_)  TBAG_PP_SEQ_SIZE_220
#define TBAG_PP_SEQ_SIZE_220(_)  TBAG_PP_SEQ_SIZE_221
#define TBAG_PP_SEQ_SIZE_221(_)  TBAG_PP_SEQ_SIZE_222
#define TBAG_PP_SEQ_SIZE_222(_)  TBAG_PP_SEQ_SIZE_223
#define TBAG_PP_SEQ_SIZE_223(_)  TBAG_PP_SEQ_SIZE_224
#define TBAG_PP_SEQ_SIZE_224(_)  TBAG_PP_SEQ_SIZE_225
#define TBAG_PP_SEQ_SIZE_225(_)  TBAG_PP_SEQ_SIZE_226
#define TBAG_PP_SEQ_SIZE_226(_)  TBAG_PP_SEQ_SIZE_227
#define TBAG_PP_SEQ_SIZE_227(_)  TBAG_PP_SEQ_SIZE_228
#define TBAG_PP_SEQ_SIZE_228(_)  TBAG_PP_SEQ_SIZE_229
#define TBAG_PP_SEQ_SIZE_229(_)  TBAG_PP_SEQ_SIZE_230
#define TBAG_PP_SEQ_SIZE_230(_)  TBAG_PP_SEQ_SIZE_231
#define TBAG_PP_SEQ_SIZE_231(_)  TBAG_PP_SEQ_SIZE_232
#define TBAG_PP_SEQ_SIZE_232(_)  TBAG_PP_SEQ_SIZE_233
#define TBAG_PP_SEQ_SIZE_233(_)  TBAG_PP_SEQ_SIZE_234
#define TBAG_PP_SEQ_SIZE_234(_)  TBAG_PP_SEQ_SIZE_235
#define TBAG_PP_SEQ_SIZE_235(_)  TBAG_PP_SEQ_SIZE_236
#define TBAG_PP_SEQ_SIZE_236(_)  TBAG_PP_SEQ_SIZE_237
#define TBAG_PP_SEQ_SIZE_237(_)  TBAG_PP_SEQ_SIZE_238
#define TBAG_PP_SEQ_SIZE_238(_)  TBAG_PP_SEQ_SIZE_239
#define TBAG_PP_SEQ_SIZE_239(_)  TBAG_PP_SEQ_SIZE_240
#define TBAG_PP_SEQ_SIZE_240(_)  TBAG_PP_SEQ_SIZE_241
#define TBAG_PP_SEQ_SIZE_241(_)  TBAG_PP_SEQ_SIZE_242
#define TBAG_PP_SEQ_SIZE_242(_)  TBAG_PP_SEQ_SIZE_243
#define TBAG_PP_SEQ_SIZE_243(_)  TBAG_PP_SEQ_SIZE_244
#define TBAG_PP_SEQ_SIZE_244(_)  TBAG_PP_SEQ_SIZE_245
#define TBAG_PP_SEQ_SIZE_245(_)  TBAG_PP_SEQ_SIZE_246
#define TBAG_PP_SEQ_SIZE_246(_)  TBAG_PP_SEQ_SIZE_247
#define TBAG_PP_SEQ_SIZE_247(_)  TBAG_PP_SEQ_SIZE_248
#define TBAG_PP_SEQ_SIZE_248(_)  TBAG_PP_SEQ_SIZE_249
#define TBAG_PP_SEQ_SIZE_249(_)  TBAG_PP_SEQ_SIZE_250
#define TBAG_PP_SEQ_SIZE_250(_)  TBAG_PP_SEQ_SIZE_251
#define TBAG_PP_SEQ_SIZE_251(_)  TBAG_PP_SEQ_SIZE_252
#define TBAG_PP_SEQ_SIZE_252(_)  TBAG_PP_SEQ_SIZE_253
#define TBAG_PP_SEQ_SIZE_253(_)  TBAG_PP_SEQ_SIZE_254
#define TBAG_PP_SEQ_SIZE_254(_)  TBAG_PP_SEQ_SIZE_255
#define TBAG_PP_SEQ_SIZE_255(_)  TBAG_PP_SEQ_SIZE_256
#define TBAG_PP_SEQ_SIZE_256(_)  TBAG_PP_SEQ_SIZE_257

#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_0      0
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_1      1
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_2      2
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_3      3
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_4      4
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_5      5
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_6      6
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_7      7
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_8      8
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_9      9
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_10    10
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_11    11
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_12    12
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_13    13
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_14    14
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_15    15
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_16    16
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_17    17
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_18    18
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_19    19
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_20    20
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_21    21
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_22    22
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_23    23
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_24    24
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_25    25
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_26    26
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_27    27
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_28    28
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_29    29
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_30    30
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_31    31
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_32    32
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_33    33
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_34    34
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_35    35
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_36    36
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_37    37
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_38    38
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_39    39
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_40    40
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_41    41
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_42    42
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_43    43
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_44    44
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_45    45
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_46    46
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_47    47
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_48    48
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_49    49
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_50    50
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_51    51
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_52    52
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_53    53
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_54    54
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_55    55
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_56    56
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_57    57
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_58    58
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_59    59
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_60    60
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_61    61
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_62    62
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_63    63
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_64    64
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_65    65
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_66    66
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_67    67
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_68    68
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_69    69
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_70    70
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_71    71
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_72    72
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_73    73
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_74    74
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_75    75
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_76    76
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_77    77
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_78    78
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_79    79
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_80    80
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_81    81
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_82    82
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_83    83
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_84    84
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_85    85
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_86    86
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_87    87
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_88    88
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_89    89
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_90    90
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_91    91
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_92    92
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_93    93
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_94    94
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_95    95
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_96    96
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_97    97
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_98    98
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_99    99
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_100  100
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_101  101
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_102  102
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_103  103
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_104  104
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_105  105
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_106  106
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_107  107
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_108  108
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_109  109
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_110  110
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_111  111
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_112  112
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_113  113
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_114  114
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_115  115
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_116  116
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_117  117
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_118  118
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_119  119
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_120  120
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_121  121
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_122  122
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_123  123
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_124  124
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_125  125
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_126  126
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_127  127
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_128  128
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_129  129
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_130  130
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_131  131
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_132  132
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_133  133
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_134  134
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_135  135
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_136  136
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_137  137
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_138  138
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_139  139
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_140  140
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_141  141
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_142  142
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_143  143
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_144  144
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_145  145
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_146  146
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_147  147
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_148  148
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_149  149
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_150  150
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_151  151
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_152  152
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_153  153
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_154  154
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_155  155
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_156  156
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_157  157
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_158  158
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_159  159
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_160  160
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_161  161
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_162  162
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_163  163
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_164  164
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_165  165
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_166  166
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_167  167
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_168  168
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_169  169
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_170  170
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_171  171
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_172  172
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_173  173
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_174  174
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_175  175
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_176  176
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_177  177
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_178  178
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_179  179
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_180  180
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_181  181
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_182  182
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_183  183
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_184  184
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_185  185
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_186  186
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_187  187
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_188  188
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_189  189
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_190  190
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_191  191
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_192  192
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_193  193
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_194  194
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_195  195
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_196  196
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_197  197
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_198  198
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_199  199
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_200  200
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_201  201
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_202  202
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_203  203
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_204  204
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_205  205
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_206  206
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_207  207
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_208  208
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_209  209
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_210  210
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_211  211
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_212  212
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_213  213
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_214  214
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_215  215
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_216  216
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_217  217
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_218  218
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_219  219
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_220  220
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_221  221
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_222  222
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_223  223
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_224  224
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_225  225
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_226  226
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_227  227
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_228  228
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_229  229
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_230  230
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_231  231
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_232  232
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_233  233
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_234  234
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_235  235
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_236  236
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_237  237
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_238  238
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_239  239
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_240  240
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_241  241
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_242  242
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_243  243
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_244  244
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_245  245
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_246  246
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_247  247
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_248  248
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_249  249
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_250  250
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_251  251
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_252  252
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_253  253
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_254  254
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_255  255
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_256  256
#define TBAG_PP_SEQ_SIZE_TBAG_PP_SEQ_SIZE_257  257

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_SEQ_SEQSIZE_HPP__

