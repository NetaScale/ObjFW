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

#import "OFException.h"

OF_ASSUME_NONNULL_BEGIN

@class OFURL;

/**
 * @class OFCopyItemFailedException \
 *	  OFCopyItemFailedException.h ObjFW/OFCopyItemFailedException.h
 *
 * @brief An exception indicating that copying a item failed.
 */
@interface OFCopyItemFailedException: OFException
{
	OFURL *_sourceURL, *_destinationURL;
	int _errNo;
}

/**
 * @brief The URL of the source item.
 */
@property (readonly, nonatomic) OFURL *sourceURL;

/**
 * @brief The destination URL.
 */
@property (readonly, nonatomic) OFURL *destinationURL;

/**
 * @brief The errno of the error that occurred.
 */
@property (readonly, nonatomic) int errNo;

/**
 * @brief Creates a new, autoreleased copy item failed exception.
 *
 * @param sourceURL The URL of the source item
 * @param destinationURL The destination URL
 * @param errNo The errno of the error that occurred
 * @return A new, autoreleased copy item failed exception
 */
+ (instancetype)exceptionWithSourceURL: (OFURL *)sourceURL
			destinationURL: (OFURL *)destinationURL
				 errNo: (int)errNo;

+ (instancetype)exception OF_UNAVAILABLE;

/**
 * @brief Initializes an already allocated copy item failed exception.
 *
 * @param sourceURL The URL of the source item
 * @param destinationURL The destination URL
 * @param errNo The errno of the error that occurred
 * @return An initialized copy item failed exception
 */
- (instancetype)initWithSourceURL: (OFURL *)sourceURL
		   destinationURL: (OFURL *)destinationURL
			    errNo: (int)errNo OF_DESIGNATED_INITIALIZER;

- (instancetype)init OF_UNAVAILABLE;
@end

OF_ASSUME_NONNULL_END
