
#ifndef NNG_COMPAT_PIPELINE_H
#define NNG_COMPAT_PIPELINE_H


#ifdef __cplusplus
extern "C" {
#endif

#define NN_PROTO_PIPELINE 5
#define NN_PUSH (NN_PROTO_PIPELINE * 16 + 0)
#define NN_PULL (NN_PROTO_PIPELINE * 16 + 1)


#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_PIPELINE_H
