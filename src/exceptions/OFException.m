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

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_DLFCN_H
# include <dlfcn.h>
#endif

#import "OFException.h"
#import "OFArray.h"
#import "OFLocale.h"
#ifdef OF_HAVE_THREADS
# import "OFPlainMutex.h"
#endif
#import "OFString.h"
#import "OFSystemInfo.h"

#import "OFInitializationFailedException.h"
#import "OFLockFailedException.h"
#import "OFUnlockFailedException.h"

#if defined(OF_WINDOWS) && defined(OF_HAVE_SOCKETS)
# include <winerror.h>
#endif

#if defined(OF_ARM) && !defined(__ARM_DWARF_EH__)
# define HAVE_ARM_EHABI_EXCEPTIONS
#endif

struct _Unwind_Context;
typedef enum {
	_URC_OK		  = 0,
	_URC_END_OF_STACK = 5
} _Unwind_Reason_Code;

struct BacktraceCtx {
	void **backtrace;
	uint8_t i;
};

#ifdef HAVE__UNWIND_BACKTRACE
extern _Unwind_Reason_Code _Unwind_Backtrace(
    _Unwind_Reason_Code (*)(struct _Unwind_Context *, void *), void *);
#endif
#ifndef HAVE_ARM_EHABI_EXCEPTIONS
extern uintptr_t _Unwind_GetIP(struct _Unwind_Context *);
#else
extern int _Unwind_VRS_Get(struct _Unwind_Context *, int, uint32_t, int,
    void *);
#endif

#if !defined(HAVE_STRERROR_R) && defined(OF_HAVE_THREADS)
static OFPlainMutex mutex;

OF_CONSTRUCTOR()
{
	OFEnsure(OFPlainMutexNew(&mutex) == 0);
}

OF_DESTRUCTOR()
{
	OFPlainMutexFree(&mutex);
}
#endif

OFString *
OFStrError(int errNo)
{
	OFString *ret;
#ifdef HAVE_STRERROR_R
	char buffer[256];
#endif

	if (errNo == 0)
		return @"Unknown error";

#if defined(OF_WINDOWS) && defined(OF_HAVE_SOCKETS)
	/*
	 * These were translated from WSAE* errors to errno and thus Win32's
	 * strerror_r() does not know about them.
	 *
	 * FIXME: These could have better descriptions!
	 */
	switch (errNo) {
	case EADDRINUSE:
		return @"EADDRINUSE";
	case EADDRNOTAVAIL:
		return @"EADDRNOTAVAIL";
	case EAFNOSUPPORT:
		return @"EAFNOSUPPORT";
	case EALREADY:
		return @"EALREADY";
	case ECONNABORTED:
		return @"ECONNABORTED";
	case ECONNREFUSED:
		return @"ECONNREFUSED";
	case ECONNRESET:
		return @"ECONNRESET";
	case EDESTADDRREQ:
		return @"EDESTADDRREQ";
	case EDQUOT:
		return @"EDQUOT";
	case EHOSTDOWN:
		return @"EHOSTDOWN";
	case EHOSTUNREACH:
		return @"EHOSTUNREACH";
	case EINPROGRESS:
		return @"EINPROGRESS";
	case EISCONN:
		return @"EISCONN";
	case ELOOP:
		return @"ELOOP";
	case EMSGSIZE:
		return @"EMSGSIZE";
	case ENETDOWN:
		return @"ENETDOWN";
	case ENETRESET:
		return @"ENETRESET";
	case ENETUNREACH:
		return @"ENETUNREACH";
	case ENOBUFS:
		return @"ENOBUFS";
	case ENOPROTOOPT:
		return @"ENOPROTOOPT";
	case ENOTCONN:
		return @"ENOTCONN";
	case ENOTSOCK:
		return @"ENOTSOCK";
	case EOPNOTSUPP:
		return @"EOPNOTSUPP";
	case EPFNOSUPPORT:
		return @"EPFNOSUPPORT";
	case EPROCLIM:
		return @"EPROCLIM";
	case EPROTONOSUPPORT:
		return @"EPROTONOSUPPORT";
	case EPROTOTYPE:
		return @"EPROTOTYPE";
	case EREMOTE:
		return @"EREMOTE";
	case ESHUTDOWN:
		return @"ESHUTDOWN";
	case ESOCKTNOSUPPORT:
		return @"ESOCKTNOSUPPORT";
	case ESTALE:
		return @"ESTALE";
	case ETIMEDOUT:
		return @"ETIMEDOUT";
	case ETOOMANYREFS:
		return @"ETOOMANYREFS";
	case EUSERS:
		return @"EUSERS";
	case EWOULDBLOCK:
		return @"EWOULDBLOCK";
	}
#endif

#if defined(STRERROR_R_RETURNS_CHARP)
	/* glibc uses a different strerror_r when _GNU_SOURCE is defined */
	char *string;

	if ((string = strerror_r(errNo, buffer, 256)) == NULL)
		return @"Unknown error (strerror_r failed)";

	ret = [OFString stringWithCString: string
				 encoding: [OFLocale encoding]];
#elif defined(HAVE_STRERROR_R)
	if (strerror_r(errNo, buffer, 256) != 0)
		return @"Unknown error (strerror_r failed)";

	ret = [OFString stringWithCString: buffer
				 encoding: [OFLocale encoding]];
#else
# ifdef OF_HAVE_THREADS
	if (OFPlainMutexLock(&mutex) != 0)
		@throw [OFLockFailedException exception];

	@try {
# endif
		ret = [OFString
		    stringWithCString: strerror(errNo)
			     encoding: [OFLocale encoding]];
# ifdef OF_HAVE_THREADS
	} @finally {
		if (OFPlainMutexUnlock(&mutex) != 0)
			@throw [OFUnlockFailedException exception];
	}
# endif
#endif

	return ret;
}

