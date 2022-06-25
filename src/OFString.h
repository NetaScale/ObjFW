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

#ifndef OBJFW_OF_STRING_H
#define OBJFW_OF_STRING_H

#ifndef __STDC_LIMIT_MACROS
# define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif

#include "objfw-defs.h"

#ifdef OF_HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#ifdef OF_HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include "OFObject.h"
#ifdef __OBJC__
# import "OFSerialization.h"
# import "OFJSONRepresentation.h"
# import "OFMessagePackRepresentation.h"
#endif

OF_ASSUME_NONNULL_BEGIN

/** @file */

#ifdef __OBJC__
@class OFConstantString;
@class OFString;
#else
typedef void OFString;
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
typedef char16_t OFChar16;
typedef char32_t OFChar32;
#else
typedef uint_least16_t OFChar16;
typedef uint_least32_t OFChar32;
#endif
typedef OFChar32 OFUnichar;

/**
 * @brief The encoding of a string.
 */
typedef enum {
	/*
	 * UTF-8 *has* to be 0, so that if the current @ref OFLocale is
	 * `nil`, `[OFLocale encoding]` returns UTF-8.
	 */
	/** UTF-8 */
	OFStringEncodingUTF8,
	/** ASCII */
	OFStringEncodingASCII,
	/** ISO 8859-1 */
	OFStringEncodingISO8859_1,
	/** ISO 8859-2 */
	OFStringEncodingISO8859_2,
	/** ISO 8859-3 */
	OFStringEncodingISO8859_3,
	/** ISO 8859-15 */
	OFStringEncodingISO8859_15,
	/** Windows-1251 */
	OFStringEncodingWindows1251,
	/** Windows-1252 */
	OFStringEncodingWindows1252,
	/** Codepage 437 */
	OFStringEncodingCodepage437,
	/** Codepage 850 */
	OFStringEncodingCodepage850,
	/** Codepage 858 */
	OFStringEncodingCodepage858,
	/** Mac OS Roman */
	OFStringEncodingMacRoman,
	/** KOI8-R */
	OFStringEncodingKOI8R,
	/** KOI8-U */
	OFStringEncodingKOI8U,
	/** Try to automatically detect the encoding */
	OFStringEncodingAutodetect = 0xFF
} OFStringEncoding;

/**
 * @brief Options for searching in strings.
 *
 * This is a bit mask.
 */
typedef enum {
	/** Search backwards in the string */
	OFStringSearchBackwards = 1
} OFStringSearchOptions;

/**
 * @brief Options for separating strings.
 *
 * This is a bit mask.
 */
typedef enum {
	/** Skip empty components */
	OFStringSkipEmptyComponents = 1
} OFStringSeparationOptions;

#ifdef OF_HAVE_BLOCKS
/**
 * @brief A block for enumerating the lines of a string.
 *
 * @param line The current line
 * @param stop A pointer to a variable that can be set to true to stop the
 *	       enumeration
 */
typedef void (^OFStringLineEnumerationBlock)(OFString *line, bool *stop);
#endif

#ifdef __OBJC__
@class OFArray OF_GENERIC(ObjectType);
@class OFCharacterSet;
@class OFURL;

/**
 * @class OFString OFString.h ObjFW/OFString.h
 *
 * @brief A class for handling strings.
 */
@interface OFString: OFObject <OFCopying, OFMutableCopying, OFComparing,
    OFSerialization, OFJSONRepresentation, OFMessagePackRepresentation>
/**
 * @brief The length of the string in Unicode codepoints.
 */
@property (readonly, nonatomic) size_t length;

/**
 * @brief The OFString as a UTF-8 encoded C string.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 */
@property (readonly, nonatomic) const char *UTF8String OF_RETURNS_INNER_POINTER;

/**
 * @brief The number of bytes the string needs in UTF-8 encoding.
 */
@property (readonly, nonatomic) size_t UTF8StringLength;

/**
 * @brief The string in uppercase.
 */
@property (readonly, nonatomic) OFString *uppercaseString;

/**
 * @brief The string in lowercase.
 */
@property (readonly, nonatomic) OFString *lowercaseString;

/**
 * @brief The string in capitalized form.
 *
 * @note This only considers spaces, tabs and newlines to be word delimiters!
 *	 Also note that this might change in the future to all word delimiters
 *	 specified by Unicode!
 */
@property (readonly, nonatomic) OFString *capitalizedString;

/**
 * @brief The decimal value of the string as a `long long`.
 *
 * Leading and trailing whitespaces are ignored.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 *
 * If the number is too big to fit into a `long long`, an
 * @ref OFOutOfRangeException is thrown.
 */
@property (readonly, nonatomic) long long longLongValue;

