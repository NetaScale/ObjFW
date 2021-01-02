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

#include "config.h"

#include <string.h>

#import "TestsAppDelegate.h"

static OFString *module = @"scrypt";
/* Test vectors form RFC 7914 */
static const unsigned char salsa20Input[64] = {
	0x7E, 0x87, 0x9A, 0x21, 0x4F, 0x3E, 0xC9, 0x86, 0x7C, 0xA9, 0x40, 0xE6,
	0x41, 0x71, 0x8F, 0x26, 0xBA, 0xEE, 0x55, 0x5B, 0x8C, 0x61, 0xC1, 0xB5,
	0x0D, 0xF8, 0x46, 0x11, 0x6D, 0xCD, 0x3B, 0x1D, 0xEE, 0x24, 0xF3, 0x19,
	0xDF, 0x9B, 0x3D, 0x85, 0x14, 0x12, 0x1E, 0x4B, 0x5A, 0xC5, 0xAA, 0x32,
	0x76, 0x02, 0x1D, 0x29, 0x09, 0xC7, 0x48, 0x29, 0xED, 0xEB, 0xC6, 0x8D,
	0xB8, 0xB8, 0xC2, 0x5E
};
static const unsigned char salsa20Output[64] = {
	0xA4, 0x1F, 0x85, 0x9C, 0x66, 0x08, 0xCC, 0x99, 0x3B, 0x81, 0xCA, 0xCB,
	0x02, 0x0C, 0xEF, 0x05, 0x04, 0x4B, 0x21, 0x81, 0xA2, 0xFD, 0x33, 0x7D,
	0xFD, 0x7B, 0x1C, 0x63, 0x96, 0x68, 0x2F, 0x29, 0xB4, 0x39, 0x31, 0x68,
	0xE3, 0xC9, 0xE6, 0xBC, 0xFE, 0x6B, 0xC5, 0xB7, 0xA0, 0x6D, 0x96, 0xBA,
	0xE4, 0x24, 0xCC, 0x10, 0x2C, 0x91, 0x74, 0x5C, 0x24, 0xAD, 0x67, 0x3D,
	0xC7, 0x61, 0x8F, 0x81
};
static const union {
	unsigned char uc[128];
	uint32_t u32[32];
} blockMixInput = { .uc = {
	0xF7, 0xCE, 0x0B, 0x65, 0x3D, 0x2D, 0x72, 0xA4, 0x10, 0x8C, 0xF5, 0xAB,
	0xE9, 0x12, 0xFF, 0xDD, 0x77, 0x76, 0x16, 0xDB, 0xBB, 0x27, 0xA7, 0x0E,
	0x82, 0x04, 0xF3, 0xAE, 0x2D, 0x0F, 0x6F, 0xAD, 0x89, 0xF6, 0x8F, 0x48,
	0x11, 0xD1, 0xE8, 0x7B, 0xCC, 0x3B, 0xD7, 0x40, 0x0A, 0x9F, 0xFD, 0x29,
	0x09, 0x4F, 0x01, 0x84, 0x63, 0x95, 0x74, 0xF3, 0x9A, 0xE5, 0xA1, 0x31,
	0x52, 0x17, 0xBC, 0xD7,
	0x89, 0x49, 0x91, 0x44, 0x72, 0x13, 0xBB, 0x22, 0x6C, 0x25, 0xB5, 0x4D,
	0xA8, 0x63, 0x70, 0xFB, 0xCD, 0x98, 0x43, 0x80, 0x37, 0x46, 0x66, 0xBB,
	0x8F, 0xFC, 0xB5, 0xBF, 0x40, 0xC2, 0x54, 0xB0, 0x67, 0xD2, 0x7C, 0x51,
	0xCE, 0x4A, 0xD5, 0xFE, 0xD8, 0x29, 0xC9, 0x0B, 0x50, 0x5A, 0x57, 0x1B,
	0x7F, 0x4D, 0x1C, 0xAD, 0x6A, 0x52, 0x3C, 0xDA, 0x77, 0x0E, 0x67, 0xBC,
	0xEA, 0xAF, 0x7E, 0x89
}};
static const unsigned char blockMixOutput[128] = {
	0xA4, 0x1F, 0x85, 0x9C, 0x66, 0x08, 0xCC, 0x99, 0x3B, 0x81, 0xCA, 0xCB,
	0x02, 0x0C, 0xEF, 0x05, 0x04, 0x4B, 0x21, 0x81, 0xA2, 0xFD, 0x33, 0x7D,
	0xFD, 0x7B, 0x1C, 0x63, 0x96, 0x68, 0x2F, 0x29, 0xB4, 0x39, 0x31, 0x68,
	0xE3, 0xC9, 0xE6, 0xBC, 0xFE, 0x6B, 0xC5, 0xB7, 0xA0, 0x6D, 0x96, 0xBA,
	0xE4, 0x24, 0xCC, 0x10, 0x2C, 0x91, 0x74, 0x5C, 0x24, 0xAD, 0x67, 0x3D,
	0xC7, 0x61, 0x8F, 0x81,
	0x20, 0xED, 0xC9, 0x75, 0x32, 0x38, 0x81, 0xA8, 0x05, 0x40, 0xF6, 0x4C,
	0x16, 0x2D, 0xCD, 0x3C, 0x21, 0x07, 0x7C, 0xFE, 0x5F, 0x8D, 0x5F, 0xE2,
	0xB1, 0xA4, 0x16, 0x8F, 0x95, 0x36, 0x78, 0xB7, 0x7D, 0x3B, 0x3D, 0x80,
	0x3B, 0x60, 0xE4, 0xAB, 0x92, 0x09, 0x96, 0xE5, 0x9B, 0x4D, 0x53, 0xB6,
	0x5D, 0x2A, 0x22, 0x58, 0x77, 0xD5, 0xED, 0xF5, 0x84, 0x2C, 0xB9, 0xF1,
	0x4E, 0xEF, 0xE4, 0x25
};
static const unsigned char ROMixInput[128] = {
	0xF7, 0xCE, 0x0B, 0x65, 0x3D, 0x2D, 0x72, 0xA4, 0x10, 0x8C, 0xF5, 0xAB,
	0xE9, 0x12, 0xFF, 0xDD, 0x77, 0x76, 0x16, 0xDB, 0xBB, 0x27, 0xA7, 0x0E,
	0x82, 0x04, 0xF3, 0xAE, 0x2D, 0x0F, 0x6F, 0xAD, 0x89, 0xF6, 0x8F, 0x48,
	0x11, 0xD1, 0xE8, 0x7B, 0xCC, 0x3B, 0xD7, 0x40, 0x0A, 0x9F, 0xFD, 0x29,
	0x09, 0x4F, 0x01, 0x84, 0x63, 0x95, 0x74, 0xF3, 0x9A, 0xE5, 0xA1, 0x31,
	0x52, 0x17, 0xBC, 0xD7, 0x89, 0x49, 0x91, 0x44, 0x72, 0x13, 0xBB, 0x22,
	0x6C, 0x25, 0xB5, 0x4D, 0xA8, 0x63, 0x70, 0xFB, 0xCD, 0x98, 0x43, 0x80,
	0x37, 0x46, 0x66, 0xBB, 0x8F, 0xFC, 0xB5, 0xBF, 0x40, 0xC2, 0x54, 0xB0,
	0x67, 0xD2, 0x7C, 0x51, 0xCE, 0x4A, 0xD5, 0xFE, 0xD8, 0x29, 0xC9, 0x0B,
	0x50, 0x5A, 0x57, 0x1B, 0x7F, 0x4D, 0x1C, 0xAD, 0x6A, 0x52, 0x3C, 0xDA,
	0x77, 0x0E, 0x67, 0xBC, 0xEA, 0xAF, 0x7E, 0x89
};
static const unsigned char ROMixOutput[128] = {
	0x79, 0xCC, 0xC1, 0x93, 0x62, 0x9D, 0xEB, 0xCA, 0x04, 0x7F, 0x0B, 0x70,
	0x60, 0x4B, 0xF6, 0xB6, 0x2C, 0xE3, 0xDD, 0x4A, 0x96, 0x26, 0xE3, 0x55,
	0xFA, 0xFC, 0x61, 0x98, 0xE6, 0xEA, 0x2B, 0x46, 0xD5, 0x84, 0x13, 0x67,
	0x3B, 0x99, 0xB0, 0x29, 0xD6, 0x65, 0xC3, 0x57, 0x60, 0x1F, 0xB4, 0x26,
	0xA0, 0xB2, 0xF4, 0xBB, 0xA2, 0x00, 0xEE, 0x9F, 0x0A, 0x43, 0xD1, 0x9B,
	0x57, 0x1A, 0x9C, 0x71, 0xEF, 0x11, 0x42, 0xE6, 0x5D, 0x5A, 0x26, 0x6F,
	0xDD, 0xCA, 0x83, 0x2C, 0xE5, 0x9F, 0xAA, 0x7C, 0xAC, 0x0B, 0x9C, 0xF1,
	0xBE, 0x2B, 0xFF, 0xCA, 0x30, 0x0D, 0x01, 0xEE, 0x38, 0x76, 0x19, 0xC4,
	0xAE, 0x12, 0xFD, 0x44, 0x38, 0xF2, 0x03, 0xA0, 0xE4, 0xE1, 0xC4, 0x7E,
	0xC3, 0x14, 0x86, 0x1F, 0x4E, 0x90, 0x87, 0xCB, 0x33, 0x39, 0x6A, 0x68,
	0x73, 0xE8, 0xF9, 0xD2, 0x53, 0x9A, 0x4B, 0x8E
};
static const unsigned char testVector1[64] = {
	0x77, 0xD6, 0x57, 0x62, 0x38, 0x65, 0x7B, 0x20, 0x3B, 0x19, 0xCA, 0x42,
	0xC1, 0x8A, 0x04, 0x97, 0xF1, 0x6B, 0x48, 0x44, 0xE3, 0x07, 0x4A, 0xE8,
	0xDF, 0xDF, 0xFA, 0x3F, 0xED, 0xE2, 0x14, 0x42, 0xFC, 0xD0, 0x06, 0x9D,
	0xED, 0x09, 0x48, 0xF8, 0x32, 0x6A, 0x75, 0x3A, 0x0F, 0xC8, 0x1F, 0x17,
	0xE8, 0xD3, 0xE0, 0xFB, 0x2E, 0x0D, 0x36, 0x28, 0xCF, 0x35, 0xE2, 0x0C,
	0x38, 0xD1, 0x89, 0x06
};
static const unsigned char testVector2[64] = {
	0xFD, 0xBA, 0xBE, 0x1C, 0x9D, 0x34, 0x72, 0x00, 0x78, 0x56, 0xE7, 0x19,
	0x0D, 0x01, 0xE9, 0xFE, 0x7C, 0x6A, 0xD7, 0xCB, 0xC8, 0x23, 0x78, 0x30,
	0xE7, 0x73, 0x76, 0x63, 0x4B, 0x37, 0x31, 0x62, 0x2E, 0xAF, 0x30, 0xD9,
	0x2E, 0x22, 0xA3, 0x88, 0x6F, 0xF1, 0x09, 0x27, 0x9D, 0x98, 0x30, 0xDA,
	0xC7, 0x27, 0xAF, 0xB9, 0x4A, 0x83, 0xEE, 0x6D, 0x83, 0x60, 0xCB, 0xDF,
	0xA2, 0xCC, 0x06, 0x40
};
static const unsigned char testVector3[64] = {
	0x70, 0x23, 0xBD, 0xCB, 0x3A, 0xFD, 0x73, 0x48, 0x46, 0x1C, 0x06, 0xCD,
	0x81, 0xFD, 0x38, 0xEB, 0xFD, 0xA8, 0xFB, 0xBA, 0x90, 0x4F, 0x8E, 0x3E,
	0xA9, 0xB5, 0x43, 0xF6, 0x54, 0x5D, 0xA1, 0xF2, 0xD5, 0x43, 0x29, 0x55,
	0x61, 0x3F, 0x0F, 0xCF, 0x62, 0xD4, 0x97, 0x05, 0x24, 0x2A, 0x9A, 0xF9,
	0xE6, 0x1E, 0x85, 0xDC, 0x0D, 0x65, 0x1E, 0x40, 0xDF, 0xCF, 0x01, 0x7B,
	0x45, 0x57, 0x58, 0x87
};
/* The forth test vector is too expensive to include it in the tests. */
#if 0
static const unsigned char testVector4[64] = {
	0x21, 0x01, 0xCB, 0x9B, 0x6A, 0x51, 0x1A, 0xAE, 0xAD, 0xDB, 0xBE, 0x09,
	0xCF, 0x70, 0xF8, 0x81, 0xEC, 0x56, 0x8D, 0x57, 0x4A, 0x2F, 0xFD, 0x4D,
	0xAB, 0xE5, 0xEE, 0x98, 0x20, 0xAD, 0xAA, 0x47, 0x8E, 0x56, 0xFD, 0x8F,
	0x4B, 0xA5, 0xD0, 0x9F, 0xFA, 0x1C, 0x6D, 0x92, 0x7C, 0x40, 0xF4, 0xC3,
	0x37, 0x30, 0x40, 0x49, 0xE8, 0xA9, 0x52, 0xFB, 0xCB, 0xF4, 0x5C, 0x6F,
	0xA7, 0x7A, 0x41, 0xA4
};
#endif

