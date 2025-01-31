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
#import "OFDictionary.h"

OF_ASSUME_NONNULL_BEGIN

/** @file */

#ifdef OF_HAVE_FILES
# if defined(OF_HAVE_CHMOD) && !defined(OF_AMIGAOS)
#  define OF_FILE_MANAGER_SUPPORTS_PERMISSIONS
# endif
# if defined(OF_HAVE_CHOWN) && !defined(OF_AMIGAOS)
#  define OF_FILE_MANAGER_SUPPORTS_OWNER
# endif
# if (defined(OF_HAVE_LINK) && !defined(OF_AMIGAOS)) || defined(OF_WINDOWS)
#  define OF_FILE_MANAGER_SUPPORTS_LINKS
# endif
# if (defined(OF_HAVE_SYMLINK) && !defined(OF_AMIGAOS)) || defined(OF_WINDOWS)
#  define OF_FILE_MANAGER_SUPPORTS_SYMLINKS
# endif
#endif

@class OFArray OF_GENERIC(ObjectType);
@class OFConstantString;
@class OFDate;
@class OFString;
@class OFURL;

/**
 * @brief A key for a file attribute in the file attributes dictionary.
 *
 * Possible keys for file URLs are:
 *
 *  * @ref OFFileSize
 *  * @ref OFFileType
 *  * @ref OFFilePOSIXPermissions
 *  * @ref OFFileOwnerAccountID
 *  * @ref OFFileGroupOwnerAccountID
 *  * @ref OFFileOwnerAccountName
 *  * @ref OFFileGroupOwnerAccountName
 *  * @ref OFFileLastAccessDate
 *  * @ref OFFileModificationDate
 *  * @ref OFFileStatusChangeDate
 *  * @ref OFFileCreationDate
 *  * @ref OFFileSymbolicLinkDestination
 *
 * Other URL schemes might not have all keys and might have keys not listed.
 */
typedef OFConstantString *OFFileAttributeKey;

/**
 * @brief The type of a file.
 *
 * Possibles values for file URLs are:
 *
 *  * @ref OFFileTypeRegular
 *  * @ref OFFileTypeDirectory
 *  * @ref OFFileTypeSymbolicLink
 *  * @ref OFFileTypeFIFO
 *  * @ref OFFileTypeCharacterSpecial
 *  * @ref OFFileTypeBlockSpecial
 *  * @ref OFFileTypeSocket
 *  * @ref OFFileTypeUnknown
 *
 * Other URL schemes might not have all types and might have types not listed.
 */
typedef OFConstantString *OFFileAttributeType;

/**
 * @brief A dictionary mapping keys of type @ref OFFileAttributeKey to their
 *	  attribute values.
 */
typedef OFDictionary OF_GENERIC(OFFileAttributeKey, id) *OFFileAttributes;

/**
 * @brief A mutable dictionary mapping keys of type @ref OFFileAttributeKey to
 *	  their attribute values.
 */
typedef OFMutableDictionary OF_GENERIC(OFFileAttributeKey, id)
    *OFMutableFileAttributes;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief The size of the file as an @ref OFNumber.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileSize.
 */
extern const OFFileAttributeKey OFFileSize;

/**
 * @brief The type of the file.
 *
 * The corresponding value is of type @ref OFFileAttributeType.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileType.
 */
extern const OFFileAttributeKey OFFileType;

/**
 * @brief The POSIX permissions of the file as an @ref OFNumber.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#filePOSIXPermissions.
 */
extern const OFFileAttributeKey OFFilePOSIXPermissions;

/**
 * @brief The account ID of the owner of the file as an @ref OFNumber.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileOwnerAccountID.
 */
extern const OFFileAttributeKey OFFileOwnerAccountID;

/**
 * @brief The account ID of the group owner of the file as an @ref OFNumber.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileGroupOwnerAccountID.
 */
extern const OFFileAttributeKey OFFileGroupOwnerAccountID;

/**
 * @brief The account name of the owner of the file as an OFString.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileOwnerAccountName.
 */
extern const OFFileAttributeKey OFFileOwnerAccountName;

/**
 * @brief The account name of the group owner of the file as an OFString.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileGroupOwnerAccountName.
 */
extern const OFFileAttributeKey OFFileGroupOwnerAccountName;

/**
 * @brief The last access date of the file as an @ref OFDate.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileLastAccessDate.
 */
extern const OFFileAttributeKey OFFileLastAccessDate;

