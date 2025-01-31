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

#import "OFRectValue.h"
#import "OFMethodSignature.h"
#import "OFString.h"

#import "OFOutOfRangeException.h"

@implementation OFRectValue
@synthesize rectValue = _rect;

- (instancetype)initWithRect: (OFRect)rect
{
	self = [super init];

	_rect = rect;

	return self;
}

- (const char *)objCType
{
	return @encode(OFRect);
}

- (void)getValue: (void *)value size: (size_t)size
{
	if (size != sizeof(_rect))
		@throw [OFOutOfRangeException exception];

	memcpy(value, &_rect, sizeof(_rect));
}

- (OFString *)description
{
	return [OFString stringWithFormat:
	    @"<OFValue: OFRect { %f, %f, %f, %f }>",
	    _rect.origin.x, _rect.origin.y,
	    _rect.size.width, _rect.size.height];
}
@end
