/*
 * Copyright (c) 2008 - 2009
 *   Jonathan Schleifer <js@webkeks.org>
 *
 * All rights reserved.
 *
 * This file is part of ObjFW. It may be distributed under the terms of the
 * Q Public License 1.0, which can be found in the file LICENSE included in
 * the packaging of this file.
 */

#include "config.h"

#ifdef STDOUT
#include <stdio.h>
#endif
#include <stdlib.h>

#import "OFString.h"
#import "OFAutoreleasePool.h"

extern void array_tests();
extern void dataarray_tests();
extern void dictionary_tests();
extern void hashes_tests();
extern void list_tests();
extern void number_tests();
extern void object_tests();
#ifdef OF_PLUGINS
extern void plugin_tests();
#endif
#ifdef OF_HAVE_PROPERTIES
extern void properties_tests();
#endif
extern void string_tests();
extern void tcpsocket_tests();
#ifdef OF_THREADS
extern void thread_tests();
#endif
extern void xmlelement_tests();
extern void xmlparser_tests();

static int fails = 0;

static void
output(OFString *str, int color)
{
#ifdef STDOUT
	switch (color) {
	case 0:
		fputs("\r\033[K\033[1;33m", stdout);
		break;
	case 1:
		fputs("\r\033[K\033[1;32m", stdout);
		break;
	case 2:
		fputs("\r\033[K\033[1;31m", stdout);
		break;
	}

	fputs([str cString], stdout);
	fputs("\033[m", stdout);
	fflush(stdout);
#else
#error No output method!
#endif
}

void
testing(OFString *module, OFString *test)
{
	OFAutoreleasePool *pool = [[OFAutoreleasePool alloc] init];
	OFString *str = [OFString stringWithFormat: @"[%s] %s: testing...",
						    [module cString],
						    [test cString]];
	output(str, 0);
	[pool release];
}

void
success(OFString *module, OFString *test)
{
	OFAutoreleasePool *pool = [[OFAutoreleasePool alloc] init];
	OFString *str = [OFString stringWithFormat: @"[%s] %s: ok\n",
						    [module cString],
						    [test cString]];
	output(str, 1);
	[pool release];
}

void
failed(OFString *module, OFString *test)
{
	OFAutoreleasePool *pool = [[OFAutoreleasePool alloc] init];
	OFString *str = [OFString stringWithFormat: @"[%s] %s: failed\n",
						    [module cString],
						    [test cString]];
	output(str, 2);
	fails++;
	[pool release];
}

int
main()
{
	object_tests();
	hashes_tests();
	string_tests();
	dataarray_tests();
	array_tests();
	dictionary_tests();
	list_tests();
	number_tests();
	tcpsocket_tests();
#ifdef OF_THREADS
	thread_tests();
#endif
	xmlelement_tests();
	xmlparser_tests();
#ifdef OF_PLUGINS
	plugin_tests();
#endif
#ifdef OF_HAVE_PROPERTIES
	properties_tests();
#endif

	return fails;
}
