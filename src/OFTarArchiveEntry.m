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

#import "OFTarArchiveEntry.h"
#import "OFTarArchiveEntry+Private.h"
#import "OFDate.h"
#import "OFStream.h"
#import "OFString.h"

#import "OFOutOfRangeException.h"

static OFString *
stringFromBuffer(const unsigned char *buffer, size_t length,
    OFStringEncoding encoding)
{
	for (size_t i = 0; i < length; i++)
		if (buffer[i] == '\0')
			length = i;

	return [OFString stringWithCString: (const char *)buffer
				  encoding: encoding
				    length: length];
}

static void
stringToBuffer(unsigned char *buffer, OFString *string, size_t length,
    OFStringEncoding encoding)
{
	size_t cStringLength = [string cStringLengthWithEncoding: encoding];

	if (cStringLength > length)
		@throw [OFOutOfRangeException exception];

	memcpy(buffer, [string cStringWithEncoding: encoding], cStringLength);

	for (size_t i = cStringLength; i < length; i++)
		buffer[i] = '\0';
}

static unsigned long long
octalValueFromBuffer(const unsigned char *buffer, size_t length,
    unsigned long long max)
{
	unsigned long long value = 0;

	if (length == 0)
		return 0;

	if (buffer[0] == 0x80) {
		for (size_t i = 1; i < length; i++)
			value = (value << 8) | buffer[i];
	} else
		value = [stringFromBuffer(buffer, length,
		    OFStringEncodingASCII) unsignedLongLongValueWithBase: 8];

	if (value > max)
		@throw [OFOutOfRangeException exception];

	return value;
}

@implementation OFTarArchiveEntry
+ (instancetype)entryWithFileName: (OFString *)fileName
{
	return [[[self alloc] initWithFileName: fileName] autorelease];
}

- (instancetype)init
{
	OF_INVALID_INIT_METHOD
}

- (instancetype)of_initWithHeader: (unsigned char [512])header
			 encoding: (OFStringEncoding)encoding
{
	self = [super init];

	@try {
		void *pool = objc_autoreleasePoolPush();
		OFString *targetFileName;

		_fileName = [stringFromBuffer(header, 100, encoding) copy];
		_mode = (unsigned long)octalValueFromBuffer(
		    header + 100, 8, ULONG_MAX);
		_UID = (unsigned long)octalValueFromBuffer(
		    header + 108, 8, ULONG_MAX);
		_GID = (unsigned long)octalValueFromBuffer(
		    header + 116, 8, ULONG_MAX);
		_size = (unsigned long long)octalValueFromBuffer(
		    header + 124, 12, ULLONG_MAX);
		_modificationDate = [[OFDate alloc]
		    initWithTimeIntervalSince1970:
		    (OFTimeInterval)octalValueFromBuffer(
		    header + 136, 12, ULLONG_MAX)];
		_type = header[156];

		targetFileName = stringFromBuffer(header + 157, 100, encoding);
		if (targetFileName.length > 0)
			_targetFileName = [targetFileName copy];

		if (_type == '\0')
			_type = OFTarArchiveEntryTypeFile;

		if (memcmp(header + 257, "ustar\0" "00", 8) == 0) {
			OFString *prefix;

			_owner = [stringFromBuffer(header + 265, 32, encoding)
			    copy];
			_group = [stringFromBuffer(header + 297, 32, encoding)
			    copy];

			_deviceMajor = (unsigned long)octalValueFromBuffer(
			    header + 329, 8, ULONG_MAX);
			_deviceMinor = (unsigned long)octalValueFromBuffer(
			    header + 337, 8, ULONG_MAX);

			prefix = stringFromBuffer(header + 345, 155, encoding);
			if (prefix.length > 0) {
				OFString *fileName = [OFString
				    stringWithFormat: @"%@/%@",
						      prefix, _fileName];
				[_fileName release];
				_fileName = [fileName copy];
			}
		}

		objc_autoreleasePoolPop(pool);
	} @catch (id e) {
		[self release];
		@throw e;
	}

	return self;
}

- (instancetype)initWithFileName: (OFString *)fileName
{
	self = [super init];

	@try {
		_fileName = [fileName copy];
		_type = OFTarArchiveEntryTypeFile;
		_mode = 0644;
	} @catch (id e) {
		[self release];
		@throw e;
	}

	return self;
}

- (void)dealloc
{
	[_fileName release];
	[_modificationDate release];
	[_targetFileName release];
	[_owner release];
	[_group release];

	[super dealloc];
}

- (id)copy
{
	return [self retain];
}

