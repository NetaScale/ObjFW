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

#import "OFRangeValue.h"
#import "OFMethodSignature.h"
#import "OFString.h"

#import "OFOutOfRangeException.h"

@implementation OFRangeValue
@synthesize rangeValue = _range;

- (instancetype)initWithRange: (OFRange)range
{
	self = [super init];

	_range = range;

	return self;
}

- (const char *)objCType
{
	return @encode(OFRange);
}

- (void)getValue: (void *)value size: (size_t)size
{
	if (size != sizeof(_range))
		@throw [OFOutOfRangeException exception];

	memcpy(value, &_range, sizeof(_range));
}

- (OFString *)description
{
	return [OFString stringWithFormat:
	    @"<OFValue: OFRange { %zu, %zu }>",
	    _range.location, _range.length];
}
@end
