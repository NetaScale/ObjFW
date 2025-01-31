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

#define OF_CONSTANT_STRING_M

#include "config.h"

#include <stdlib.h>
#include <string.h>

#import "OFConstantString.h"
#import "OFUTF8String.h"

#import "OFInitializationFailedException.h"
#import "OFInvalidEncodingException.h"
#import "OFOutOfMemoryException.h"

#if defined(OF_APPLE_RUNTIME) && !defined(__OBJC2__)
# import <objc/runtime.h>

struct {
	struct class *isa, *superclass;
	const char *name;
	long version, info, instanceSize;
	struct ivar_list *iVars;
	struct method_list **methodList;
	struct cache *cache;
	struct protocol_list *protocols;
	const char *iVarLayout;
	struct class_ext *ext;
} _OFConstantStringClassReference;
#endif

@interface OFConstantUTF8String: OFUTF8String
@end

@implementation OFConstantUTF8String
+ (instancetype)alloc
{
	OF_UNRECOGNIZED_SELECTOR
}

- (instancetype)retain
{
	return self;
}

- (instancetype)autorelease
{
	return self;
}

- (unsigned int)retainCount
{
	return OFMaxRetainCount;
}

- (void)release
{
}

- (void)dealloc
{
	OF_DEALLOC_UNSUPPORTED
}
@end

@implementation OFConstantString
+ (void)load
{
#if defined(OF_APPLE_RUNTIME) && !defined(__OBJC2__)
	/*
	 * objc_setFutureClass suddenly stopped working as OFConstantString
	 * became more complex. So the only solution is to make
	 * _OFConstantStringClassReference the actual class, but there is no
	 * objc_initializeClassPair in 10.5. However, objc_allocateClassPair
	 * does not register the new class with the subclass in the ObjC1
	 * runtime like the ObjC2 runtime does, so this workaround should be
	 * fine.
	 */
	Class class;

	if ((class = objc_allocateClassPair(self, "OFConstantString_hack",
	    0)) == NULL)
		@throw [OFInitializationFailedException
		    exceptionWithClass: self];
	memcpy(&_OFConstantStringClassReference, class,
	    sizeof(_OFConstantStringClassReference));
	free(class);
	objc_registerClassPair((Class)&_OFConstantStringClassReference);
#endif
}

- (void)finishInitialization
{
	@synchronized (self) {
		struct OFUTF8StringIvars *ivars;

		if ([self isMemberOfClass: [OFConstantUTF8String class]])
			return;

		ivars = OFAllocZeroedMemory(1, sizeof(*ivars));
		ivars->cString = _cString;
		ivars->cStringLength = _cStringLength;

		switch (OFUTF8StringCheck(ivars->cString, ivars->cStringLength,
		    &ivars->length)) {
		case 1:
			ivars->isUTF8 = true;
			break;
		case -1:
			OFFreeMemory(ivars);
			@throw [OFInvalidEncodingException exception];
		}

		_cString = (char *)ivars;
		object_setClass(self, [OFConstantUTF8String class]);
	}
}

+ (instancetype)alloc
{
	OF_UNRECOGNIZED_SELECTOR
}

- (instancetype)retain
{
	return self;
}

- (instancetype)autorelease
{
	return self;
}

- (unsigned int)retainCount
{
	return OFMaxRetainCount;
}

- (void)release
{
}

- (void)dealloc
{
	OF_DEALLOC_UNSUPPORTED
}

/*
 * In all following methods, the constant string is converted to an
 * OFConstantUTF8String and the message sent again.
 */

/* From protocol OFCopying */
- (id)copy
{
	[self finishInitialization];
	return [self copy];
}

/* From protocol OFMutableCopying */
- (id)mutableCopy
{
	[self finishInitialization];
	return [self mutableCopy];
}

/* From protocol OFComparing,  but overridden in OFString */
- (OFComparisonResult)compare: (OFString *)string
{
	[self finishInitialization];
	return [self compare: string];
}

/* From OFObject, but reimplemented in OFString */
- (bool)isEqual: (id)object
{
	[self finishInitialization];
	return [self isEqual: object];
}

- (unsigned long)hash
{
	[self finishInitialization];
	return self.hash;
}

- (OFString *)description
{
	[self finishInitialization];
	return self.description;
}

/* From OFString */
- (const char *)UTF8String
{
	[self finishInitialization];
	return self.UTF8String;
}

- (size_t)getCString: (char *)cString_
	   maxLength: (size_t)maxLength
	    encoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	return [self getCString: cString_
		      maxLength: maxLength
		       encoding: encoding];
}

