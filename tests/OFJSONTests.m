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

#import "TestsAppDelegate.h"

static OFString *const module = @"OFJSON";

@implementation TestsAppDelegate (JSONTests)
- (void)JSONTests
{
	void *pool = objc_autoreleasePoolPush();
	OFString *string = @"{\"foo\"\t:'b\\na\\r', \"x\":/*foo*/ [.5\r,0xF,"
	    @"null//bar\n,\"foo\",false]}";
	OFDictionary *dict = [OFDictionary dictionaryWithKeysAndObjects:
	    @"foo", @"b\na\r",
	    @"x", [OFArray arrayWithObjects:
		[OFNumber numberWithFloat: .5f],
		[OFNumber numberWithInt: 0xF],
		[OFNull null],
		@"foo",
		[OFNumber numberWithBool: false],
		nil],
	    nil];

	TEST(@"-[objectByParsingJSON] #1",
	    [string.objectByParsingJSON isEqual: dict])

	TEST(@"-[JSONRepresentation]",
	    [[dict JSONRepresentation] isEqual:
	    @"{\"x\":[0.5,15,null,\"foo\",false],\"foo\":\"b\\na\\r\"}"])

	TEST(@"OFJSONRepresentationOptionPretty",
	    [[dict JSONRepresentationWithOptions:
	    OFJSONRepresentationOptionPretty] isEqual:
	    @"{\n\t\"x\": [\n\t\t0.5,\n\t\t15,\n\t\tnull,\n\t\t"
	    @"\"foo\",\n\t\tfalse\n\t],\n\t\"foo\": \"b\\na\\r\"\n}"])

	TEST(@"OFJSONRepresentationOptionJSON5",
	    [[dict JSONRepresentationWithOptions:
	    OFJSONRepresentationOptionJSON5] isEqual:
	    @"{x:[0.5,15,null,\"foo\",false],foo:\"b\\\na\\r\"}"])

	EXPECT_EXCEPTION(@"-[objectByParsingJSON] #2", OFInvalidJSONException,
	    [@"{" objectByParsingJSON])
	EXPECT_EXCEPTION(@"-[objectByParsingJSON] #3", OFInvalidJSONException,
	    [@"]" objectByParsingJSON])
	EXPECT_EXCEPTION(@"-[objectByParsingJSON] #4", OFInvalidJSONException,
	    [@"bar" objectByParsingJSON])
	EXPECT_EXCEPTION(@"-[objectByParsingJSON] #5", OFInvalidJSONException,
	    [@"[\"a\" \"b\"]" objectByParsingJSON])

	TEST(@"-[objectByParsingJSON] #6",
	    [@"[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[{}]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]"
	    .objectByParsingJSON isEqual: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject: [OFArray arrayWithObject:
	    [OFArray arrayWithObject:
	    [OFDictionary dictionary]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]])

	EXPECT_EXCEPTION(@"-[objectByParsingJSON] #7", OFInvalidJSONException,
	    [@"[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[{}]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]"
	    objectByParsingJSON])

	objc_autoreleasePoolPop(pool);
}
@end
