/*
 * Copyright (c) 2008, 2009, 2010, 2011
 *   Jonathan Schleifer <js@webkeks.org>
 *
 * All rights reserved.
 *
 * This file is part of ObjFW. It may be distributed under the terms of the
 * Q Public License 1.0, which can be found in the file LICENSE.QPL included in
 * the packaging of this file.
 *
 * Alternatively, it may be distributed under the terms of the GNU General
 * Public License, either version 2 or 3, which can be found in the file
 * LICENSE.GPLv2 or LICENSE.GPLv3 respectively included in the packaging of this
 * file.
 */

#import "objfw-defs.h"

#if !defined(OF_THREADS) || (!defined(OF_HAVE_PTHREADS) && !defined(_WIN32))
# error No threads available!
#endif

#import "macros.h"

#if defined(OF_HAVE_PTHREADS)
# include <pthread.h>
typedef pthread_t of_thread_t;
typedef pthread_key_t of_tlskey_t;
typedef pthread_mutex_t of_mutex_t;
typedef pthread_cond_t of_condition_t;
#elif defined(_WIN32)
# include <windows.h>
typedef HANDLE of_thread_t;
typedef DWORD of_tlskey_t;
typedef CRITICAL_SECTION of_mutex_t;
typedef struct {
	HANDLE event;
	int count;
} of_condition_t;
#endif

#if defined(OF_ATOMIC_OPS)
# import "atomic.h"
typedef volatile int of_spinlock_t;
# define OF_SPINCOUNT 10
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
typedef pthread_spinlock_t of_spinlock_t;
#else
typedef of_mutex_t of_spinlock_t;
#endif

#if defined(OF_HAVE_PTHREADS)
# define of_thread_is_current(t) pthread_equal(t, pthread_self())
# define of_thread_current() pthread_self()
#elif defined(_WIN32)
# define of_thread_is_current(t) (t == GetCurrentThread())
# define of_thread_current() GetCurrentThread()
#endif

static OF_INLINE BOOL
of_thread_new(of_thread_t *thread, id (*main)(id), id data)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_create(thread, NULL, (void*(*)(void*))main,
	    (void*)data);
#elif defined(_WIN32)
	*thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main,
	    (void*)data, 0, NULL);

	return (thread != NULL);
#endif
}

static OF_INLINE BOOL
of_thread_join(of_thread_t thread)
{
#if defined(OF_HAVE_PTHREADS)
	void *ret;

	if (pthread_join(thread, &ret))
		return NO;

	return (ret != PTHREAD_CANCELED);
#elif defined(_WIN32)
	if (WaitForSingleObject(thread, INFINITE))
		return NO;

	CloseHandle(thread);

	return YES;
#endif
}

static OF_INLINE void
of_thread_exit()
{
#if defined(OF_HAVE_PTHREADS)
	pthread_exit(NULL);
#elif defined(_WIN32)
	ExitThread(0);
#endif
}

static OF_INLINE BOOL
of_mutex_new(of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
	InitializeCriticalSection(mutex);
	return YES;
#endif
}

static OF_INLINE BOOL
of_mutex_free(of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
	DeleteCriticalSection(mutex);
	return YES;
#endif
}

static OF_INLINE BOOL
of_mutex_lock(of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_mutex_lock(mutex);
#elif defined(_WIN32)
	EnterCriticalSection(mutex);
	return YES;
#endif
}

static OF_INLINE BOOL
of_mutex_trylock(of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_mutex_trylock(mutex);
#elif defined(_WIN32)
	return TryEnterCriticalSection(mutex);
#endif
}

static OF_INLINE BOOL
of_mutex_unlock(of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
	LeaveCriticalSection(mutex);
	return YES;
#endif
}

static OF_INLINE BOOL
of_condition_new(of_condition_t *condition)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_cond_init(condition, NULL);
#elif defined(_WIN32)
	condition->count = 0;

	if ((condition->event = CreateEvent(NULL, FALSE, 0, NULL)) == NULL)
		return NO;

	return YES;
#endif
}

#if defined(OF_HAVE_PTHREADS)
static OF_INLINE BOOL
#elif defined(_WIN32)
static BOOL
#endif
of_condition_wait(of_condition_t *condition, of_mutex_t *mutex)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_cond_wait(condition, mutex);
#elif defined(_WIN32)
	if (!of_mutex_unlock(mutex))
		return NO;

	of_atomic_inc_int(&condition->count);

	if (WaitForSingleObject(condition->event, INFINITE) != WAIT_OBJECT_0) {
		of_mutex_lock(mutex);
		return NO;
	}

	of_atomic_dec_int(&condition->count);

	if (!of_mutex_lock(mutex))
		return NO;

	return YES;
