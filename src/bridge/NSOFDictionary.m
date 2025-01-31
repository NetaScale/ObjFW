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

#import "NSOFDictionary.h"
#import "OFDictionary.h"
#import "OFEnumerator+NSObject.h"

#import "NSBridging.h"
#import "OFBridging.h"

#import "OFOutOfRangeException.h"

@implementation NSOFDictionary
- (instancetype)initWithOFDictionary: (OFDictionary *)dictionary
{
	if ((self = [super init]) != nil)
		_dictionary = [dictionary retain];

	return self;
}

- (void)dealloc
{
	[_dictionary release];

	[super dealloc];
}

- (id)objectForKey: (id)key
{
	id object;

	if ([(NSObject *)key conformsToProtocol: @protocol(NSBridging)])
		key = [key OFObject];

	object = [_dictionary objectForKey: key];

	if ([(OFObject *)object conformsToProtocol: @protocol(OFBridging)])
		return [object NSObject];

	return object;
}

- (NSUInteger)count
{
	size_t count = _dictionary.count;

	if (count > NSUIntegerMax)
		@throw [OFOutOfRangeException exception];

	return (NSUInteger)count;
}

- (NSEnumerator *)keyEnumerator
{
	return [_dictionary keyEnumerator].NSObject;
}
@end
