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

#ifdef OF_BRIDGE_LOCAL_INCLUDES
# import "OFString.h"
#else
# if defined(__has_feature) && __has_feature(modules)
@import ObjFW;
# else
#  import <ObjFW/OFString.h>
# endif
#endif

#import "OFBridging.h"

OF_ASSUME_NONNULL_BEGIN

#ifdef __cplusplus
extern "C" {
#endif
extern int _OFString_NSObject_reference;
#ifdef __cplusplus
}
#endif

/**
 * @category OFString (NSObject) \
 *	     OFString+NSObject.h ObjFWBridge/OFString+NSObject.h
 *
 * @brief Support for bridging OFStrings to NSStrings.
 *
 * Unfortunately, they need to be copied, as NSString is not capable of
 * handling UCS-4 properly (a character of NSString is only 2 bytes, while a
 * character of OFString is 4).
 */
@interface OFString (NSObject) <OFBridging>
@property (readonly, nonatomic) NSString *NSObject;
@end

OF_ASSUME_NONNULL_END
