
#ifndef NNG_PROTOCOL_SURVEY0_SURVEY_H
#define NNG_PROTOCOL_SURVEY0_SURVEY_H

#ifdef __cplusplus
extern "C" {
#endif

/*NNG_DECL int nng_surveyor0_open(nng_socket *);*/
/*NNG_DECL int nng_surveyor0_open_raw(nng_socket *);*/

#ifndef nng_surveyor_open
#define nng_surveyor_open nng_surveyor0_open
#endif

#ifndef nng_surveyor_open_raw
#define nng_surveyor_open_raw nng_surveyor0_open_raw
#endif

#define NNG_SURVEYOR0_SELF 0x62
#define NNG_SURVEYOR0_PEER 0x63
#define NNG_SURVEYOR0_SELF_NAME "surveyor"
#define NNG_SURVEYOR0_PEER_NAME "respondent"

#define NNG_OPT_SURVEYOR_SURVEYTIME "surveyor:survey-time"

#ifdef __cplusplus
}
#endif

#endif // NNG_PROTOCOL_SURVEY0_SURVEY_H
