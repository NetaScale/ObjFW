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

#import "OFCreateSymbolicLinkFailedException.h"
#import "OFString.h"
#import "OFURL.h"

@implementation OFCreateSymbolicLinkFailedException
@synthesize URL = _URL, target = _target, errNo = _errNo;

+ (instancetype)exception
{
	OF_UNRECOGNIZED_SELECTOR
}

+ (instancetype)exceptionWithURL: (OFURL *)URL
			  target: (OFString *)target
			   errNo: (int)errNo
{
	return [[[self alloc] initWithURL: URL
				   target: target
				    errNo: errNo] autorelease];
}

- (instancetype)init
{
	OF_INVALID_INIT_METHOD
}

- (instancetype)initWithURL: (OFURL *)URL
		     target: (OFString *)target
		      errNo: (int)errNo
{
	self = [super init];

	@try {
		_URL = [URL copy];
		_target = [target copy];
		_errNo = errNo;
	} @catch (id e) {
		[self release];
		@throw e;
	}

	return self;
}

- (void)dealloc
{
	[_URL release];
	[_target release];

	[super dealloc];
}

- (OFString *)description
{
	return [OFString stringWithFormat:
	    @"Failed to create symbolic link %@ with target %@: %@",
	    _URL, _target, OFStrError(_errNo)];
}
@end
