
#ifndef NNG_PROTOCOL_PUBSUB0_PUB_H
#define NNG_PROTOCOL_PUBSUB0_PUB_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_pub0_open(nng_socket *);*/
/*NNG_DECL int nng_pub0_open_raw(nng_socket *);*/

#ifndef nng_pub_open
#define nng_pub_open nng_pub0_open
#endif

#ifndef nng_pub_open_raw
#define nng_pub_open_raw nng_pub0_open_raw
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_PUBSUB0_PUB_H