/**
 * @brief The last modification date of the file as an @ref OFDate.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileModificationDate.
 */
extern const OFFileAttributeKey OFFileModificationDate;

/**
 * @brief The last status change date of the file as an @ref OFDate.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileStatusChangeDate.
 */
extern const OFFileAttributeKey OFFileStatusChangeDate;

/**
 * @brief The creation date of the file as an @ref OFDate.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileCreationDate.
 */
extern const OFFileAttributeKey OFFileCreationDate;

/**
 * @brief The destination of a symbolic link as an OFString.
 *
 * For convenience, a category on @ref OFDictionary is provided to access this
 * via @ref OFDictionary#fileSymbolicLinkDestination.
 */
extern const OFFileAttributeKey OFFileSymbolicLinkDestination;

/**
 * @brief A regular file.
 */
extern const OFFileAttributeType OFFileTypeRegular;

/**
 * @brief A directory.
 */
extern const OFFileAttributeType OFFileTypeDirectory;

/**
 * @brief A symbolic link.
 */
extern const OFFileAttributeType OFFileTypeSymbolicLink;

/**
 * @brief A FIFO.
 */
extern const OFFileAttributeType OFFileTypeFIFO;

/**
 * @brief A character special file.
 */
extern const OFFileAttributeType OFFileTypeCharacterSpecial;

/**
 * @brief A block special file.
 */
extern const OFFileAttributeType OFFileTypeBlockSpecial;

/**
 * @brief A socket.
 */
extern const OFFileAttributeType OFFileTypeSocket;

/**
 * @brief An unknown file type.
 *
 * This is different from not having an @ref OFFileType at all in that it means
 * that retrieving file types is supported, but the particular file type is
 * unknown.
 */
extern const OFFileAttributeType OFFileTypeUnknown;
#ifdef __cplusplus
}
#endif

/**
 * @class OFFileManager OFFileManager.h ObjFW/OFFileManager.h
 *
 * @brief A class which provides management for files, e.g. reading contents of
 *	  directories, deleting files, renaming files, etc.
 */
#ifndef OF_FILE_MANAGER_M
OF_SUBCLASSING_RESTRICTED
#endif
@interface OFFileManager: OFObject
#ifdef OF_HAVE_CLASS_PROPERTIES
@property (class, readonly, nonatomic) OFFileManager *defaultManager;
#endif

#ifdef OF_HAVE_FILES
/**
 * @brief The path of the current working directory.
 */
@property (readonly, nonatomic) OFString *currentDirectoryPath;

/**
 * @brief The URL of the current working directory.
 */
@property (readonly, nonatomic) OFURL *currentDirectoryURL;
#endif

/**
 * @brief Returns the default file manager.
 */
+ (OFFileManager *)defaultManager;

#ifdef OF_HAVE_FILES
/**
 * @brief Returns the attributes for the item at the specified path.
 *
 * @param path The path to return the attributes for
 * @return A dictionary of attributes for the specified path, with the keys of
 *	   type @ref OFFileAttributeKey
 */
- (OFFileAttributes)attributesOfItemAtPath: (OFString *)path;
#endif

/**
 * @brief Returns the attributes for the item at the specified URL.
 *
 * @param URL The URL to return the attributes for
 * @return A dictionary of attributes for the specified URL, with the keys of
 *	   type @ref OFFileAttributeKey
 */
- (OFFileAttributes)attributesOfItemAtURL: (OFURL *)URL;

#ifdef OF_HAVE_FILES
/**
 * @brief Sets the attributes for the item at the specified path.
 *
 * All attributes not part of the dictionary are left unchanged.
 *
 * @param attributes The attributes to set for the specified path
 * @param path The path of the item to set the attributes for
 */
- (void)setAttributes: (OFFileAttributes)attributes
	 ofItemAtPath: (OFString *)path;
#endif

/**
 * @brief Sets the attributes for the item at the specified URL.
 *
 * All attributes not part of the dictionary are left unchanged.
 *
 * @param attributes The attributes to set for the specified URL
 * @param URL The URL of the item to set the attributes for
 */
- (void)setAttributes: (OFFileAttributes)attributes ofItemAtURL: (OFURL *)URL;

#ifdef OF_HAVE_FILES
/**
 * @brief Checks whether a file exists at the specified path.
 *
 * @param path The path to check
 * @return A boolean whether there is a file at the specified path
 */
- (bool)fileExistsAtPath: (OFString *)path;
#endif