/**
 * @brief The decimal value of the string as an `unsigned long long`.
 *
 * Leading and trailing whitespaces are ignored.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 *
 * If the number is too big to fit into an `unsigned long long`, an
 * @ref OFOutOfRangeException is thrown.
 */
@property (readonly, nonatomic) unsigned long long unsignedLongLongValue;

/**
 * @brief The float value of the string as a float.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 */
@property (readonly, nonatomic) float floatValue;

/**
 * @brief The double value of the string as a double.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 */
@property (readonly, nonatomic) double doubleValue;

/**
 * @brief The string as an array of Unicode characters.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 */
@property (readonly, nonatomic) const OFUnichar *characters
    OF_RETURNS_INNER_POINTER;

/**
 * @brief The string in UTF-16 encoding with native byte order.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 */
@property (readonly, nonatomic) const OFChar16 *UTF16String
    OF_RETURNS_INNER_POINTER;

/**
 * @brief The length of the string in UTF-16 characters.
 */
@property (readonly, nonatomic) size_t UTF16StringLength;

/**
 * @brief The string in UTF-32 encoding with native byte order.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 */
@property (readonly, nonatomic) const OFChar32 *UTF32String
    OF_RETURNS_INNER_POINTER;

/**
 * @brief The string with leading whitespaces deleted.
 */
@property (readonly, nonatomic) OFString *stringByDeletingLeadingWhitespaces;

/**
 * @brief The string with trailing whitespaces deleted.
 */
@property (readonly, nonatomic) OFString *stringByDeletingTrailingWhitespaces;

/**
 * @brief The string with leading and trailing whitespaces deleted.
 */
@property (readonly, nonatomic) OFString *stringByDeletingEnclosingWhitespaces;

# ifdef OF_HAVE_UNICODE_TABLES
/**
 * @brief The string in Unicode Normalization Form D (NFD).
 */
@property (readonly, nonatomic) OFString *decomposedStringWithCanonicalMapping;

/**
 * @brief The string in Unicode Normalization Form KD (NFKD).
 */
@property (readonly, nonatomic)
    OFString *decomposedStringWithCompatibilityMapping;
# endif

# ifdef OF_WINDOWS
/**
 * @brief The string with the Windows Environment Strings expanded.
 */
@property (readonly, nonatomic)
    OFString *stringByExpandingWindowsEnvironmentStrings;
# endif

/**
 * @brief Creates a new OFString.
 *
 * @return A new, autoreleased OFString
 */
+ (instancetype)string;

/**
 * @brief Creates a new OFString from a UTF-8 encoded C string.
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF8String: (const char *)UTF8String;

/**
 * @brief Creates a new OFString from a UTF-8 encoded C string with the
 *	  specified length.
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param UTF8StringLength The length of the UTF-8 encoded C string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF8String: (const char *)UTF8String
			      length: (size_t)UTF8StringLength;

/**
 * @brief Creates a new OFString from a UTF-8 encoded C string without copying
 *	  the string, if possible.
 *
 * If initialization fails for whatever reason, the passed C string is *not*
 * free'd if `freeWhenDone` is true.
 *
 * @note OFMutableString always creates a copy!
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param freeWhenDone Whether to free the C string when the OFString gets
 *		       deallocated
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF8StringNoCopy: (char *)UTF8String
			      freeWhenDone: (bool)freeWhenDone;

/**
 * @brief Creates a new OFString from a UTF-8 encoded C string with the
 *	  specified length without copying the string, if possible.
 *
 * If initialization fails for whatever reason, the passed C string is *not*
 * free'd if `freeWhenDone` is true.
 *
 * @note OFMutableString always creates a copy!
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param UTF8StringLength The length of the UTF-8 encoded C string
 * @param freeWhenDone Whether to free the C string when the OFString gets
 *		       deallocated
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF8StringNoCopy: (char *)UTF8String
				    length: (size_t)UTF8StringLength
			      freeWhenDone: (bool)freeWhenDone;

/**
 * @brief Creates a new OFString from a C string with the specified encoding.
 *
 * @param cString A C string to initialize the OFString with
 * @param encoding The encoding of the C string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithCString: (const char *)cString
			 encoding: (OFStringEncoding)encoding;

/**
 * @brief Creates a new OFString from a C string with the specified encoding
 *	  and length.
 *
 * @param cString A C string to initialize the OFString with
 * @param encoding The encoding of the C string
 * @param cStringLength The length of the C string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithCString: (const char *)cString
			 encoding: (OFStringEncoding)encoding
			   length: (size_t)cStringLength;

/**
 * @brief Creates a new OFString from OFData with the specified encoding.
 *
 * @param data OFData with the contents of the string
 * @param encoding The encoding in which the string is stored in the OFData
 * @return An new autoreleased OFString
 */
