#ifndef NNG_PROTOCOL_PUBSUB0_SUB_H
#define NNG_PROTOCOL_PUBSUB0_SUB_H

#ifndef nng_sub_open
#define nng_sub_open nng_sub0_open
#endif

#ifndef nng_sub_open_raw
#define nng_sub_open_raw nng_sub0_open_raw
#endif

#define NNG_OPT_SUB_SUBSCRIBE "sub:subscribe"
#define NNG_OPT_SUB_UNSUBSCRIBE "sub:unsubscribe"

#endif // NNG_PROTOCOL_PUBSUB0_SUB_H
