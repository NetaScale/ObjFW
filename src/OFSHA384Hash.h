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

#import "OFSHA384Or512Hash.h"

OF_ASSUME_NONNULL_BEGIN

/**
 * @class OFSHA384Hash OFSHA384Hash.h ObjFW/OFSHA384Hash.h
 *
 * @brief A class which provides methods to create an SHA-384 hash.
 */
OF_SUBCLASSING_RESTRICTED
@interface OFSHA384Hash: OFSHA384Or512Hash
@end

OF_ASSUME_NONNULL_END