+ (instancetype)stringWithData: (OFData *)data
		      encoding: (OFStringEncoding)encoding;

/**
 * @brief Creates a new OFString from another string.
 *
 * @param string A string to initialize the OFString with
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithString: (OFString *)string;

/**
 * @brief Creates a new OFString from a Unicode string with the specified
 *	  length.
 *
 * @param characters An array of Unicode characters
 * @param length The length of the Unicode character array
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithCharacters: (const OFUnichar *)characters
			      length: (size_t)length;

/**
 * @brief Creates a new OFString from a UTF-16 encoded string.
 *
 * @param string The UTF-16 string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF16String: (const OFChar16 *)string;

/**
 * @brief Creates a new OFString from a UTF-16 encoded string with the
 *	  specified length.
 *
 * @param string The UTF-16 string
 * @param length The length of the UTF-16 string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF16String: (const OFChar16 *)string
			       length: (size_t)length;

/**
 * @brief Creates a new OFString from a UTF-16 encoded string, assuming the
 *	  specified byte order if no byte order mark is found.
 *
 * @param string The UTF-16 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF16String: (const OFChar16 *)string
			    byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Creates a new OFString from a UTF-16 encoded string with the
 *	  specified length, assuming the specified byte order if no byte order
 *	  mark is found.
 *
 * @param string The UTF-16 string
 * @param length The length of the UTF-16 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF16String: (const OFChar16 *)string
			       length: (size_t)length
			    byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Creates a new OFString from a UTF-32 encoded string.
 *
 * @param string The UTF-32 string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF32String: (const OFChar32 *)string;

/**
 * @brief Creates a new OFString from a UTF-32 encoded string with the
 *	  specified length.
 *
 * @param string The UTF-32 string
 * @param length The length of the UTF-32 string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF32String: (const OFChar32 *)string
			       length: (size_t)length;

/**
 * @brief Creates a new OFString from a UTF-32 encoded string, assuming the
 *	  specified byte order if no byte order mark is found.
 *
 * @param string The UTF-32 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF32String: (const OFChar32 *)string
			    byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Creates a new OFString from a UTF-32 encoded string with the
 *	  specified length, assuming the specified byte order if no byte order
 *	  mark is found.
 *
 * @param string The UTF-32 string
 * @param length The length of the UTF-32 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithUTF32String: (const OFChar32 *)string
			       length: (size_t)length
			    byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Creates a new OFString from a format string.
 *
 * See printf for the format syntax. As an addition, `%@` is available as
 * format specifier for objects, `%C` for `OFUnichar` and `%S` for
 * `const OFUnichar *`.
 *
 * @param format A string used as format to initialize the OFString
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithFormat: (OFConstantString *)format, ...;

# ifdef OF_HAVE_FILES
/**
 * @brief Creates a new OFString with the contents of the specified UTF-8
 *	  encoded file.
 *
 * @param path The path to the file
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithContentsOfFile: (OFString *)path;

/**
 * @brief Creates a new OFString with the contents of the specified file in the
 *	  specified encoding.
 *
 * @param path The path to the file
 * @param encoding The encoding of the file
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithContentsOfFile: (OFString *)path
				encoding: (OFStringEncoding)encoding;
# endif

/**
 * @brief Creates a new OFString with the contents of the specified URL.
 *
 * If the URL's scheme is file, it tries UTF-8 encoding.
 *
 * If the URL's scheme is http(s), it tries to detect the encoding from the HTTP
 * headers. If it could not detect the encoding using the HTTP headers, it tries
 * UTF-8.
 *
 * @param URL The URL to the contents for the string
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithContentsOfURL: (OFURL *)URL;

/**
 * @brief Creates a new OFString with the contents of the specified URL in the
 *	  specified encoding.
 *
 * @param URL The URL to the contents for the string
 * @param encoding The encoding to assume
 * @return A new autoreleased OFString
 */
+ (instancetype)stringWithContentsOfURL: (OFURL *)URL
			       encoding: (OFStringEncoding)encoding;

/**
 * @brief Initializes an already allocated OFString from a UTF-8 encoded C
 *	  string.
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @return An initialized OFString
 */
- (instancetype)initWithUTF8String: (const char *)UTF8String;

/**
 * @brief Initializes an already allocated OFString from a UTF-8 encoded C
 *	  string with the specified length.
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param UTF8StringLength The length of the UTF-8 encoded C string
 * @return An initialized OFString
 */
- (instancetype)initWithUTF8String: (const char *)UTF8String
			    length: (size_t)UTF8StringLength;