- (const char *)cStringWithEncoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	return [self cStringWithEncoding: encoding];
}

- (size_t)length
{
	[self finishInitialization];
	return self.length;
}

- (size_t)UTF8StringLength
{
	[self finishInitialization];
	return self.UTF8StringLength;
}

- (size_t)cStringLengthWithEncoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	return [self cStringLengthWithEncoding: encoding];
}

- (OFComparisonResult)caseInsensitiveCompare: (OFString *)string
{
	[self finishInitialization];
	return [self caseInsensitiveCompare: string];
}

- (OFUnichar)characterAtIndex: (size_t)idx
{
	[self finishInitialization];
	return [self characterAtIndex: idx];
}

- (void)getCharacters: (OFUnichar *)buffer inRange: (OFRange)range
{
	[self finishInitialization];
	[self getCharacters: buffer inRange: range];
}

- (OFRange)rangeOfString: (OFString *)string
{
	[self finishInitialization];
	return [self rangeOfString: string];
}

- (OFRange)rangeOfString: (OFString *)string
		 options: (OFStringSearchOptions)options
{
	[self finishInitialization];
	return [self rangeOfString: string options: options];
}

- (OFRange)rangeOfString: (OFString *)string
		 options: (OFStringSearchOptions)options
		   range: (OFRange)range
{
	[self finishInitialization];
	return [self rangeOfString: string options: options range: range];
}

- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet
{
	[self finishInitialization];
	return [self indexOfCharacterFromSet: characterSet];
}

- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet
			  options: (OFStringSearchOptions)options
{
	[self finishInitialization];
	return [self indexOfCharacterFromSet: characterSet options: options];
}

- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet
			  options: (OFStringSearchOptions)options
			    range: (OFRange)range
{
	[self finishInitialization];
	return [self indexOfCharacterFromSet: characterSet
				     options: options
				       range: range];
}

- (bool)containsString: (OFString *)string
{
	[self finishInitialization];
	return [self containsString: string];
}

- (OFString *)substringFromIndex: (size_t)idx
{
	[self finishInitialization];
	return [self substringFromIndex: idx];
}

- (OFString *)substringToIndex: (size_t)idx
{
	[self finishInitialization];
	return [self substringToIndex: idx];
}

- (OFString *)substringWithRange: (OFRange)range
{
	[self finishInitialization];
	return [self substringWithRange: range];
}

- (OFString *)stringByAppendingString: (OFString *)string
{
	[self finishInitialization];
	return [self stringByAppendingString: string];
}

- (OFString *)stringByAppendingFormat: (OFConstantString *)format
			    arguments: (va_list)arguments
{
	[self finishInitialization];
	return [self stringByAppendingFormat: format arguments: arguments];
}

- (OFString *)stringByAppendingPathComponent: (OFString *)component
{
	[self finishInitialization];
	return [self stringByAppendingPathComponent: component];
}

- (OFString *)stringByPrependingString: (OFString *)string
{
	[self finishInitialization];
	return [self stringByPrependingString: string];
}

- (OFString *)stringByReplacingOccurrencesOfString: (OFString *)string
					withString: (OFString *)replacement
{
	[self finishInitialization];
	return [self stringByReplacingOccurrencesOfString: string
					       withString: replacement];
}

- (OFString *)stringByReplacingOccurrencesOfString: (OFString *)string
					withString: (OFString *)replacement
					   options: (int)options
					     range: (OFRange)range
{
	[self finishInitialization];
	return [self stringByReplacingOccurrencesOfString: string
					       withString: replacement
						  options: options
						    range: range];
}

- (OFString *)uppercaseString
{
	[self finishInitialization];
	return self.uppercaseString;
}

- (OFString *)lowercaseString
{
	[self finishInitialization];
	return self.lowercaseString;
}

- (OFString *)capitalizedString
{
	[self finishInitialization];
	return self.capitalizedString;
}

- (OFString *)stringByDeletingLeadingWhitespaces
{
	[self finishInitialization];
	return self.stringByDeletingLeadingWhitespaces;
}

- (OFString *)stringByDeletingTrailingWhitespaces
{
	[self finishInitialization];
	return self.stringByDeletingTrailingWhitespaces;
}

- (OFString *)stringByDeletingEnclosingWhitespaces
{
	[self finishInitialization];
	return self.stringByDeletingEnclosingWhitespaces;
}

