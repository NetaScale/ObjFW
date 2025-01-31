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

#import "OFApplication.h"
#import "OFFile.h"
#import "OFFileManager.h"
#import "OFURL.h"
#import "OFXMLElement.h"

#import "FuncArrayGenerator.h"
#import "GlueGenerator.h"
#import "LinkLibGenerator.h"

@interface LibraryGenerator: OFObject <OFApplicationDelegate>
@end

OF_APPLICATION_DELEGATE(LibraryGenerator)

@implementation LibraryGenerator
- (void)applicationDidFinishLaunching
{
	OFURL *sourcesURL = [[OFFileManager defaultManager].currentDirectoryURL
	    URLByAppendingPathComponent: @"../../src"];
	OFURL *runtimeLibraryURL = [sourcesURL
	    URLByAppendingPathComponent: @"runtime/amiga-library.xml"];
	OFURL *runtimeLinkLibURL = [sourcesURL
	    URLByAppendingPathComponent: @"runtime/linklib/linklib.m"];
	OFURL *runtimeGlueHeaderURL = [sourcesURL
	    URLByAppendingPathComponent: @"runtime/amiga-glue.h"];
	OFURL *runtimeGlueURL = [sourcesURL
	    URLByAppendingPathComponent: @"runtime/amiga-glue.m"];
	OFURL *runtimeFuncArrayURL = [sourcesURL
	    URLByAppendingPathComponent: @"runtime/amiga-funcarray.inc"];
	OFXMLElement *runtimeLibrary = [OFXMLElement elementWithStream:
	    [OFFile fileWithPath: runtimeLibraryURL.fileSystemRepresentation
			    mode: @"r"]];
	OFFile *runtimeLinkLib =
	    [OFFile fileWithPath: runtimeLinkLibURL.fileSystemRepresentation
			    mode: @"w"];
	OFFile *runtimeGlueHeader =
	    [OFFile fileWithPath: runtimeGlueHeaderURL.fileSystemRepresentation
			    mode: @"w"];
	OFFile *runtimeGlue =
	    [OFFile fileWithPath: runtimeGlueURL.fileSystemRepresentation
			    mode: @"w"];
	OFFile *runtimeFuncArray =
	    [OFFile fileWithPath: runtimeFuncArrayURL.fileSystemRepresentation
			    mode: @"w"];
	LinkLibGenerator *runtimeLinkLibGenerator = [[[LinkLibGenerator alloc]
	    initWithLibrary: runtimeLibrary
	     implementation: runtimeLinkLib] autorelease];
	GlueGenerator *runtimeGlueGenerator = [[[GlueGenerator alloc]
	    initWithLibrary: runtimeLibrary
		     header: runtimeGlueHeader
	     implementation: runtimeGlue] autorelease];
	FuncArrayGenerator *runtimeFuncArrayGenerator;
	runtimeFuncArrayGenerator = [[[FuncArrayGenerator alloc]
	    initWithLibrary: runtimeLibrary
		    include: runtimeFuncArray] autorelease];

	[runtimeLinkLibGenerator generate];
	[runtimeGlueGenerator generate];
	[runtimeFuncArrayGenerator generate];

	[OFApplication terminate];
}
@end
