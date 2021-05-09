/*
 * Copyright (c) 2008-2021 Jonathan Schleifer <js@nil.im>
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

/* This file is automatically generated from library.xml */

#import "amiga-library.h"
#import "OFObject.h"
#import "OFStdIOStream.h"
#import "OFApplication.h"
#import "OFBlock.h"
#import "OFDNSResourceRecord.h"
#import "OFHTTPRequest.h"
#import "OFHTTPResponse.h"
#import "OFList.h"
#import "OFMethodSignature.h"
#import "OFOnce.h"
#import "OFPBKDF2.h"
#import "OFScrypt.h"
#import "OFSocket.h"
#import "OFStrPTime.h"
#import "OFString.h"
#import "OFZIPArchiveEntry.h"

#ifdef OF_AMIGAOS_M68K
# define PPC_PARAMS(...) (void)
# define M68K_ARG(type, name, reg)		\
	register type reg##name __asm__(#reg);	\
	type name = reg##name;
#else
# define PPC_PARAMS(...) (__VA_ARGS__)
# define M68K_ARG(...)
#endif

extern bool glue_OFInit PPC_PARAMS(unsigned int version, struct OFLibC *_Nonnull libc, FILE *_Nonnull *_Nonnull sF);
extern void *_Nullable glue_OFAllocMemory PPC_PARAMS(size_t count, size_t size);
extern void *_Nullable glue_OFAllocZeroedMemory PPC_PARAMS(size_t count, size_t size);
extern void *_Nullable glue_OFResizeMemory PPC_PARAMS(void *_Nullable pointer, size_t count, size_t size);
extern void glue_OFFreeMemory PPC_PARAMS(void *_Nullable pointer);
extern void glue_OFHashInit PPC_PARAMS(unsigned long *_Nonnull hash);
extern uint16_t glue_OFRandom16(void);
extern uint32_t glue_OFRandom32(void);
extern uint64_t glue_OFRandom64(void);
extern unsigned long *_Nonnull glue_OFHashSeedRef(void);
extern OFStdIOStream *_Nonnull *_Nullable glue_OFStdInRef(void);
extern OFStdIOStream *_Nonnull *_Nullable glue_OFStdOutRef(void);
extern OFStdIOStream *_Nonnull *_Nullable glue_OFStdErrRef(void);
extern void glue_OFLogV PPC_PARAMS(OFConstantString *format, va_list arguments);
extern int glue_OFApplicationMain PPC_PARAMS(int *_Nonnull argc, char *_Nullable *_Nonnull *_Nonnull argv, id <OFApplicationDelegate> delegate);
extern void *_Nullable glue__Block_copy PPC_PARAMS(const void *_Nullable block);
extern void glue__Block_release PPC_PARAMS(const void *_Nullable block);
extern OFString *_Nonnull glue_OFDNSClassName PPC_PARAMS(OFDNSClass DNSClass);
extern OFString *_Nonnull glue_OFDNSRecordTypeName PPC_PARAMS(OFDNSRecordType recordType);
extern OFDNSClass glue_OFDNSClassParseName PPC_PARAMS(OFString *_Nonnull string);
extern OFDNSRecordType glue_OFDNSRecordTypeParseName PPC_PARAMS(OFString *_Nonnull string);
extern const char *_Nullable glue_OFHTTPRequestMethodName PPC_PARAMS(OFHTTPRequestMethod method);
extern OFHTTPRequestMethod glue_OFHTTPRequestMethodParseName PPC_PARAMS(OFString *string);
extern OFString *_Nonnull glue_OFHTTPStatusCodeString PPC_PARAMS(short code);
extern OFListItem _Nullable glue_OFListItemNext PPC_PARAMS(OFListItem _Nonnull listItem);
extern OFListItem _Nullable glue_OFListItemPrevious PPC_PARAMS(OFListItem _Nonnull listItem);
extern id _Nonnull glue_OFListItemObject PPC_PARAMS(OFListItem _Nonnull listItem);
extern size_t glue_OFSizeOfTypeEncoding PPC_PARAMS(const char *type);
extern size_t glue_OFAlignmentOfTypeEncoding PPC_PARAMS(const char *type);
extern void glue_OFOnce PPC_PARAMS(OFOnceControl *_Nonnull control, OFOnceFunction _Nonnull func);
extern void glue_OFPBKDF2 PPC_PARAMS(OFPBKDF2Parameters parameters);
extern void glue_OFScrypt PPC_PARAMS(OFScryptParameters parameters);
extern void glue_OFSalsa20_8Core PPC_PARAMS(uint32_t *_Nonnull buffer);
extern void glue_OFScryptBlockMix PPC_PARAMS(uint32_t *_Nonnull output, const uint32_t *_Nonnull input, size_t blockSize);
extern void glue_OFScryptROMix PPC_PARAMS(uint32_t *buffer, size_t blockSize, size_t costFactor, uint32_t *tmp);
extern OFSocketAddress glue_OFSocketAddressParseIP PPC_PARAMS(OFString *IP, uint16_t port);
extern OFSocketAddress glue_OFSocketAddressParseIPv4 PPC_PARAMS(OFString *IP, uint16_t port);
extern OFSocketAddress glue_OFSocketAddressParseIPv6 PPC_PARAMS(OFString *IP, uint16_t port);
extern OFSocketAddress glue_OFSocketAddressMakeIPX PPC_PARAMS(const unsigned char *_Nonnull node, uint32_t network, uint16_t port);
extern bool glue_OFSocketAddressEqual PPC_PARAMS(const OFSocketAddress *_Nonnull address1, const OFSocketAddress *_Nonnull address2);
extern unsigned long glue_OFSocketAddressHash PPC_PARAMS(const OFSocketAddress *_Nonnull address);
extern OFString *_Nonnull glue_OFSocketAddressString PPC_PARAMS(const OFSocketAddress *_Nonnull address);
extern void glue_OFSocketAddressSetPort PPC_PARAMS(OFSocketAddress *_Nonnull address, uint16_t port);
extern uint16_t glue_OFSocketAddressPort PPC_PARAMS(const OFSocketAddress *_Nonnull address);
extern void glue_OFSocketAddressSetIPXNetwork PPC_PARAMS(OFSocketAddress *_Nonnull address, uint32_t network);
extern uint32_t glue_OFSocketAddressIPXNetwork PPC_PARAMS(const OFSocketAddress *_Nonnull address);
extern void glue_OFSocketAddressSetIPXNode PPC_PARAMS(OFSocketAddress *_Nonnull address, const unsigned char *_Nonnull node);
extern void glue_OFSocketAddressIPXNode PPC_PARAMS(const OFSocketAddress *_Nonnull address, unsigned char *_Nonnull node);
extern const char *_Nullable glue_OFStrPTime PPC_PARAMS(const char *buffer, const char *format, struct tm *tm, int16_t *_Nullable tz);
extern OFStringEncoding glue_OFStringEncodingParseName PPC_PARAMS(OFString *string);
extern OFString *_Nullable glue_OFStringEncodingName PPC_PARAMS(OFStringEncoding encoding);
extern size_t glue_OFUTF16StringLength PPC_PARAMS(const OFChar16 *string);
extern size_t glue_OFUTF32StringLength PPC_PARAMS(const OFChar32 *string);
extern OFString *_Nonnull glue_OFZIPArchiveEntryVersionToString PPC_PARAMS(uint16_t version);
extern OFString *_Nonnull glue_OFZIPArchiveEntryCompressionMethodName PPC_PARAMS(OFZIPArchiveEntryCompressionMethod compressionMethod);
extern size_t glue_OFZIPArchiveEntryExtraFieldFind PPC_PARAMS(OFData *extraField, OFZIPArchiveEntryExtraFieldTag tag, uint16_t *size);
