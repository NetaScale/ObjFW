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

#import "OFObject.h"
#import "OFString.h"

#include <windows.h>

OF_ASSUME_NONNULL_BEGIN

@class OFData;

/**
 * @class OFWindowsRegistryKey \
 *	  OFWindowsRegistryKey.h ObjFW/OFWindowsRegistryKey.h
 */
OF_SUBCLASSING_RESTRICTED
@interface OFWindowsRegistryKey: OFObject
{
	HKEY _hKey;
	bool _close;
}

/**
 * @brief Returns the OFWindowsRegistryKey for the HKEY_CLASSES_ROOT key.
 *
 * @return The OFWindowsRegistryKey for the HKEY_CLASSES_ROOT key
 */
+ (instancetype)classesRootKey;

/**
 * @brief Returns the OFWindowsRegistryKey for the HKEY_CURRENT_CONFIG key.
 *
 * @return The OFWindowsRegistryKey for the HKEY_CURRENT_CONFIG key
 */
+ (instancetype)currentConfigKey;

/**
 * @brief Returns the OFWindowsRegistryKey for the HKEY_CURRENT_USER key.
 *
 * @return The OFWindowsRegistryKey for the HKEY_CURRENT_USER key
 */
+ (instancetype)currentUserKey;

/**
 * @brief Returns the OFWindowsRegistryKey for the HKEY_LOCAL_MACHINE key.
 *
 * @return The OFWindowsRegistryKey for the HKEY_LOCAL_MACHINE key
 */
+ (instancetype)localMachineKey;

/**
 * @brief Returns the OFWindowsRegistryKey for the HKEY_USERS key.
 *
 * @return The OFWindowsRegistryKey for the HKEY_USERS key
 */
+ (instancetype)usersKey;

- (instancetype)init OF_UNAVAILABLE;

/**
 * @brief Opens the subkey at the specified path.
 *
 * @param path The path of the subkey to open
 * @param securityAndAccessRights Please refer to the `RegOpenKeyEx()`
 *				  documentation for `samDesired`
 * @return The subkey with the specified path, or nil if it does not exist
 */
- (nullable OFWindowsRegistryKey *)
	   openSubkeyAtPath: (OFString *)path
    securityAndAccessRights: (REGSAM)securityAndAccessRights;

/**
 * @brief Opens the subkey at the specified path.
 *
 * @param path The path of the subkey to open
 * @param options Please refer to the `RegOpenKeyEx()` documentation for
 *		  `ulOptions`. Usually 0.
 * @param securityAndAccessRights Please refer to the `RegOpenKeyEx()`
 *				  documentation for `samDesired`
 * @return The subkey with the specified path, or nil if it does not exist
 */
- (nullable OFWindowsRegistryKey *)
	   openSubkeyAtPath: (OFString *)path
		    options: (DWORD)options
    securityAndAccessRights: (REGSAM)securityAndAccessRights;

/**
 * @brief Creates a subkey at the specified path or opens it if it already
 *	  exists.
 *
 * @param path The path of the subkey to create
 * @param securityAndAccessRights Please refer to the `RegCreateKeyEx()`
 *				  documentation for `samDesired`
 * @return The subkey with the specified path
 */
- (OFWindowsRegistryKey *)createSubkeyAtPath: (OFString *)path
		     securityAndAccessRights: (REGSAM)securityAndAccessRights;

/**
 * @brief Creates a subkey at the specified path or opens it if it already
 *	  exists.
 *
 * @param path The path of the subkey to create
 * @param options Please refer to the `RegCreateKeyEx()` documentation.
 *		  Usually 0.
 * @param securityAndAccessRights Please refer to the `RegCreateKeyEx()`
 *				  documentation for `samDesired`
 * @param securityAttributes Please refer to the `RegCreateKeyEx()`
 *			     documentation for `lpSecurityAttributes`. Usually
 *			     NULL.
 * @param disposition Whether the key was created or already existed. Please
 *		      refer to the `RegCreateKeyEx()` documentation for
 *		      `lpdwDisposition`.
 * @return The subkey with the specified path
 */
- (OFWindowsRegistryKey *)
	 createSubkeyAtPath: (OFString *)path
		    options: (DWORD)options
    securityAndAccessRights: (REGSAM)securityAndAccessRights
	 securityAttributes: (nullable SECURITY_ATTRIBUTES *)securityAttributes
		disposition: (nullable DWORD *)disposition;

/**
 * @brief Returns the data for the specified value at the specified path.
 *
 * @param name The name of the value to return
 * @param type A pointer to store the type of the value, or NULL
 * @return The data for the specified value
 */
- (nullable OFData *)dataForValueNamed: (nullable OFString *)name
				  type: (nullable DWORD *)type;

/**
 * @brief Sets the data for the specified value.
 *
 * @param data The data to set the value to
 * @param name The name of the value to set
 * @param type The type for the value
 */
- (void)setData: (nullable OFData *)data
  forValueNamed: (nullable OFString *)name
	   type: (DWORD)type;

/**
 * @brief Returns the string for the specified value at the specified path.
 *
 * @param name The name of the value to return
 * @return The string for the specified value
 */
- (nullable OFString *)stringForValueNamed: (nullable OFString *)name;

/**
 * @brief Returns the string for the specified value at the specified path.
 *
 * @param name The name of the value to return
 * @param type A pointer to store the type of the value, or NULL
 * @return The string for the specified value
 */
- (nullable OFString *)stringForValueNamed: (nullable OFString *)name
				      type: (nullable DWORD *)type;

/**
 * @brief Sets the string for the specified value.
 *
 * @param string The string to set the value to
 * @param name The name of the value to set
 */
- (void)setString: (nullable OFString *)string
    forValueNamed: (nullable OFString *)name;

/**
 * @brief Sets the string for the specified value.
 *
 * @param string The string to set the value to
 * @param name The name of the value to set
 * @param type The type for the value
 */
- (void)setString: (nullable OFString *)string
    forValueNamed: (nullable OFString *)name
	     type: (DWORD)type;

/**
 * @brief Deletes the specified value.
 *
 * @param name The value to delete
 */
- (void)deleteValueNamed: (nullable OFString *)name;

/**
 * @brief Deletes the specified subkey.
 *
 * @param subkeyPath The path of the subkey to delete
 */
- (void)deleteSubkeyAtPath: (OFString *)subkeyPath;
@end

OF_ASSUME_NONNULL_END
