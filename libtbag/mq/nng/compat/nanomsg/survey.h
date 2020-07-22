
#ifndef NNG_COMPAT_SURVEY_H
#define NNG_COMPAT_SURVEY_H


#ifdef __cplusplus
extern "C" {
#endif

#define NN_PROTO_SURVEY 6
#define NN_SURVEYOR (NN_PROTO_SURVEY * 16 + 2)
#define NN_RESPONDENT (NN_PROTO_SURVEY * 16 + 3)


#define NN_SURVEYOR_DEADLINE (NN_SURVEYOR * 16 + 1)

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_SURVEY_H