#endif
}

static OF_INLINE BOOL
of_condition_signal(of_condition_t *condition)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_cond_signal(condition);
#elif defined(_WIN32)
	return SetEvent(condition->event);
#endif
}

#if defined(OF_HAVE_PTHREADS)
static OF_INLINE BOOL
#elif defined(_WIN32)
static BOOL
#endif
of_condition_broadcast(of_condition_t *condition)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_cond_broadcast(condition);
#elif defined(_WIN32)
	size_t i;

	for (i = 0; i < condition->count; i++)
		if (!SetEvent(condition->event))
			return NO;

	return YES;
#endif
}

#if defined(OF_HAVE_PTHREADS)
static OF_INLINE BOOL
#elif defined(_WIN32)
static BOOL
#endif
of_condition_free(of_condition_t *condition)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_cond_destroy(condition);
#elif defined(_WIN32)
	if (condition->count)
		return NO;

	return CloseHandle(condition->event);
#endif
}

static OF_INLINE BOOL
of_tlskey_new(of_tlskey_t *key)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_key_create(key, NULL);
#elif defined(_WIN32)
	return ((*key = TlsAlloc()) != TLS_OUT_OF_INDEXES);
#endif
}

static OF_INLINE id
of_tlskey_get(of_tlskey_t key)
{
#if defined(OF_HAVE_PTHREADS)
	void *ret = pthread_getspecific(key);
#elif defined(_WIN32)
	void *ret = TlsGetValue(key);
#endif

	/* NULL and nil might be different! */
	if (ret == NULL)
		return nil;

	return (id)ret;
}

static OF_INLINE BOOL
of_tlskey_set(of_tlskey_t key, id obj)
{
	void *p = (obj != nil ? (void*)obj : NULL);

#if defined(OF_HAVE_PTHREADS)
	return !pthread_setspecific(key, p);
#elif defined(_WIN32)
	return TlsSetValue(key, p);
#endif
}

static OF_INLINE BOOL
of_tlskey_free(of_tlskey_t key)
{
#if defined(OF_HAVE_PTHREADS)
	return !pthread_key_delete(key);
#elif defined(_WIN32)
	return TlsFree(key);
#endif
}

static OF_INLINE BOOL
of_spinlock_new(of_spinlock_t *s)
{
#if defined(OF_ATOMIC_OPS)
	*s = 0;
	return YES;
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
	return !pthread_spin_init(s, 0);
#else
	return of_mutex_new(s);
#endif
}

static OF_INLINE BOOL
of_spinlock_trylock(of_spinlock_t *s)
{
#if defined(OF_ATOMIC_OPS)
	return of_atomic_cmpswap_int(s, 0, 1);
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
	return !pthread_spin_trylock(s);
#else
	return of_mutex_trylock(s);
#endif
}

static OF_INLINE BOOL
of_spinlock_lock(of_spinlock_t *s)
{
#if defined(OF_ATOMIC_OPS)
# if defined(OF_HAVE_SCHED_YIELD) || defined(_WIN32)
	int i;

	for (i = 0; i < OF_SPINCOUNT; i++)
		if (of_spinlock_trylock(s))
			return YES;

	while (!of_spinlock_trylock(s))
#  ifndef _WIN32
		sched_yield();
#  else
		Sleep(0);
#  endif
# else
	while (!of_spinlock_trylock(s));
# endif

	return YES;
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
	return !pthread_spin_lock(s);
#else
	return of_mutex_lock(s);
#endif
}

static OF_INLINE BOOL
of_spinlock_unlock(of_spinlock_t *s)
{
#if defined(OF_ATOMIC_OPS)
	*s = 0;
	return YES;
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
	return !pthread_spin_unlock(s);
#else
	return of_mutex_unlock(s);
#endif
}

static OF_INLINE BOOL
of_spinlock_free(of_spinlock_t *s)
{
#if defined(OF_ATOMIC_OPS)
	return YES;
#elif defined(OF_HAVE_PTHREAD_SPINLOCKS)
	return !pthread_spin_destroy(s);
#else
	return of_mutex_free(s);
#endif
}
