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

#include <string.h>

#import "TestsAppDelegate.h"

#define FORMAT @"%@ %@ %@ %@ %@ %@ %@ %@ %@ %g %g %g %g %g %g %g %g %g"
#define ARGS @"a", @"b", @"c", @"d", @"e", @"f", @"g", @"h", @"i", \
	    1.5, 2.25, 3.125, 4.0625, 5.03125, 6.5, 7.25, 8.0, 9.0
#define RESULT @"a b c d e f g h i 1.5 2.25 3.125 4.0625 5.03125 6.5 7.25 8 9"

static OFString *const module = @"Forwarding";
static size_t forwardingsCount = 0;
static bool success = false;
static id target = nil;

struct StretTest {
	char buffer[1024];
};

@interface ForwardingTest: OFObject
@end

@interface ForwardingTest (Test)
+ (void)test;
- (void)test;
- (uint32_t)forwardingTargetTest: (intptr_t)a0
				: (intptr_t)a1
				: (double)a2
				: (double)a3;
- (OFString *)forwardingTargetVarArgTest: (OFConstantString *)format, ...;
- (long double)forwardingTargetFPRetTest;
- (struct StretTest)forwardingTargetStRetTest;
- (void)forwardingTargetNilTest;
- (void)forwardingTargetSelfTest;
- (struct StretTest)forwardingTargetNilStRetTest;
- (struct StretTest)forwardingTargetSelfStRetTest;
@end

@interface ForwardingTarget: OFObject
@end

static void
test(id self, SEL _cmd)
{
	success = true;
}

@implementation ForwardingTest
+ (bool)resolveClassMethod: (SEL)selector
{
	forwardingsCount++;

	if (sel_isEqual(selector, @selector(test))) {
		class_replaceMethod(object_getClass(self), @selector(test),
		    (IMP)test, "v#:");
		return YES;
	}

	return NO;
}

+ (bool)resolveInstanceMethod: (SEL)selector
{
	forwardingsCount++;

	if (sel_isEqual(selector, @selector(test))) {
		class_replaceMethod(self, @selector(test), (IMP)test, "v@:");
		return YES;
	}

	return NO;
}

- (id)forwardingTargetForSelector: (SEL)selector
{
	/*
	 * Do some useless calculations in as many registers as possible to
	 * check if the arguments are properly saved and restored.
	 */
	volatile register intptr_t r0 = 0, r1 = 1, r2 = 2, r3 = 3, r4 = 4,
	    r5 = 5, r6 = 6, r7 = 7, r8 = 8, r9 = 9, r10 = 10, r11 = 11;
	volatile register double f0 = 0.5, f1 = 1.5, f2 = 2.5, f3 = 3.5,
	    f4 = 4.5, f5 = 5.5, f6 = 6.5, f7 = 7.5, f8 = 8.5, f9 = 9.5,
	    f10 = 10.5, f11 = 11.5;
	double add = r0 * r1 * r2 * r3 * r4 * r5 * r6 * r7 * r8 * r9 * r10 *
	    r11 * f0 * f1 * f2 * f3 * f4 * f5 * f6 * f7 * f8 * f9 * f10 * f11;

	if (sel_isEqual(selector, @selector(forwardingTargetTest::::)) ||
	    sel_isEqual(selector, @selector(forwardingTargetVarArgTest:)) ||
	    sel_isEqual(selector, @selector(forwardingTargetFPRetTest)) ||
	    sel_isEqual(selector, @selector(forwardingTargetStRetTest)))
		return (id)((char *)target + (ptrdiff_t)add);

	if (sel_isEqual(selector, @selector(forwardingTargetNilTest)) ||
	    sel_isEqual(selector, @selector(forwardingTargetNilStRetTest)))
		return nil;

	if (sel_isEqual(selector, @selector(forwardingTargetSelfTest)) ||
	    sel_isEqual(selector, @selector(forwardingTargetSelfStRetTest)))
		return self;

	abort();

	OF_UNREACHABLE
}
@end

