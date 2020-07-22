
#ifndef NNG_PROTOCOL_PUBSUB0_SUB_H
#define NNG_PROTOCOL_PUBSUB0_SUB_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_sub0_open(nng_socket *);*/

/*NNG_DECL int nng_sub0_open_raw(nng_socket *);*/

#ifndef nng_sub_open
#define nng_sub_open nng_sub0_open
#endif

#ifndef nng_sub_open_raw
#define nng_sub_open_raw nng_sub0_open_raw
#endif

#define NNG_OPT_SUB_SUBSCRIBE "sub:subscribe"
#define NNG_OPT_SUB_UNSUBSCRIBE "sub:unsubscribe"

#define NNG_OPT_SUB_PREFNEW "sub:prefnew"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_PUBSUB0_SUB_H