/**
 * @brief Initializes an already allocated OFString from an UTF-8 encoded C
 *	  string without copying the string, if possible.
 *
 * If initialization fails for whatever reason, the passed C string is *not*
 * free'd if `freeWhenDone` is true.
 *
 * @note OFMutableString always creates a copy!
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param freeWhenDone Whether to free the C string when it is not needed
 *		       anymore
 * @return An initialized OFString
 */
- (instancetype)initWithUTF8StringNoCopy: (char *)UTF8String
			    freeWhenDone: (bool)freeWhenDone;

/**
 * @brief Initializes an already allocated OFString from an UTF-8 encoded C
 *	  string with the specified length without copying the string, if
 *	  possible.
 *
 * If initialization fails for whatever reason, the passed C string is *not*
 * free'd if `freeWhenDone` is true.
 *
 * @note OFMutableString always creates a copy!
 *
 * @param UTF8String A UTF-8 encoded C string to initialize the OFString with
 * @param UTF8StringLength The length of the UTF-8 encoded C string
 * @param freeWhenDone Whether to free the C string when it is not needed
 *		       anymore
 * @return An initialized OFString
 */
- (instancetype)initWithUTF8StringNoCopy: (char *)UTF8String
				  length: (size_t)UTF8StringLength
			    freeWhenDone: (bool)freeWhenDone;

/**
 * @brief Initializes an already allocated OFString from a C string with the
 *	  specified encoding.
 *
 * @param cString A C string to initialize the OFString with
 * @param encoding The encoding of the C string
 * @return An initialized OFString
 */
- (instancetype)initWithCString: (const char *)cString
		       encoding: (OFStringEncoding)encoding;

/**
 * @brief Initializes an already allocated OFString from a C string with the
 *	  specified encoding and length.
 *
 * @param cString A C string to initialize the OFString with
 * @param encoding The encoding of the C string
 * @param cStringLength The length of the C string
 * @return An initialized OFString
 */
- (instancetype)initWithCString: (const char *)cString
		       encoding: (OFStringEncoding)encoding
			 length: (size_t)cStringLength;

/**
 * @brief Initializes an already allocated OFString from OFData with the
 *	  specified encoding.
 *
 * @param data OFData with the contents of the string
 * @param encoding The encoding in which the string is stored in the OFData
 * @return An initialized OFString
 */
- (instancetype)initWithData: (OFData *)data
		    encoding: (OFStringEncoding)encoding;

/**
 * @brief Initializes an already allocated OFString with another string.
 *
 * @param string A string to initialize the OFString with
 * @return An initialized OFString
 */
- (instancetype)initWithString: (OFString *)string;

/**
 * @brief Initializes an already allocated OFString with a Unicode string with
 *	  the specified length.
 *
 * @param characters An array of Unicode characters
 * @param length The length of the Unicode character array
 * @return An initialized OFString
 */
- (instancetype)initWithCharacters: (const OFUnichar *)characters
			    length: (size_t)length;

/**
 * @brief Initializes an already allocated OFString with a UTF-16 string.
 *
 * @param string The UTF-16 string
 * @return An initialized OFString
 */
- (instancetype)initWithUTF16String: (const OFChar16 *)string;

/**
 * @brief Initializes an already allocated OFString with a UTF-16 string with
 *	  the specified length.
 *
 * @param string The UTF-16 string
 * @param length The length of the UTF-16 string
 * @return An initialized OFString
 */
- (instancetype)initWithUTF16String: (const OFChar16 *)string
			     length: (size_t)length;

/**
 * @brief Initializes an already allocated OFString with a UTF-16 string,
 *	  assuming the specified byte order if no byte order mark is found.
 *
 * @param string The UTF-16 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return An initialized OFString
 */
- (instancetype)initWithUTF16String: (const OFChar16 *)string
			  byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Initializes an already allocated OFString with a UTF-16 string with
 *	  the specified length, assuming the specified byte order if no byte
 *	  order mark is found.
 *
 * @param string The UTF-16 string
 * @param length The length of the UTF-16 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return An initialized OFString
 */
- (instancetype)initWithUTF16String: (const OFChar16 *)string
			     length: (size_t)length
			  byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Initializes an already allocated OFString with a UTF-32 string.
 *
 * @param string The UTF-32 string
 * @return An initialized OFString
 */
- (instancetype)initWithUTF32String: (const OFChar32 *)string;

/**
 * @brief Initializes an already allocated OFString with a UTF-32 string with
 *	  the specified length
 *
 * @param string The UTF-32 string
 * @param length The length of the UTF-32 string
 * @return An initialized OFString
 */
- (instancetype)initWithUTF32String: (const OFChar32 *)string
			     length: (size_t)length;

