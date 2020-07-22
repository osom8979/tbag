
#ifndef NNG_PROTOCOL_BUS0_BUS_H
#define NNG_PROTOCOL_BUS0_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_bus0_open(nng_socket *);*/

/*NNG_DECL int nng_bus0_open_raw(nng_socket *);*/

#ifndef nng_bus_open
#define nng_bus_open nng_bus0_open
#endif

#ifndef nng_bus_open_raw
#define nng_bus_open_raw nng_bus0_open_raw
#endif

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_BUS0_BUS_H