@implementation TestsAppDelegate (ScryptTests)
- (void)scryptTests
{
	void *pool = objc_autoreleasePoolPush();
	uint32_t salsa20Buffer[16];
	uint32_t blockMixBuffer[32];
	uint32_t ROMixBuffer[32], ROMixTmp[17 * 32];
	unsigned char output[64];

	TEST(@"Salsa20/8 Core",
	    R(memcpy(salsa20Buffer, salsa20Input, 64)) &&
	    R(of_salsa20_8_core(salsa20Buffer)) &&
	    memcmp(salsa20Buffer, salsa20Output, 64) == 0)

	TEST(@"Block mix",
	    R(of_scrypt_block_mix(blockMixBuffer, blockMixInput.u32, 1)) &&
	    memcmp(blockMixBuffer, blockMixOutput, 128) == 0)

	TEST(@"ROMix",
	    R(memcpy(ROMixBuffer, ROMixInput, 128)) &&
	    R(of_scrypt_romix(ROMixBuffer, 1, 16, ROMixTmp)) &&
	    memcmp(ROMixBuffer, ROMixOutput, 128) == 0)

	TEST(@"scrypt test vector #1",
	    R(of_scrypt((of_scrypt_parameters_t){
		.blockSize             = 1,
		.costFactor            = 16,
		.parallelization       = 1,
		.salt                  = (unsigned char *)"",
		.saltLength            = 0,
		.password              = "",
		.passwordLength        = 0,
		.key                   = output,
		.keyLength             = 64,
		.allowsSwappableMemory = true
	    })) && memcmp(output, testVector1, 64) == 0)

	TEST(@"scrypt test vector #2",
	    R(of_scrypt((of_scrypt_parameters_t){
		.blockSize             = 8,
		.costFactor            = 1024,
		.parallelization       = 16,
		.salt                  = (unsigned char *)"NaCl",
		.saltLength            = 4,
		.password              = "password",
		.passwordLength        = 8,
		.key                   = output,
		.keyLength             = 64,
		.allowsSwappableMemory = true
	    })) && memcmp(output, testVector2, 64) == 0)

	TEST(@"scrypt test vector #3",
	    R(of_scrypt((of_scrypt_parameters_t){
		.blockSize             = 8,
		.costFactor            = 16384,
		.parallelization       = 1,
		.salt                  = (unsigned char *)"SodiumChloride",
		.saltLength            = 14,
		.password              = "pleaseletmein",
		.passwordLength        = 13,
		.key                   = output,
		.keyLength             = 64,
		.allowsSwappableMemory = true
	    })) && memcmp(output, testVector3, 64) == 0)

	/* The forth test vector is too expensive to include it in the tests. */
#if 0
	TEST(@"scrypt test vector #4",
	    R(of_scrypt((of_scrypt_parameters_t){
		.blockSize             = 8,
		.costFactor            = 1048576,
		.parallelization       = 1,
		.salt                  = (unsigned char *)"SodiumChloride",
		.saltLength            = 14,
		.password              = "pleaseletmein",
		.passwordLength        = 13,
		.key                   = output,
		.keyLength             = 64,
		.allowsSwappableMemory = true
	    })) && memcmp(output, testVector4, 64) == 0)
#endif

	objc_autoreleasePoolPop(pool);
}
@end