/**
 * @brief Checks whether a file exists at the specified URL.
 *
 * @param URL The URL to check
 * @return A boolean whether there is a file at the specified URL
 */
- (bool)fileExistsAtURL: (OFURL *)URL;

#ifdef OF_HAVE_FILES
/**
 * @brief Checks whether a directory exists at the specified path.
 *
 * @param path The path to check
 * @return A boolean whether there is a directory at the specified path
 */
- (bool)directoryExistsAtPath: (OFString *)path;
#endif

/**
 * @brief Checks whether a directory exists at the specified URL.
 *
 * @param URL The URL to check
 * @return A boolean whether there is a directory at the specified URL
 */
- (bool)directoryExistsAtURL: (OFURL *)URL;

#ifdef OF_HAVE_FILES
/**
 * @brief Creates a directory at the specified path.
 *
 * @param path The path of the directory to create
 */
- (void)createDirectoryAtPath: (OFString *)path;

/**
 * @brief Creates a directory at the specified path.
 *
 * @param path The path of the directory to create
 * @param createParents Whether to create the parents of the directory
 */
- (void)createDirectoryAtPath: (OFString *)path
		createParents: (bool)createParents;
#endif

/**
 * @brief Creates a directory at the specified URL.
 *
 * @param URL The URL of the directory to create
 */
- (void)createDirectoryAtURL: (OFURL *)URL;

/**
 * @brief Creates a directory at the specified URL.
 *
 * @param URL The URL of the directory to create
 * @param createParents Whether to create the parents of the directory
 */
- (void)createDirectoryAtURL: (OFURL *)URL createParents: (bool)createParents;

#ifdef OF_HAVE_FILES
/**
 * @brief Returns an array with the items in the specified directory.
 *
 * @note `.` and `..` are not part of the returned array.
 *
 * @param path The path to the directory whose items should be returned
 * @return An array of OFString with the items in the specified directory
 */
- (OFArray OF_GENERIC(OFString *) *)contentsOfDirectoryAtPath: (OFString *)path;
#endif

/**
 * @brief Returns an array with the URLs of the items in the specified
 *	  directory.
 *
 * @note `.` and `..` are not part of the returned array.
 *
 * @param URL The URL to the directory whose items should be returned
 * @return An array with the URLs of the items in the specified directory
 */
- (OFArray OF_GENERIC(OFURL *) *)contentsOfDirectoryAtURL: (OFURL *)URL;

#ifdef OF_HAVE_FILES
/**
 * @brief Returns an array with all subpaths of the specified directory.
 *
 * @note `.` and `..` (of the directory itself or any subdirectory) are not
 * part of the returned array.
 *
 * @param path The path to the directory whose subpaths should be returned
 * @return An array of OFString with the subpaths of the specified directory
 */
- (OFArray OF_GENERIC(OFString *) *)subpathsOfDirectoryAtPath: (OFString *)path;

/**
 * @brief Changes the current working directory.
 *
 * @param path The new directory to change to
 */
- (void)changeCurrentDirectoryPath: (OFString *)path;

/**
 * @brief Changes the current working directory.
 *
 * @param URL The new directory to change to
 */
- (void)changeCurrentDirectoryURL: (OFURL *)URL;

/**
 * @brief Copies a file, directory or symbolic link (if supported by the OS).
 *
 * The destination path must be a full path, which means it must include the
 * name of the item.
 *
 * If an item already exists, the copy operation fails. This is also the case
 * if a directory is copied and an item already exists in the destination
 * directory.
 *
 * @param source The file, directory or symbolic link to copy
 * @param destination The destination path
 */
- (void)copyItemAtPath: (OFString *)source toPath: (OFString *)destination;
#endif

/**
 * @brief Copies a file, directory or symbolic link (if supported by the OS).
 *
 * The destination URL must have a full path, which means it must include the
 * name of the item.
 *
 * If an item already exists, the copy operation fails. This is also the case
 * if a directory is copied and an item already exists in the destination
 * directory.
 *
 * @param source The file, directory or symbolic link to copy
 * @param destination The destination URL
 */
- (void)copyItemAtURL: (OFURL *)source toURL: (OFURL *)destination;

#ifdef OF_HAVE_FILES
/**
 * @brief Moves an item.
 *
 * The destination path must be a full path, which means it must include the
 * name of the item.
 *
 * If the destination is on a different logical device, the source will be
 * copied to the destination using @ref copyItemAtPath:toPath: and the source
 * removed using @ref removeItemAtPath:.
 *
 * @param source The item to rename
 * @param destination The new name for the item
 */
