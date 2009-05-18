/*
 * Copyright (c) 2008 - 2009
 *   Jonathan Schleifer <js@webkeks.org>
 *
 * All rights reserved.
 *
 * This file is part of libobjfw. It may be distributed under the terms of the
 * Q Public License 1.0, which can be found in the file LICENSE included in
 * the packaging of this file.
 */

#import "config.h"

#include <string.h>

#import "OFDictionary.h"
#import "OFIterator.h"
#import "OFExceptions.h"
#import "OFMacros.h"

/* Reference for static linking */
void _reference_to_OFIterator_in_OFDictionary()
{
	_OFIterator_reference = 1;
}

@implementation OFDictionary
+ dictionary;
{
	return [[[OFDictionary alloc] init] autorelease];
}

+ dictionaryWithHashSize: (int)hashsize
{
	return [[[OFDictionary alloc] initWithHashSize: hashsize] autorelease];
}

- init
{
	self = [super init];

	size = 4096;

	@try {
		data = [self allocNItems: size
				withSize: sizeof(OFList*)];
	} @catch (OFException *e) {
		/*
		 * We can't use [super dealloc] on OS X here. Compiler bug?
		 * Anyway, set size to 0 so that [self dealloc] works.
		 */
		size = 0;
		[self dealloc];
		@throw e;
	}
	memset(data, 0, size * sizeof(OFList*));

	return self;
}

- initWithHashSize: (int)hashsize
{
	self = [super init];

	if (hashsize < 8 || hashsize >= 28) {
		Class c = isa;
		[super dealloc];
		@throw [OFInvalidArgumentException newWithClass: c
						    andSelector: _cmd];
	}

	size = (size_t)1 << hashsize;

	@try {
		data = [self allocNItems: size
				withSize: sizeof(OFList*)];
	} @catch (OFException *e) {
		/*
		 * We can't use [super dealloc] on OS X here. Compiler bug?
		 * Anyway, set size to 0 so that [self dealloc] works.
		 */
		size = 0;
		[self dealloc];
		@throw e;
	}
	memset(data, 0, size * sizeof(OFList*));

	return self;
}

- (void)dealloc
{
	size_t i;

	for (i = 0; i < size; i++)
		if (data[i] != nil)
			[data[i] release];

	[super dealloc];
}

- set: (OFObject <OFCopying>*)key
   to: (OFObject*)obj
{
	uint32_t hash;
	of_list_object_t *iter, *key_obj;

	if (key == nil || obj == nil)
		@throw [OFInvalidArgumentException newWithClass: isa
						    andSelector: _cmd];

	hash = [key hash] & (size - 1);

	if (data[hash] == nil)
		data[hash] = [[OFList alloc] init];

	for (iter = [data[hash] first]; iter != NULL; iter = iter->next->next) {
		if ([iter->object isEqual: key]) {
			[iter->next->object release];
			[obj retain];
			iter->next->object = obj;

			return self;
		}
	}

	key = [key copy];
	@try {
		key_obj = [data[hash] append: key];
	} @finally {
		[key release];
	}

	@try {
		[data[hash] append: obj];
	} @catch (OFException *e) {
		[data[hash] remove: key_obj];
		@throw e;
	}

	return self;
}

- (id)get: (OFObject*)key
{
	uint32_t hash;
	of_list_object_t *iter;

	if (key == nil)
		@throw [OFInvalidArgumentException newWithClass: isa
						    andSelector: _cmd];

	hash = [key hash] & (size - 1);

	if (data[hash] == nil)
		return nil;

	for (iter = [data[hash] first]; iter != NULL; iter = iter->next->next)
		if ([iter->object isEqual: key])
			return iter->next->object;

	return nil;
}

- remove: (OFObject*)key
{
	uint32_t hash;
	of_list_object_t *iter;

	if (key == nil)
		@throw [OFInvalidArgumentException newWithClass: isa
						    andSelector: _cmd];

	hash = [key hash] & (size - 1);

	if (data[hash] == nil)
		return self;

	for (iter = [data[hash] first]; iter != NULL; iter = iter->next->next) {
		if ([iter->object isEqual: key]) {
			[data[hash] remove: iter->next];
			[data[hash] remove: iter];

			if ([data[hash] first] == NULL) {
				[data[hash] release];
				data[hash] = nil;
			}

			return self;
		}
	}

	return self;
}

- (float)averageItemsPerBucket
{
	size_t items, buckets, i;

	items = 0;
	buckets = 0;

	for (i = 0; i < size; i++) {
		if (data[i] != nil) {
			items += [data[i] items] / 2;
			buckets++;
		}
	}

	return (float)items / buckets;
}

- changeHashSize: (int)hashsize
{
	OFList **newdata;
	size_t newsize, i;
	of_list_object_t *iter;

	if (hashsize < 8 || hashsize >= 28)
		@throw [OFInvalidArgumentException newWithClass: isa
						    andSelector: _cmd];

	newsize = (size_t)1 << hashsize;
	newdata = [self allocNItems: newsize
			   withSize: sizeof(OFList*)];
	memset(data, 0, newsize * sizeof(OFList*));

	for (i = 0; i < size; i++) {
		if (OF_LIKELY(data[i] == nil))
			continue;

		for (iter = [data[i] first]; iter != NULL;
		    iter = iter->next->next) {
			uint32_t hash = [iter->object hash] & (newsize - 1);

			if (newdata[hash] == nil)
				newdata[hash] = [[OFList alloc] init];

			[newdata[hash] append: iter->object];
			[newdata[hash] append: iter->next->object];
		}

		[data[i] release];
	}

	[self freeMem: data];
	data = newdata;
	size = newsize;

	return self;
}

/* FIXME: Implement this! */
/*
- (BOOL)isEqual
{
}
*/
@end
