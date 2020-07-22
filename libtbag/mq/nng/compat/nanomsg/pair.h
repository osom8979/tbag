
#ifndef NNG_COMPAT_PAIR_H
#define NNG_COMPAT_PAIR_H


#ifdef __cplusplus
extern "C" {
#endif

#define NN_PROTO_PAIR 1
#define NN_PAIR (NN_PROTO_PAIR * 16 + 0)

#define NN_PAIR_v0 (NN_PROTO_PAIR * 16 + 0)
#define NN_PAIR_V1 (NN_PROTO_PAIR * 16 + 1)


#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_PAIR_H
