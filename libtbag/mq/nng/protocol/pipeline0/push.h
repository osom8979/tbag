
#ifndef NNG_PROTOCOL_PIPELINE0_PUSH_H
#define NNG_PROTOCOL_PIPELINE0_PUSH_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_push0_open(nng_socket *);*/
/*NNG_DECL int nng_push0_open_raw(nng_socket *);*/

#ifndef nng_push_open
#define nng_push_open nng_push0_open
#endif

#ifndef nng_push_open_raw
#define nng_push_open_raw nng_push0_open_raw
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_PIPELINE0_PUSH_H