- (bool)hasPrefix: (OFString *)prefix
{
	[self finishInitialization];
	return [self hasPrefix: prefix];
}

- (bool)hasSuffix: (OFString *)suffix
{
	[self finishInitialization];
	return [self hasSuffix: suffix];
}

- (OFArray *)componentsSeparatedByString: (OFString *)delimiter
{
	[self finishInitialization];
	return [self componentsSeparatedByString: delimiter];
}

- (OFArray *)componentsSeparatedByString: (OFString *)delimiter
				 options: (OFStringSeparationOptions)options
{
	[self finishInitialization];
	return [self componentsSeparatedByString: delimiter options: options];
}

- (OFArray *)
    componentsSeparatedByCharactersInSet: (OFCharacterSet *)characterSet
{
	[self finishInitialization];
	return [self componentsSeparatedByCharactersInSet: characterSet];
}

- (OFArray *)
    componentsSeparatedByCharactersInSet: (OFCharacterSet *)characterSet
				 options: (OFStringSeparationOptions)options
{
	[self finishInitialization];
	return [self componentsSeparatedByCharactersInSet: characterSet
						  options: options];
}

- (OFArray *)pathComponents
{
	[self finishInitialization];
	return self.pathComponents;
}

- (OFString *)lastPathComponent
{
	[self finishInitialization];
	return self.lastPathComponent;
}

- (OFString *)stringByDeletingLastPathComponent
{
	[self finishInitialization];
	return self.stringByDeletingLastPathComponent;
}

- (long long)longLongValue
{
	[self finishInitialization];
	return self.longLongValue;
}

- (long long)longLongValueWithBase: (int)base
{
	[self finishInitialization];
	return [self longLongValueWithBase: base];
}

- (unsigned long long)unsignedLongLongValue
{
	[self finishInitialization];
	return self.unsignedLongLongValue;
}

- (unsigned long long)unsignedLongLongValueWithBase: (int)base
{
	[self finishInitialization];
	return [self unsignedLongLongValueWithBase: base];
}

- (float)floatValue
{
	[self finishInitialization];
	return self.floatValue;
}

- (double)doubleValue
{
	[self finishInitialization];
	return self.doubleValue;
}

- (const OFUnichar *)characters
{
	[self finishInitialization];
	return self.characters;
}

- (const OFChar16 *)UTF16String
{
	[self finishInitialization];
	return self.UTF16String;
}

- (const OFChar16 *)UTF16StringWithByteOrder: (OFByteOrder)byteOrder
{
	[self finishInitialization];
	return [self UTF16StringWithByteOrder: byteOrder];
}

- (size_t)UTF16StringLength
{
	[self finishInitialization];
	return self.UTF16StringLength;
}

- (const OFChar32 *)UTF32String
{
	[self finishInitialization];
	return self.UTF32String;
}

- (const OFChar32 *)UTF32StringWithByteOrder: (OFByteOrder)byteOrder
{
	[self finishInitialization];
	return [self UTF32StringWithByteOrder: byteOrder];
}

- (OFData *)dataWithEncoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	return [self dataWithEncoding: encoding];
}

#ifdef OF_HAVE_UNICODE_TABLES
- (OFString *)decomposedStringWithCanonicalMapping
{
	[self finishInitialization];
	return self.decomposedStringWithCanonicalMapping;
}

- (OFString *)decomposedStringWithCompatibilityMapping
{
	[self finishInitialization];
	return self.decomposedStringWithCompatibilityMapping;
}
#endif

#ifdef OF_WINDOWS
- (OFString *)stringByExpandingWindowsEnvironmentStrings
{
	[self finishInitialization];
	return self.stringByExpandingWindowsEnvironmentStrings;
}
#endif

#ifdef OF_HAVE_FILES
- (void)writeToFile: (OFString *)path
{
	[self finishInitialization];
	[self writeToFile: path];
}

- (void)writeToFile: (OFString *)path encoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	[self writeToFile: path encoding: encoding];
}
#endif

- (void)writeToURL: (OFURL *)URL
{
	[self finishInitialization];
	[self writeToURL: URL];
}

- (void)writeToURL: (OFURL *)URL encoding: (OFStringEncoding)encoding
{
	[self finishInitialization];
	[self writeToURL: URL encoding: encoding];
}

#ifdef OF_HAVE_BLOCKS
- (void)enumerateLinesUsingBlock: (OFStringLineEnumerationBlock)block
{
	[self finishInitialization];
	[self enumerateLinesUsingBlock: block];
}
#endif
@end
