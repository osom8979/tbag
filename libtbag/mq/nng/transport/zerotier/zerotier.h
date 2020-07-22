
#ifndef NNG_TRANSPORT_ZEROTIER_ZEROTIER_H
#define NNG_TRANSPORT_ZEROTIER_ZEROTIER_H

#include <nng/nng.h>



#define NNG_OPT_ZT_HOME "zt:home"

#define NNG_OPT_ZT_NWID "zt:nwid"

#define NNG_OPT_ZT_NODE "zt:node"

#define NNG_OPT_ZT_NETWORK_STATUS "zt:network-status"

#define NNG_OPT_ZT_NETWORK_NAME "zt:network-name"

#define NNG_OPT_ZT_PING_TIME "zt:ping-time"
#define NNG_OPT_ZT_PING_TRIES "zt:ping-tries"

#define NNG_OPT_ZT_CONN_TIME "zt:conn-time"
#define NNG_OPT_ZT_CONN_TRIES "zt:conn-tries"

#define NNG_OPT_ZT_MTU "zt:mtu"

#define NNG_OPT_ZT_ORBIT "zt:orbit"

#define NNG_OPT_ZT_DEORBIT "zt:deorbit"

#define NNG_OPT_ZT_ADD_LOCAL_ADDR "zt:add-local-addr"

#define NNG_OPT_ZT_CLEAR_LOCAL_ADDRS "zt:clear-local-addrs"

#ifdef __cplusplus
extern "C" {
#endif

enum nng_zt_status {
	NNG_ZT_STATUS_UP,
	NNG_ZT_STATUS_CONFIG,
	NNG_ZT_STATUS_DENIED,
	NNG_ZT_STATUS_NOTFOUND,
	NNG_ZT_STATUS_ERROR,
	NNG_ZT_STATUS_OBSOLETE,
	NNG_ZT_STATUS_UNKNOWN,
};

/*NNG_DECL int nng_zt_register(void);*/

#ifdef __cplusplus
}
#endif

#endif // NNG_TRANSPORT_ZEROTIER_ZEROTIER_H
