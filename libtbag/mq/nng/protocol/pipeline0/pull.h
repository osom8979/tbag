
#ifndef NNG_PROTOCOL_PIPELINE0_PULL_H
#define NNG_PROTOCOL_PIPELINE0_PULL_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_pull0_open(nng_socket *);*/
/*NNG_DECL int nng_pull0_open_raw(nng_socket *);*/

#ifndef nng_pull_open
#define nng_pull_open nng_pull0_open
#endif

#ifndef nng_pull_open_raw
#define nng_pull_open_raw nng_pull0_open_raw
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_PIPELINE0_PULL_H
