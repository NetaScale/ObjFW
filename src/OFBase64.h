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

#ifndef __STDC_LIMIT_MACROS
# define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif

#import "macros.h"

OF_ASSUME_NONNULL_BEGIN

@class OFString;
@class OFMutableData;

#ifdef __cplusplus
extern "C" {
#endif
extern OFString *OFBase64Encode(const void *, size_t);
extern bool OFBase64Decode(OFMutableData *, const char *, size_t);
#ifdef __cplusplus
}
#endif

OF_ASSUME_NONNULL_END
