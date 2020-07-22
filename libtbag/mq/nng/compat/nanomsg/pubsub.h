
#ifndef NNG_COMPAT_PUBSUB_H
#define NNG_COMPAT_PUBSUB_H


#ifdef __cplusplus
extern "C" {
#endif

#define NN_PROTO_PUBSUB 2
#define NN_PUB (NN_PROTO_PUBSUB * 16 + 0)
#define NN_SUB (NN_PROTO_PUBSUB * 16 + 1)

#define NN_SUB_SUBSCRIBE (NN_SUB * 16 + 1)
#define NN_SUB_UNSUBSCRIBE (NN_SUB * 16 + 2)

#ifdef __cplusplus
}
#endif

#endif // NNG_COMPAT_PUBSUB_H