/**
 * @brief Initializes an already allocated OFString with a UTF-32 string,
 *	  assuming the specified byte order if no byte order mark is found.
 *
 * @param string The UTF-32 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return An initialized OFString
 */
- (instancetype)initWithUTF32String: (const OFChar32 *)string
			  byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Initializes an already allocated OFString with a UTF-32 string with
 *	  the specified length, assuming the specified byte order if no byte
 *	  order mark is found.
 *
 * @param string The UTF-32 string
 * @param length The length of the UTF-32 string
 * @param byteOrder The byte order to assume if there is no byte order mark
 * @return An initialized OFString
 */
- (instancetype)initWithUTF32String: (const OFChar32 *)string
			     length: (size_t)length
			  byteOrder: (OFByteOrder)byteOrder;

/**
 * @brief Initializes an already allocated OFString with a format string.
 *
 * See printf for the format syntax. As an addition, `%@` is available as
 * format specifier for objects, `%C` for `OFUnichar` and `%S` for
 * `const OFUnichar *`.
 *
 * @param format A string used as format to initialize the OFString
 * @return An initialized OFString
 */
- (instancetype)initWithFormat: (OFConstantString *)format, ...;

/**
 * @brief Initializes an already allocated OFString with a format string.
 *
 * See printf for the format syntax. As an addition, `%@` is available as
 * format specifier for objects, `%C` for `OFUnichar` and `%S` for
 * `const OFUnichar *`.
 *
 * @param format A string used as format to initialize the OFString
 * @param arguments The arguments used in the format string
 * @return An initialized OFString
 */
- (instancetype)initWithFormat: (OFConstantString *)format
		     arguments: (va_list)arguments;

# ifdef OF_HAVE_FILES
/**
 * @brief Initializes an already allocated OFString with the contents of the
 *	  specified file in the specified encoding.
 *
 * @param path The path to the file
 * @return An initialized OFString
 */
- (instancetype)initWithContentsOfFile: (OFString *)path;

/**
 * @brief Initializes an already allocated OFString with the contents of the
 *	  specified file in the specified encoding.
 *
 * @param path The path to the file
 * @param encoding The encoding of the file
 * @return An initialized OFString
 */
- (instancetype)initWithContentsOfFile: (OFString *)path
			      encoding: (OFStringEncoding)encoding;
# endif

/**
 * @brief Initializes an already allocated OFString with the contents of the
 *	  specified URL.
 *
 * If the URL's scheme is file, it tries UTF-8 encoding.
 *
 * If the URL's scheme is http(s), it tries to detect the encoding from the HTTP
 * headers. If it could not detect the encoding using the HTTP headers, it tries
 * UTF-8.
 *
 * @param URL The URL to the contents for the string
 * @return An initialized OFString
 */
- (instancetype)initWithContentsOfURL: (OFURL *)URL;

/**
 * @brief Initializes an already allocated OFString with the contents of the
 *	  specified URL in the specified encoding.
 *
 * @param URL The URL to the contents for the string
 * @param encoding The encoding to assume
 * @return An initialized OFString
 */
- (instancetype)initWithContentsOfURL: (OFURL *)URL
			     encoding: (OFStringEncoding)encoding;

/**
 * @brief Writes the OFString into the specified C string with the specified
 *	  encoding.
 *
 * @param cString The C string to write into
 * @param maxLength The maximum number of bytes to write into the C string,
 *		    including the terminating zero
 * @param encoding The encoding to use for writing into the C string
 * @return The number of bytes written into the C string, without the
 *	   terminating zero
 */
- (size_t)getCString: (char *)cString
	   maxLength: (size_t)maxLength
	    encoding: (OFStringEncoding)encoding;

/**
 * @brief Writes the OFString into the specified C string with the specified
 *	  encoding, replacing characters that cannot be represented in the
 *	  specified encoding with a question mark.
 *
 * @param cString The C string to write into
 * @param maxLength The maximum number of bytes to write into the C string,
 *		    including the terminating zero
 * @param encoding The encoding to use for writing into the C string
 * @return The number of bytes written into the C string, without the
 *	   terminating zero
 */
- (size_t)getLossyCString: (char *)cString
		maxLength: (size_t)maxLength
		 encoding: (OFStringEncoding)encoding;

/**
 * @brief Returns the OFString as a C string in the specified encoding.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 *
 * @param encoding The encoding for the C string
 * @return The OFString as a C string in the specified encoding
 */
- (const char *)cStringWithEncoding: (OFStringEncoding)encoding
    OF_RETURNS_INNER_POINTER;

/**
 * @brief Returns the OFString as a C string in the specified encoding,
 *	  replacing characters that cannot be represented in the specified
 *	  encoding with a question mark.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 *
 * @param encoding The encoding for the C string
 * @return The OFString as a C string in the specified encoding
 */
