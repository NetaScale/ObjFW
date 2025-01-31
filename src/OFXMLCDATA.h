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

#import "OFXMLNode.h"

OF_ASSUME_NONNULL_BEGIN

/**
 * @class OFXMLCDATA OFXMLCDATA.h ObjFW/OFXMLCDATA.h
 *
 * @brief A class representing XML CDATA.
 */
@interface OFXMLCDATA: OFXMLNode
{
	OFString *_CDATA;
	OF_RESERVE_IVARS(OFXMLCDATA, 4)
}

/**
 * @brief Creates a new OFXMLCDATA with the specified string.
 *
 * @param string The string value for the CDATA
 * @return A new OFXMLCDATA
 */
+ (instancetype)CDATAWithString: (OFString *)string;

/**
 * @brief Initializes an already allocated OFXMLCDATA with the specified string.
 *
 * @param string The string value for the CDATA
 * @return An initialized OFXMLCDATA
 */
- (instancetype)initWithString: (OFString *)string;

- (instancetype)initWithSerialization: (OFXMLElement *)element;
@end

OF_ASSUME_NONNULL_END
