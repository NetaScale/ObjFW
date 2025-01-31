/*
 * Copyright (c) 2008-2022 Jonathan Schleifer <js@nil.im>
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

#include <libkern/OSAtomic.h>

static OF_INLINE int
OFAtomicIntAdd(volatile int *_Nonnull p, int i)
{
	return OSAtomicAdd32(i, p);
}

static OF_INLINE int32_t
OFAtomicInt32Add(volatile int32_t *_Nonnull p, int32_t i)
{
	return OSAtomicAdd32(i, p);
}

static OF_INLINE void *_Nullable
OFAtomicPointerAdd(void *volatile _Nullable *_Nonnull p, intptr_t i)
{
#ifdef __LP64__
	return (void *)OSAtomicAdd64(i, (int64_t *)p);
#else
	return (void *)OSAtomicAdd32(i, (int32_t *)p);
#endif
}

static OF_INLINE int
OFAtomicIntSubtract(volatile int *_Nonnull p, int i)
{
	return OSAtomicAdd32(-i, p);
}

static OF_INLINE int32_t
OFAtomicInt32Subtract(volatile int32_t *_Nonnull p, int32_t i)
{
	return OSAtomicAdd32(-i, p);
}

static OF_INLINE void *_Nullable
OFAtomicPointerSubtract(void *volatile _Nullable *_Nonnull p, intptr_t i)
{
#ifdef __LP64__
	return (void *)OSAtomicAdd64(-i, (int64_t *)p);
#else
	return (void *)OSAtomicAdd32(-i, (int32_t *)p);
#endif
}

static OF_INLINE int
OFAtomicIntIncrease(volatile int *_Nonnull p)
{
	return OSAtomicIncrement32(p);
}

static OF_INLINE int32_t
OFAtomicInt32Increase(volatile int32_t *_Nonnull p)
{
	return OSAtomicIncrement32(p);
}

static OF_INLINE int
OFAtomicIntDecrease(volatile int *_Nonnull p)
{
	return OSAtomicDecrement32(p);
}

static OF_INLINE int32_t
OFAtomicInt32Decrease(volatile int32_t *_Nonnull p)
{
	return OSAtomicDecrement32(p);
}

static OF_INLINE unsigned int
OFAtomicIntOr(volatile unsigned int *_Nonnull p, unsigned int i)
{
	return OSAtomicOr32(i, p);
}

static OF_INLINE uint32_t
OFAtomicInt32Or(volatile uint32_t *_Nonnull p, uint32_t i)
{
	return OSAtomicOr32(i, p);
}

static OF_INLINE unsigned int
OFAtomicIntAnd(volatile unsigned int *_Nonnull p, unsigned int i)
{
	return OSAtomicAnd32(i, p);
}

static OF_INLINE uint32_t
OFAtomicInt32And(volatile uint32_t *_Nonnull p, uint32_t i)
{
	return OSAtomicAnd32(i, p);
}

static OF_INLINE unsigned int
OFAtomicIntXor(volatile unsigned int *_Nonnull p, unsigned int i)
{
	return OSAtomicXor32(i, p);
}

static OF_INLINE uint32_t
OFAtomicInt32Xor(volatile uint32_t *_Nonnull p, uint32_t i)
{
	return OSAtomicXor32(i, p);
}

static OF_INLINE bool
OFAtomicIntCompareAndSwap(volatile int *_Nonnull p, int o, int n)
{
	return OSAtomicCompareAndSwapInt(o, n, p);
}

static OF_INLINE bool
OFAtomicInt32CompareAndSwap(volatile int32_t *_Nonnull p, int32_t o, int32_t n)
{
	return OSAtomicCompareAndSwap32(o, n, p);
}

static OF_INLINE bool
OFAtomicPointerCompareAndSwap(void *volatile _Nullable *_Nonnull p,
    void *_Nullable o, void *_Nullable n)
{
	return OSAtomicCompareAndSwapPtr(o, n, p);
}

static OF_INLINE void
OFMemoryBarrier(void)
{
	OSMemoryBarrier();
}

static OF_INLINE void
OFAcquireMemoryBarrier(void)
{
	OSMemoryBarrier();
}

static OF_INLINE void
OFReleaseMemoryBarrier(void)
{
	OSMemoryBarrier();
}