- (const char *)lossyCStringWithEncoding: (OFStringEncoding)encoding
    OF_RETURNS_INNER_POINTER;

/**
 * @brief Returns the number of bytes the string needs in the specified
 *	  encoding.
 *
 * @param encoding The encoding for the string
 * @return The number of bytes the string needs in the specified encoding.
 */
- (size_t)cStringLengthWithEncoding: (OFStringEncoding)encoding;

/**
 * @brief Compares the string to another string.
 *
 * @param string The string to compare the string to
 * @return The result of the comparison
 */
- (OFComparisonResult)compare: (OFString *)string;

/**
 * @brief Compares the string to another string without caring about the case.
 *
 * @param string The string to compare the string to
 * @return The result of the comparison
 */
- (OFComparisonResult)caseInsensitiveCompare: (OFString *)string;

/**
 * @brief Returns the Unicode character at the specified index.
 *
 * @param index The index of the Unicode character to return
 * @return The Unicode character at the specified index
 */
- (OFUnichar)characterAtIndex: (size_t)index;

/**
 * @brief Copies the Unicode characters in the specified range to the specified
 *	  buffer.
 *
 * @param buffer The buffer to store the Unicode characters
 * @param range The range of the Unicode characters to copy
 */
- (void)getCharacters: (OFUnichar *)buffer inRange: (OFRange)range;

/**
 * @brief Returns the range of the first occurrence of the string.
 *
 * @param string The string to search
 * @return The range of the first occurrence of the string or a range with
 *	   `OFNotFound` as start position if it was not found
 */
- (OFRange)rangeOfString: (OFString *)string;

/**
 * @brief Returns the range of the string.
 *
 * @param string The string to search
 * @param options Options modifying search behavior
 * @return The range of the first occurrence of the string or a range with
 *	   `OFNotFound` as start position if it was not found
 */
- (OFRange)rangeOfString: (OFString *)string
		 options: (OFStringSearchOptions)options;

/**
 * @brief Returns the range of the string in the specified range.
 *
 * @param string The string to search
 * @param options Options modifying search behaviour
 * @param range The range in which to search
 * @return The range of the first occurrence of the string or a range with
 *	   `OFNotFound` as start position if it was not found
 */
- (OFRange)rangeOfString: (OFString *)string
		 options: (OFStringSearchOptions)options
		   range: (OFRange)range;

/**
 * @brief Returns the index of the first character from the set.
 *
 * @param characterSet The set of characters to search for
 * @return The index of the first occurrence of a character from the set or
 *	   `OFNotFound` if it was not found
 */
- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet;

/**
 * @brief Returns the index of the first character from the set.
 *
 * @param characterSet The set of characters to search for
 * @param options Options modifying search behaviour
 * @return The index of the first occurrence of a character from the set or
 *	   `OFNotFound` if it was not found
 */
- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet
			  options: (OFStringSearchOptions)options;

/**
 * @brief Returns the index of the first character from the set.
 *
 * @param characterSet The set of characters to search for
 * @param options Options modifying search behaviour
 * @param range The range in which to search
 * @return The index of the first occurrence of a character from the set or
 *	   `OFNotFound` if it was not found
 */
- (size_t)indexOfCharacterFromSet: (OFCharacterSet *)characterSet
			  options: (OFStringSearchOptions)options
			    range: (OFRange)range;

/**
 * @brief Returns whether the string contains the specified string.
 *
 * @param string The string to search
 * @return Whether the string contains the specified string
 */
- (bool)containsString: (OFString *)string;

/**
 * @brief Creates a substring from the specified index to the end.
 *
 * @param idx The index from where the substring should start, inclusive
 * @return The substring from the specified index to the end
 */
- (OFString *)substringFromIndex: (size_t)idx;

/**
 * @brief Creates a substring from the beginning to the specified index.
 *
 * @param idx The index at which the substring should end, exclusive
 * @return The subtring from the beginning to the specified index
 */
- (OFString *)substringToIndex: (size_t)idx;

/**
 * @brief Creates a substring with the specified range.
 *
 * @param range The range of the substring
 * @return The substring as a new autoreleased OFString
 */
- (OFString *)substringWithRange: (OFRange)range;

/**
 * @brief The value of the string in the specified base as a `long long`.
 *
 * Leading and trailing whitespaces are ignored.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 *
 * If the number is too big to fit into a `long long`, an
 * @ref OFOutOfRangeException is thrown.
 *
 * @param base The base to use. If the base is 0, base 16 is assumed if the
 * 	       string starts with 0x (after stripping white spaces). If the
 * 	       string starts with 0, base 8 is assumed. Otherwise, base 10 is
 * 	       assumed.
 * @return The value of the string in the specified base
 */
