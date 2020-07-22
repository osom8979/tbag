
#ifndef NNG_PROTOCOL_SURVEY0_RESPOND_H
#define NNG_PROTOCOL_SURVEY0_RESPOND_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_respondent0_open(nng_socket *);*/
/*NNG_DECL int nng_respondent0_open_raw(nng_socket *);*/

#ifndef nng_respondent_open
#define nng_respondent_open nng_respondent0_open
#endif

#ifndef nng_respondent_open_raw
#define nng_respondent_open_raw nng_respondent0_open_raw
#endif

#define NNG_RESPONDENT0_SELF 0x63
#define NNG_RESPONDENT0_PEER 0x62
#define NNG_RESPONDENT0_SELF_NAME "respondent"
#define NNG_RESPONDENT0_PEER_NAME "surveyor"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_SURVEY0_RESPOND_H