#ifdef OF_WINDOWS
OFString *
OFWindowsStatusToString(LSTATUS status)
{
	OFString *string = nil;
	void *buffer;

	if ([OFSystemInfo isWindowsNT]) {
		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_IGNORE_INSERTS |
		    FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, status, 0,
		    (LPWSTR)&buffer, 0, NULL) != 0) {
			@try {
				string = [OFString
				    stringWithUTF16String: buffer];
			} @finally {
				LocalFree(buffer);
			}
		}
	} else {
		if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_IGNORE_INSERTS |
		    FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, status, 0,
		    (LPSTR)&buffer, 0, NULL) != 0) {
			@try {
				string = [OFString
				    stringWithCString: buffer
					     encoding: [OFLocale encoding]];
			} @finally {
				LocalFree(buffer);
			}
		}
	}

	if (string == nil)
		string = [OFString stringWithFormat: @"Status code %u", status];

	return string;
}
#endif

#ifdef HAVE__UNWIND_BACKTRACE
static _Unwind_Reason_Code
backtraceCallback(struct _Unwind_Context *ctx, void *data)
{
	struct BacktraceCtx *bt = data;

	if (bt->i < OFBacktraceSize) {
# ifndef HAVE_ARM_EHABI_EXCEPTIONS
		bt->backtrace[bt->i++] = (void *)_Unwind_GetIP(ctx);
# else
		uintptr_t ip;

		_Unwind_VRS_Get(ctx, 0, 15, 0, &ip);
		bt->backtrace[bt->i++] = (void *)(ip & ~1);
# endif
		return _URC_OK;
	}

	return _URC_END_OF_STACK;
}
#endif

@implementation OFException
+ (instancetype)exception
{
	return [[[self alloc] init] autorelease];
}

#ifdef HAVE__UNWIND_BACKTRACE
- (instancetype)init
{
	struct BacktraceCtx ctx;

	self = [super init];

	ctx.backtrace = _backtrace;
	ctx.i = 0;
	_Unwind_Backtrace(backtraceCallback, &ctx);

	return self;
}
#endif

- (OFString *)description
{
	return [OFString stringWithFormat:
	    @"An exception of type %@ occurred!", self.class];
}

- (OFArray OF_GENERIC(OFString *) *)backtrace
{
#ifdef HAVE__UNWIND_BACKTRACE
	OFMutableArray OF_GENERIC(OFString *) *backtrace =
	    [OFMutableArray array];
	void *pool = objc_autoreleasePoolPush();

	for (uint8_t i = 0; i < OFBacktraceSize && _backtrace[i] != NULL; i++) {
# ifdef HAVE_DLADDR
		Dl_info info;

		if (dladdr(_backtrace[i], &info)) {
			OFString *frame;

			if (info.dli_sname != NULL) {
				ptrdiff_t offset = (char *)_backtrace[i] -
				    (char *)info.dli_saddr;

				frame = [OFString stringWithFormat:
				    @"%p <%s+%td> at %s",
				    _backtrace[i], info.dli_sname, offset,
				    info.dli_fname];
			} else
				frame = [OFString stringWithFormat:
				    @"%p <?" @"?> at %s",
				    _backtrace[i], info.dli_fname];

			[backtrace addObject: frame];
		} else
# endif
			[backtrace addObject:
			    [OFString stringWithFormat: @"%p", _backtrace[i]]];
	}

	objc_autoreleasePoolPop(pool);

	[backtrace makeImmutable];

	return backtrace;
#else
	return nil;
#endif
}
@end