- (long long)longLongValueWithBase: (int)base;

/**
 * @brief The value of the string in the specified base as an
 *	  `unsigned long long`.
 *
 * Leading and trailing whitespaces are ignored.
 *
 * If the string contains any non-number characters, an
 * @ref OFInvalidFormatException is thrown.
 *
 * If the number is too big to fit into an `unsigned long long`, an
 * @ref OFOutOfRangeException is thrown.
 *
 * @param base The base to use. If the base is 0, base 16 is assumed if the
 * 	       string starts with 0x (after stripping white spaces). If the
 * 	       string starts with 0, base 8 is assumed. Otherwise, base 10 is
 * 	       assumed.
 * @return The value of the string in the specified base
 */
- (unsigned long long)unsignedLongLongValueWithBase: (int)base;

/**
 * @brief Creates a new string by appending another string.
 *
 * @param string The string to append
 * @return A new, autoreleased OFString with the specified string appended
 */
- (OFString *)stringByAppendingString: (OFString *)string;

/**
 * @brief Creates a new string by appending the specified format.
 *
 * @param format A format string which generates the string to append
 * @return A new, autoreleased OFString with the specified format appended
 */
- (OFString *)stringByAppendingFormat: (OFConstantString *)format, ...;

/**
 * @brief Creates a new string by appending the specified format.
 *
 * @param format A format string which generates the string to append
 * @param arguments The arguments used in the format string
 * @return A new, autoreleased OFString with the specified format appended
 */
- (OFString *)stringByAppendingFormat: (OFConstantString *)format
			    arguments: (va_list)arguments;

/**
 * @brief Creates a new string by prepending another string.
 *
 * @param string The string to prepend
 * @return A new autoreleased OFString with the specified string prepended
 */
- (OFString *)stringByPrependingString: (OFString *)string;

/**
 * @brief Creates a new string by replacing the occurrences of the specified
 *	  string with the specified replacement.
 *
 * @param string The string to replace
 * @param replacement The string with which it should be replaced
 * @return A new string with the occurrences of the specified string replaced
 */
- (OFString *)stringByReplacingOccurrencesOfString: (OFString *)string
					withString: (OFString *)replacement;

/**
 * @brief Creates a new string by replacing the occurrences of the specified
 *	  string in the specified range with the specified replacement.
 *
 * @param string The string to replace
 * @param replacement The string with which it should be replaced
 * @param options Options modifying search behaviour.
 *		  Possible values are:
 *		    * None yet
 * @param range The range in which to replace the string
 * @return A new string with the occurrences of the specified string replaced
 */
- (OFString *)stringByReplacingOccurrencesOfString: (OFString *)string
					withString: (OFString *)replacement
					   options: (int)options
					     range: (OFRange)range;

/**
 * @brief Checks whether the string has the specified prefix.
 *
 * @param prefix The prefix to check for
 * @return A boolean whether the string has the specified prefix
 */
- (bool)hasPrefix: (OFString *)prefix;

/**
 * @brief Checks whether the string has the specified suffix.
 *
 * @param suffix The suffix to check for
 * @return A boolean whether the string has the specified suffix
 */
- (bool)hasSuffix: (OFString *)suffix;

/**
 * @brief Separates the string into an array of strings, split by the specified
 *	  delimiter.
 *
 * @param delimiter The delimiter for separating
 * @return An autoreleased OFArray with the separated string
 */
- (OFArray OF_GENERIC(OFString *) *)
    componentsSeparatedByString: (OFString *)delimiter;

/**
 * @brief Separates the string into an array of strings, split by the specified
 *	  delimiter.
 *
 * @param delimiter The delimiter for separating
 * @param options Options according to which the string should be separated
 * @return An autoreleased OFArray with the separated string
 */
- (OFArray OF_GENERIC(OFString *) *)
    componentsSeparatedByString: (OFString *)delimiter
			options: (OFStringSeparationOptions)options;

/**
 * @brief Separates the string into an array of strings, split by characters in
 *	  the specified set.
 *
 * @param characterSet The character set for separating
 * @return An autoreleased OFArray with the separated string
 */
- (OFArray OF_GENERIC(OFString *) *)
    componentsSeparatedByCharactersInSet: (OFCharacterSet *)characterSet;

/**
 * @brief Separates the string into an array of strings, split by characters in
 *	  the specified set.
 *
 * @param characterSet The character set for separating
 * @param options Options according to which the string should be separated
 * @return An autoreleased OFArray with the separated string
 */
- (OFArray OF_GENERIC(OFString *) *)
    componentsSeparatedByCharactersInSet: (OFCharacterSet *)characterSet
				 options: (OFStringSeparationOptions)options;

