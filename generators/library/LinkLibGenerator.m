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

#import "OFArray.h"
#import "OFXMLAttribute.h"

#import "LinkLibGenerator.h"

#import "OFInvalidFormatException.h"
#import "OFUnsupportedVersionException.h"

#import "copyright.h"

@implementation LinkLibGenerator
- (instancetype)initWithLibrary: (OFXMLElement *)library
		 implementation: (OFStream *)impl
{
	self = [super init];

	@try {
		OFXMLAttribute *version;

		if (![library.name isEqual: @"amiga-library"] ||
		    library.namespace != nil)
			@throw [OFInvalidFormatException exception];

		if ((version = [library attributeForName: @"version"]) == nil)
			@throw [OFInvalidFormatException exception];

		if (![version.stringValue isEqual: @"1.0"])
			@throw [OFUnsupportedVersionException
			    exceptionWithVersion: version.stringValue];

		_library = [library retain];
		_impl = [impl retain];
	} @catch (id e) {
		[self release];
		@throw e;
	}

	return self;
}

- (void)dealloc
{
	[_library release];
	[_impl release];

	[super dealloc];
}

- (void)generate
{
	OFString *libBase = [_library attributeForName: @"base"].stringValue;
	OFArray OF_GENERIC(OFXMLElement *) *functions;
	size_t funcIndex = 0;

	[_impl writeString: COPYRIGHT];
	[_impl writeString:
	    @"/* This file is automatically generated from amiga-library.xml */"
	    @"\n\n"
	    @"#include \"config.h\"\n"
	    @"\n"];

	for (OFXMLElement *include in [_library elementsForName: @"include"])
		[_impl writeFormat: @"#import \"%@\"\n",
				    include.stringValue];

	[_impl writeFormat: @"\n"
			    @"extern struct Library *%@;\n"
			    @"\n",
			    libBase];

	functions = [_library elementsForName: @"function"];
	for (OFXMLElement *function in functions) {
		OFString *name =
		    [function attributeForName: @"name"].stringValue;
		OFString *returnType =
		    [function attributeForName: @"return-type"].stringValue;
		OFArray OF_GENERIC(OFXMLElement *) *arguments =
		    [function elementsForName: @"argument"];
		size_t argumentIndex;

		if (returnType == nil)
			returnType = @"void";

		[_impl writeFormat: @"%@\n%@(", returnType, name];

		argumentIndex = 0;
		for (OFXMLElement *argument in
		    [function elementsForName: @"argument"]) {
			OFString *argName =
			    [argument attributeForName: @"name"].stringValue;
			OFString *argType =
			    [argument attributeForName: @"type"].stringValue;

			if (argumentIndex++ > 0)
				[_impl writeString: @", "];

			[_impl writeString: argType];
			if (![argType hasSuffix: @"*"])
				[_impl writeString: @" "];
			[_impl writeString: argName];
		}

		[_impl writeFormat:
		    @")\n"
		    @"{\n"
		    @"#if defined(OF_AMIGAOS_M68K)\n"
		    @"\tregister struct Library *a6 __asm__(\"a6\") = %@;\n"
		    @"\t(void)a6;\n"
		    @"\t", libBase];

		if (![returnType isEqual: @"void"])
			[_impl writeString: @"return "];

		[_impl writeString: @"(("];
		[_impl writeString: returnType];
		if (![returnType hasSuffix: @"*"])
			[_impl writeString: @" "];
		[_impl writeString: @"(*)("];

		argumentIndex = 0;
		for (OFXMLElement *argument in arguments) {
			OFString *argType =
			    [argument attributeForName: @"type"].stringValue;
			OFString *m68kReg = [argument
			    attributeForName: @"m68k-reg"].stringValue;

			if (argumentIndex++ > 0)
				[_impl writeString: @", "];

			[_impl writeString: argType];
			if (![argType hasSuffix: @"*"])
				[_impl writeString: @" "];
			[_impl writeFormat: @"__asm__(\"%@\")", m68kReg];
		}

		[_impl writeFormat: @"))(((uintptr_t)%@) - %zu))(",
				    libBase, 30 + funcIndex * 6];

		argumentIndex = 0;
		for (OFXMLElement *argument in
		    [function elementsForName: @"argument"]) {
			OFString *argName =
			    [argument attributeForName: @"name"].stringValue;

			if (argumentIndex++ > 0)
				[_impl writeString: @", "];

			[_impl writeString: argName];
		}

		[_impl writeFormat: @");\n"
				    @"#elif defined(OF_MORPHOS)\n"
				    @"\t__asm__ __volatile__ (\n"
				    @"\t    \"mr\t\t%%%%r12, %%0\"\n"
				    @"\t    :: \"r\"(%@) : \"r12\"\n"
				    @"\t);\n"
				    @"\n"
				    @"\t",
				    libBase, libBase];

		if (![returnType isEqual: @"void"])
			[_impl writeString: @"return "];

		[_impl writeString: @"__extension__ (("];
		[_impl writeString: returnType];
		if (![returnType hasSuffix: @"*"])
			[_impl writeString: @" "];
		[_impl writeString: @"(*)("];

		argumentIndex = 0;
		for (OFXMLElement *argument in arguments) {
			OFString *argType =
			    [argument attributeForName: @"type"].stringValue;

			if (argumentIndex++ > 0)
				[_impl writeString: @", "];

			[_impl writeString: argType];
		}

		[_impl writeFormat: @"))*(void **)(((uintptr_t)%@) - %zu))(",
				    libBase, 28 + funcIndex * 6];

		argumentIndex = 0;
		for (OFXMLElement *argument in
		    [function elementsForName: @"argument"]) {
			OFString *argName =
			    [argument attributeForName: @"name"].stringValue;

			if (argumentIndex++ > 0)
				[_impl writeString: @", "];

			[_impl writeString: argName];
		}

		[_impl writeString: @");\n"
				    @"#endif\n"];

		if ([function attributeForName: @"noreturn"] != nil)
			[_impl writeString: @"\n\tOF_UNREACHABLE\n"];

		[_impl writeString: @"}\n"];

		if (++funcIndex < functions.count)
			[_impl writeString: @"\n"];
	}
}
@end
