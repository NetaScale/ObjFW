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

#import "OFNull.h"
#import "OFString.h"
#import "OFXMLElement.h"
#import "OFData.h"

#import "OFInvalidArgumentException.h"

@interface OFNull ()
- (OFString *)
    of_JSONRepresentationWithOptions: (OFJSONRepresentationOptions)options
			       depth: (size_t)depth;
@end

static OFNull *null = nil;

@implementation OFNull
+ (void)initialize
{
	null = [[self alloc] init];
}

+ (OFNull *)null
{
	return null;
}

- (instancetype)initWithSerialization: (OFXMLElement *)element
{
	void *pool;

	[self release];

	pool = objc_autoreleasePoolPush();

	if (![element.name isEqual: self.className] ||
	    ![element.namespace isEqual: OFSerializationNS])
		@throw [OFInvalidArgumentException exception];

	objc_autoreleasePoolPop(pool);

	return [OFNull null];
}

- (OFString *)description
{
	return @"<null>";
}

- (id)copy
{
	return self;
}

- (OFXMLElement *)XMLElementBySerializing
{
	void *pool = objc_autoreleasePoolPush();
	OFXMLElement *element;

	element = [OFXMLElement elementWithName: self.className
				      namespace: OFSerializationNS];

	[element retain];

	objc_autoreleasePoolPop(pool);

	return [element autorelease];
}

- (OFString *)JSONRepresentation
{
	return [self of_JSONRepresentationWithOptions: 0 depth: 0];
}

- (OFString *)JSONRepresentationWithOptions:
    (OFJSONRepresentationOptions)options
{
	return [self of_JSONRepresentationWithOptions: options depth: 0];
}

- (OFString *)
    of_JSONRepresentationWithOptions: (OFJSONRepresentationOptions)options
					 depth: (size_t)depth
{
	return @"null";
}

- (OFData *)messagePackRepresentation
{
	uint8_t type = 0xC0;
	return [OFData dataWithItems: &type count: 1];
}

- (instancetype)autorelease
{
	return self;
}

- (instancetype)retain
{
	return self;
}

- (void)release
{
}

- (unsigned int)retainCount
{
	return OFMaxRetainCount;
}

- (void)dealloc
{
	OF_DEALLOC_UNSUPPORTED
}
@end