- (void)moveItemAtPath: (OFString *)source toPath: (OFString *)destination;
#endif

/**
 * @brief Moves an item.
 *
 * The destination URL must have a full path, which means it must include the
 * name of the item.
 *
 * If the destination is on a different logical device or uses a different
 * scheme, the source will be copied to the destination using
 * @ref copyItemAtURL:toURL: and the source removed using @ref removeItemAtURL:.
 *
 * @param source The item to rename
 * @param destination The new name for the item
 */
- (void)moveItemAtURL: (OFURL *)source toURL: (OFURL *)destination;

#ifdef OF_HAVE_FILES
/**
 * @brief Removes the item at the specified path.
 *
 * If the item at the specified path is a directory, it is removed recursively.
 *
 * @param path The path to the item which should be removed
 */
- (void)removeItemAtPath: (OFString *)path;
#endif

/**
 * @brief Removes the item at the specified URL.
 *
 * If the item at the specified URL is a directory, it is removed recursively.
 *
 * @param URL The URL to the item which should be removed
 */
- (void)removeItemAtURL: (OFURL *)URL;

#ifdef OF_FILE_MANAGER_SUPPORTS_LINKS
/**
 * @brief Creates a hard link for the specified item.
 *
 * The destination path must be a full path, which means it must include the
 * name of the item.
 *
 * This method is not available on some systems.
 *
 * @param source The path to the item for which a link should be created
 * @param destination The path to the item which should link to the source
 */
- (void)linkItemAtPath: (OFString *)source toPath: (OFString *)destination;
#endif

/**
 * @brief Creates a hard link for the specified item.
 *
 * The destination URL must have a full path, which means it must include the
 * name of the item.
 *
 * This method is not available for all URLs.
 *
 * @param source The URL to the item for which a link should be created
 * @param destination The URL to the item which should link to the source
 */
- (void)linkItemAtURL: (OFURL *)source toURL: (OFURL *)destination;

#ifdef OF_FILE_MANAGER_SUPPORTS_SYMLINKS
/**
 * @brief Creates a symbolic link for an item.
 *
 * The destination path must be a full path, which means it must include the
 * name of the item.
 *
 * This method is not available on some systems.
 *
 * @note On Windows, this requires at least Windows Vista and administrator
 *	 privileges!
 *
 * @param path The path to the item which should symbolically link to the target
 * @param target The target of the symbolic link
 */
- (void)createSymbolicLinkAtPath: (OFString *)path
	     withDestinationPath: (OFString *)target;
#endif

/**
 * @brief Creates a symbolic link for an item.
 *
 * The destination uRL must have a full path, which means it must include the
 * name of the item.
 *
 * This method is not available for all URLs.
 *
 * @note On Windows, this requires at least Windows Vista and administrator
 *	 privileges!
 *
 * @param URL The URL to the item which should symbolically link to the target
 * @param target The target of the symbolic link
 */
- (void)createSymbolicLinkAtURL: (OFURL *)URL
	    withDestinationPath: (OFString *)target;
@end

@interface OFDictionary (FileAttributes)
/**
 * @brief The @ref OFFileSize key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) unsigned long long fileSize;

/**
 * @brief The @ref OFFileType key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFFileAttributeType fileType;

/**
 * @brief The @ref OFFilePOSIXPermissions key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) unsigned long filePOSIXPermissions;

/**
 * @brief The @ref OFFileOwnerAccountID key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) unsigned long fileOwnerAccountID;

/**
 * @brief The @ref OFFileGroupOwnerAccountID key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) unsigned long fileGroupOwnerAccountID;

/**
 * @brief The @ref OFFileOwnerAccountName key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFString *fileOwnerAccountName;

/**
 * @brief The @ref OFFileGroupOwnerAccountName key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFString *fileGroupOwnerAccountName;

/**
 * @brief The @ref OFFileLastAccessDate key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFDate *fileLastAccessDate;

/**
 * @brief The @ref OFFileModificationDate key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFDate *fileModificationDate;

/**
 * @brief The @ref OFFileStatusChangeDate key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFDate *fileStatusChangeDate;

/**
 * @brief The @ref OFFileCreationDate key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFDate *fileCreationDate;

/**
 * @brief The @ref OFFileSymbolicLinkDestination key from the dictionary.
 *
 * Raises an @ref OFUndefinedKeyException if the key is missing.
 */
@property (readonly, nonatomic) OFString *fileSymbolicLinkDestination;
@end

OF_ASSUME_NONNULL_END
