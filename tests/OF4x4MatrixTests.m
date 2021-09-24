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

#import "TestsAppDelegate.h"

static OFString *const module = @"OF4x4MatrixTests";

@implementation TestsAppDelegate (OF4x4MatrixTests)
- (void)_4x4MatrixTests
{
	void *pool = objc_autoreleasePoolPush();
	OF4x4Matrix *matrix;

	TEST(@"+[identity]",
	    memcmp([[OF4x4Matrix identity] values], (float [16]){
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	    }, 16 * sizeof(float)) == 0)

	TEST(@"+[matrixWithValues:]",
	    (matrix = [OF4x4Matrix matrixWithValues: (float [16]){
		 1,  2,  3,  4,
		 5,  6,  7,  8,
		 9, 10, 11, 12,
		13, 14, 15, 16
	    }]))

	TEST(@"-[description]",
	    [matrix.description isEqual: @"<OF4x4Matrix: {\n"
					 @"\t1 5 9 13\n"
					 @"\t2 6 10 14\n"
					 @"\t3 7 11 15\n"
					 @"\t4 8 12 16\n"
					 @"}>"])

	TEST(@"-[transpose]",
	    R([matrix transpose]) && memcmp(matrix.values, (float [16]){
		1, 5,  9, 13,
		2, 6, 10, 14,
		3, 7, 11, 15,
		4, 8, 12, 16
	    }, 16 * sizeof(float)) == 0)

	TEST(@"-[isEqual:]", [[OF4x4Matrix identity] isEqual:
	    [OF4x4Matrix matrixWithValues: (float [16]){
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	    }]])

	objc_autoreleasePoolPop(pool);
}
@end