/**
 * @brief Returns the string in UTF-16 encoding with the specified byte order.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 *
 * @param byteOrder The byte order for the UTF-16 encoding
 * @return The string in UTF-16 encoding with the specified byte order
 */
- (const OFChar16 *)UTF16StringWithByteOrder: (OFByteOrder)byteOrder
    OF_RETURNS_INNER_POINTER;

/**
 * @brief Returns the string in UTF-32 encoding with the specified byte order.
 *
 * The result is valid until the autorelease pool is released. If you want to
 * use the result outside the scope of the current autorelease pool, you have to
 * copy it.
 *
 * @param byteOrder The byte order for the UTF-32 encoding
 * @return The string in UTF-32 encoding with the specified byte order
 */
- (const OFChar32 *)UTF32StringWithByteOrder: (OFByteOrder)byteOrder
    OF_RETURNS_INNER_POINTER;

/**
 * @brief Returns the string as OFData with the specified encoding.
 *
 * @param encoding The encoding to use for the returned OFData
 * @return The string as OFData with the specified encoding
 */
- (OFData *)dataWithEncoding: (OFStringEncoding)encoding;

# ifdef OF_HAVE_FILES
/**
 * @brief Writes the string into the specified file using UTF-8 encoding.
 *
 * @param path The path of the file to write to
 */
- (void)writeToFile: (OFString *)path;

/**
 * @brief Writes the string into the specified file using the specified
 *	  encoding.
 *
 * @param path The path of the file to write to
 * @param encoding The encoding to use to write the string into the file
 */
- (void)writeToFile: (OFString *)path encoding: (OFStringEncoding)encoding;
# endif

/**
 * @brief Writes the string to the specified URL using UTF-8 encoding.
 *
 * @param URL The URL to write to
 */
- (void)writeToURL: (OFURL *)URL;

/**
 * @brief Writes the string to the specified URL using the specified encoding.
 *
 * @param URL The URL to write to
 * @param encoding The encoding to use to write the string to the URL
 */
- (void)writeToURL: (OFURL *)URL encoding: (OFStringEncoding)encoding;

# ifdef OF_HAVE_BLOCKS
/**
 * Enumerates all lines in the receiver using the specified block.
 *
 * @brief block The block to call for each line
 */
- (void)enumerateLinesUsingBlock: (OFStringLineEnumerationBlock)block;
# endif
@end
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Parses the specified string encoding name and returns the
 *	  OFStringEncoding for it.
 *
 * Throws @ref OFInvalidArgumentException if the specified name is not a valid
 * encoding name.
 *
 * @param name The name to parse as a string encoding
 * @return The OFStringEncoding for the specified name
 */
extern OFStringEncoding OFStringEncodingParseName(OFString *name);

/**
 * @brief Returns the name of the specified OFStringEncoding.
 *
 * @param encoding The encoding for which to return the name
 * @return The name of the specified OFStringEncoding
 */
extern OFString *_Nullable OFStringEncodingName(OFStringEncoding encoding);

extern char *_Nullable OFStrDup(const char *_Nonnull);
extern size_t OFUTF8StringEncode(OFUnichar, char *);
extern ssize_t OFUTF8StringDecode(const char *, size_t, OFUnichar *);
extern size_t OFUTF16StringLength(const OFChar16 *);
extern size_t OFUTF32StringLength(const OFChar32 *);
#ifdef __cplusplus
}
#endif

OF_ASSUME_NONNULL_END

#include "OFConstantString.h"
#include "OFMutableString.h"
#ifdef __OBJC__
# import "OFString+CryptographicHashing.h"
# import "OFString+JSONParsing.h"
# ifdef OF_HAVE_FILES
#  import "OFString+PathAdditions.h"
# endif
# import "OFString+PropertyListParsing.h"
# import "OFString+Serialization.h"
# import "OFString+URLEncoding.h"
# import "OFString+XMLEscaping.h"
# import "OFString+XMLUnescaping.h"
#endif

#if defined(__OBJC__) && !defined(NSINTEGER_DEFINED) && !__has_feature(modules)
/*
 * Very *ugly* hack required for string boxing literals to work.
 *
 * This hack is needed in order to work with `@class NSString` from Apple's
 * objc/NSString.h - which is included when using modules - as
 * @compatibility_alias does not work if @class has been used before.
 * For some reason, this makes Clang refer to OFString for string box literals
 * and not to NSString (which would result in a linker error, but would be the
 * correct behavior).
 *
 * TODO: Submit a patch for Clang that makes the boxing classes configurable!
 */
@interface NSString: OFString
@end
#endif

#endif
