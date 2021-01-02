/*
 * Copyright (c) 2008-2021 Jonathan Schleifer <js@nil.im>
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

#import "tlskey.h"

int
of_tlskey_new(of_tlskey_t *key)
{
	*key = TLSAllocA(NULL);

	if (*key == TLS_INVALID_INDEX)
		return EAGAIN;

	return 0;
}

int
of_tlskey_free(of_tlskey_t key)
{
	return (TLSFree(key) ? 0 : EINVAL);
}