@implementation ForwardingTarget
- (uint32_t)forwardingTargetTest: (intptr_t)a0
				: (intptr_t)a1
				: (double)a2
				: (double)a3
{
	OFEnsure(self == target);

	if (a0 != (intptr_t)0xDEADBEEF)
		return 0;
	if (a1 != -1)
		return 0;
	if (a2 != 1.25)
		return 0;
	if (a3 != 2.75)
		return 0;

	return 0x12345678;
}

- (OFString *)forwardingTargetVarArgTest: (OFConstantString *)format, ...
{
	va_list args;
	OFString *ret;

	OFEnsure(self == target);

	va_start(args, format);
	ret = [[[OFString alloc] initWithFormat: format
				      arguments: args] autorelease];
	va_end(args);

	return ret;
}

- (long double)forwardingTargetFPRetTest
{
	OFEnsure(self == target);

	return 12345678.00006103515625;
}

- (struct StretTest)forwardingTargetStRetTest
{
	struct StretTest ret = { { 0 } };

	OFEnsure(self == target);

	memcpy(ret.buffer, "abcdefghijklmnopqrstuvwxyz", 27);

	return ret;
}
@end

@implementation TestsAppDelegate (ForwardingTests)
- (void)forwardingTests
{
	void *pool = objc_autoreleasePoolPush();

	TEST(@"Forwarding a message and adding a class method",
	    R([ForwardingTest test]) && success &&
	    R([ForwardingTest test]) && forwardingsCount == 1);

	ForwardingTest *testObject =
	    [[[ForwardingTest alloc] init] autorelease];

	success = false;
	forwardingsCount = 0;

	TEST(@"Forwarding a message and adding an instance method",
	    R([testObject test]) && success && R([testObject test]) &&
	    forwardingsCount == 1);

#ifdef OF_HAVE_FORWARDING_TARGET_FOR_SELECTOR
	target = [[[ForwardingTarget alloc] init] autorelease];
	TEST(@"-[forwardingTargetForSelector:]",
	    [testObject forwardingTargetTest: 0xDEADBEEF
					    : -1
					    : 1.25
					    : 2.75] == 0x12345678)
	TEST(@"-[forwardingTargetForSelector:] variable arguments",
	    [[testObject forwardingTargetVarArgTest: FORMAT, ARGS]
	    isEqual: RESULT])
	/*
	 * Don't try fpret on Win64 if we don't have stret forwarding, as
	 * long double is handled as a struct there.
	 */
# if !defined(OF_WINDOWS) || !defined(OF_X86_64) || \
	defined(OF_HAVE_FORWARDING_TARGET_FOR_SELECTOR_STRET)
	TEST(@"-[forwardingTargetForSelector:] fp return",
	    [testObject forwardingTargetFPRetTest] == 12345678.00006103515625)
# endif
# ifdef OF_HAVE_FORWARDING_TARGET_FOR_SELECTOR_STRET
	TEST(@"-[forwardingTargetForSelector:] struct return",
	    !memcmp([testObject forwardingTargetStRetTest].buffer,
	    "abcdefghijklmnopqrstuvwxyz", 27))
# endif
	EXPECT_EXCEPTION(@"-[forwardingTargetForSelector:] nil target",
	    OFNotImplementedException, [testObject forwardingTargetNilTest])
	EXPECT_EXCEPTION(@"-[forwardingTargetForSelector:] self target",
	    OFNotImplementedException, [testObject forwardingTargetSelfTest])
# ifdef OF_HAVE_FORWARDING_TARGET_FOR_SELECTOR_STRET
	EXPECT_EXCEPTION(@"-[forwardingTargetForSelector:] nil target + "
	    @"stret", OFNotImplementedException,
	    [testObject forwardingTargetNilStRetTest])
	EXPECT_EXCEPTION(@"-[forwardingTargetForSelector:] self target + "
	    @"stret", OFNotImplementedException,
	    [testObject forwardingTargetSelfStRetTest])
# endif
#endif

	objc_autoreleasePoolPop(pool);
}
@end
