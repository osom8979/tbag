
#ifndef NNG_SUPPLEMENTAL_UTIL_PLATFORM_H
#define NNG_SUPPLEMENTAL_UTIL_PLATFORM_H



#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t nng_time;

/*NNG_DECL nng_time nng_clock(void);*/

/*NNG_DECL void nng_msleep(nng_duration);*/

typedef struct nng_thread nng_thread;

/*NNG_DECL int nng_thread_create(nng_thread **, void (*)(void *), void *);*/

/*NNG_DECL void nng_thread_destroy(nng_thread *);*/

typedef struct nng_mtx nng_mtx;

/*NNG_DECL int nng_mtx_alloc(nng_mtx **);*/

/*NNG_DECL void nng_mtx_free(nng_mtx *);*/

/*NNG_DECL void nng_mtx_lock(nng_mtx *);*/

/*NNG_DECL void nng_mtx_unlock(nng_mtx *);*/

typedef struct nng_cv nng_cv;

/*NNG_DECL int nng_cv_alloc(nng_cv **, nng_mtx *);*/

/*NNG_DECL void nng_cv_free(nng_cv *);*/

/*NNG_DECL void nng_cv_wait(nng_cv *);*/

/*NNG_DECL int nng_cv_until(nng_cv *, nng_time);*/

/*NNG_DECL void nng_cv_wake(nng_cv *);*/

/*NNG_DECL void nng_cv_wake1(nng_cv *);*/

/*NNG_DECL uint32_t nng_random(void);*/

#ifdef __cplusplus
}
#endif

#endif // NNG_SUPPLEMENTAL_UTIL_PLATFORM_H