- (id)mutableCopy
{
	OFTarArchiveEntry *copy = [[OFMutableTarArchiveEntry alloc]
	    initWithFileName: _fileName];

	@try {
		copy->_mode = _mode;
		copy->_size = _size;
		copy->_modificationDate = [_modificationDate copy];
		copy->_type = _type;
		copy->_targetFileName = [_targetFileName copy];
		copy->_owner = [_owner copy];
		copy->_group = [_group copy];
		copy->_deviceMajor = _deviceMajor;
		copy->_deviceMinor = _deviceMinor;
	} @catch (id e) {
		[copy release];
		@throw e;
	}

	return copy;
}

- (OFString *)fileName
{
	return _fileName;
}

- (unsigned long)mode
{
	return _mode;
}

- (unsigned long)UID
{
	return _UID;
}

- (unsigned long)GID
{
	return _GID;
}

- (unsigned long long)size
{
	return _size;
}

- (OFDate *)modificationDate
{
	return _modificationDate;
}

- (OFTarArchiveEntryType)type
{
	return _type;
}

- (OFString *)targetFileName
{
	return _targetFileName;
}

- (OFString *)owner
{
	return _owner;
}

- (OFString *)group
{
	return _group;
}

- (unsigned long)deviceMajor
{
	return _deviceMajor;
}

- (unsigned long)deviceMinor
{
	return _deviceMinor;
}

- (OFString *)description
{
	void *pool = objc_autoreleasePoolPush();
	OFString *ret = [OFString stringWithFormat: @"<%@:\n"
	     @"\tFile name = %@\n"
	     @"\tMode = %06o\n"
	     @"\tUID = %u\n"
	     @"\tGID = %u\n"
	     @"\tSize = %" PRIu64 @"\n"
	     @"\tModification date = %@\n"
	     @"\tType = %u\n"
	     @"\tTarget file name = %@\n"
	     @"\tOwner = %@\n"
	     @"\tGroup = %@\n"
	     @"\tDevice major = %" PRIu32 @"\n"
	     @"\tDevice minor = %" PRIu32 @"\n"
	     @">",
	    self.class, _fileName, _mode, _UID, _GID, _size, _modificationDate,
	    _type, _targetFileName, _owner, _group, _deviceMajor, _deviceMinor];

	[ret retain];

	objc_autoreleasePoolPop(pool);

	return [ret autorelease];
}

- (void)of_writeToStream: (OFStream *)stream
		encoding: (OFStringEncoding)encoding
{
	unsigned char buffer[512];
	unsigned long long modificationDate;
	uint16_t checksum = 0;

	stringToBuffer(buffer, _fileName, 100, encoding);
	stringToBuffer(buffer + 100,
	    [OFString stringWithFormat: @"%06" PRIo16 " ", _mode], 8,
	    OFStringEncodingASCII);
	stringToBuffer(buffer + 108,
	    [OFString stringWithFormat: @"%06" PRIo16 " ", _UID], 8,
	    OFStringEncodingASCII);
	stringToBuffer(buffer + 116,
	    [OFString stringWithFormat: @"%06" PRIo16 " ", _GID], 8,
	    OFStringEncodingASCII);
	stringToBuffer(buffer + 124,
	    [OFString stringWithFormat: @"%011" PRIo64 " ", _size], 12,
	    OFStringEncodingASCII);
	modificationDate = _modificationDate.timeIntervalSince1970;
	stringToBuffer(buffer + 136,
	    [OFString stringWithFormat: @"%011llo", modificationDate],
	    12, OFStringEncodingASCII);

	/*
	 * During checksumming, the checksum field is expected to be set to 8
	 * spaces.
	 */
	memset(buffer + 148, ' ', 8);

	buffer[156] = _type;
	stringToBuffer(buffer + 157, _targetFileName, 100, encoding);

	/* ustar */
	memcpy(buffer + 257, "ustar\0" "00", 8);
	stringToBuffer(buffer + 265, _owner, 32, encoding);
	stringToBuffer(buffer + 297, _group, 32, encoding);
	stringToBuffer(buffer + 329,
	    [OFString stringWithFormat: @"%06" PRIo32 " ", _deviceMajor], 8,
	    OFStringEncodingASCII);
	stringToBuffer(buffer + 337,
	    [OFString stringWithFormat: @"%06" PRIo32 " ", _deviceMinor], 8,
	    OFStringEncodingASCII);
	memset(buffer + 345, '\0', 155 + 12);

	/* Fill in the checksum */
	for (size_t i = 0; i < 500; i++)
		checksum += buffer[i];
	stringToBuffer(buffer + 148,
	    [OFString stringWithFormat: @"%06" PRIo16, checksum], 7,
	    OFStringEncodingASCII);

	[stream writeBuffer: buffer length: sizeof(buffer)];
}
@end
