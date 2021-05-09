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

#import "ObjFWRT.h"
#import "private.h"

#ifdef OF_AMIGAOS_M68K
# define PPC_PARAMS(...) (void)
# define M68K_ARG(type, name, reg)		\
	register type reg##name __asm__(#reg);	\
	type name = reg##name;
#else
# define PPC_PARAMS(...) (__VA_ARGS__)
# define M68K_ARG(...)
#endif

extern bool glue_objc_init PPC_PARAMS(unsigned int version, struct objc_libc *libc);
extern void glue___objc_exec_class PPC_PARAMS(struct objc_module *_Nonnull module);
extern IMP _Nonnull glue_objc_msg_lookup PPC_PARAMS(id _Nullable object, SEL _Nonnull selector);
extern IMP _Nonnull glue_objc_msg_lookup_stret PPC_PARAMS(id _Nullable object, SEL _Nonnull selector);
extern IMP _Nonnull glue_objc_msg_lookup_super PPC_PARAMS(struct objc_super *_Nonnull super, SEL _Nonnull selector);
extern IMP _Nonnull glue_objc_msg_lookup_super_stret PPC_PARAMS(struct objc_super *_Nonnull super, SEL _Nonnull selector);
extern Class _Nullable glue_objc_lookUpClass PPC_PARAMS(const char *_Nonnull name);
extern Class _Nullable glue_objc_getClass PPC_PARAMS(const char *_Nonnull name);
extern Class _Nonnull glue_objc_getRequiredClass PPC_PARAMS(const char *_Nonnull name);
extern Class _Nullable glue_objc_lookup_class PPC_PARAMS(const char *_Nonnull name);
extern Class _Nonnull glue_objc_get_class PPC_PARAMS(const char *_Nonnull name);
extern void glue_objc_exception_throw PPC_PARAMS(id _Nonnull object);
extern int glue_objc_sync_enter PPC_PARAMS(id _Nullable object);
extern int glue_objc_sync_exit PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_getProperty PPC_PARAMS(id _Nonnull self, SEL _Nonnull _cmd, ptrdiff_t offset, bool atomic);
extern void glue_objc_setProperty PPC_PARAMS(id _Nonnull self, SEL _Nonnull _cmd, ptrdiff_t offset, id _Nullable value, bool atomic, signed char copy);
extern void glue_objc_getPropertyStruct PPC_PARAMS(void *_Nonnull dest, const void *_Nonnull src, ptrdiff_t size, bool atomic, bool strong);
extern void glue_objc_setPropertyStruct PPC_PARAMS(void *_Nonnull dest, const void *_Nonnull src, ptrdiff_t size, bool atomic, bool strong);
extern void glue_objc_enumerationMutation PPC_PARAMS(id _Nonnull object);
extern int glue___gnu_objc_personality PPC_PARAMS(int version, int actions, uint64_t *_Nonnull exClass, void *_Nonnull ex, void *_Nonnull ctx);
extern id _Nullable glue_objc_retain PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_retainBlock PPC_PARAMS(id _Nullable block);
extern id _Nullable glue_objc_retainAutorelease PPC_PARAMS(id _Nullable object);
extern void glue_objc_release PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_autorelease PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_autoreleaseReturnValue PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_retainAutoreleaseReturnValue PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_retainAutoreleasedReturnValue PPC_PARAMS(id _Nullable object);
extern id _Nullable glue_objc_storeStrong PPC_PARAMS(id _Nullable *_Nonnull object, id _Nullable value);
extern id _Nullable glue_objc_storeWeak PPC_PARAMS(id _Nullable *_Nonnull object, id _Nullable value);
extern id _Nullable glue_objc_loadWeakRetained PPC_PARAMS(id _Nullable *_Nonnull object);
extern id _Nullable glue_objc_initWeak PPC_PARAMS(id _Nullable *_Nonnull object, id _Nullable value);
extern void glue_objc_destroyWeak PPC_PARAMS(id _Nullable *_Nonnull object);
extern id _Nullable glue_objc_loadWeak PPC_PARAMS(id _Nullable *_Nonnull object);
extern void glue_objc_copyWeak PPC_PARAMS(id _Nullable *_Nonnull dest, id _Nullable *_Nonnull src);
extern void glue_objc_moveWeak PPC_PARAMS(id _Nullable *_Nonnull dest, id _Nullable *_Nonnull src);
extern SEL _Nonnull glue_sel_registerName PPC_PARAMS(const char *_Nonnull name);
extern const char *_Nonnull glue_sel_getName PPC_PARAMS(SEL _Nonnull selector);
extern bool glue_sel_isEqual PPC_PARAMS(SEL _Nonnull selector1, SEL _Nonnull selector2);
extern Class _Nonnull glue_objc_allocateClassPair PPC_PARAMS(Class _Nullable superclass, const char *_Nonnull name, size_t extraBytes);
extern void glue_objc_registerClassPair PPC_PARAMS(Class _Nonnull class);
extern unsigned int glue_objc_getClassList PPC_PARAMS(Class _Nonnull *_Nullable buffer, unsigned int count);
extern Class _Nonnull *_Nonnull glue_objc_copyClassList PPC_PARAMS(unsigned int *_Nullable length);
extern bool glue_class_isMetaClass PPC_PARAMS(Class _Nullable class);
extern const char *_Nullable glue_class_getName PPC_PARAMS(Class _Nullable class);
extern Class _Nullable glue_class_getSuperclass PPC_PARAMS(Class _Nullable class);
extern unsigned long glue_class_getInstanceSize PPC_PARAMS(Class _Nullable class);
extern bool glue_class_respondsToSelector PPC_PARAMS(Class _Nullable class, SEL _Nonnull selector);
extern bool glue_class_conformsToProtocol PPC_PARAMS(Class _Nullable class, Protocol *_Nonnull p);
extern IMP _Nullable glue_class_getMethodImplementation PPC_PARAMS(Class _Nullable class, SEL _Nonnull selector);
extern IMP _Nullable glue_class_getMethodImplementation_stret PPC_PARAMS(Class _Nullable class, SEL _Nonnull selector);
extern Method _Nullable glue_class_getInstanceMethod PPC_PARAMS(Class _Nullable class, SEL _Nonnull selector);
extern bool glue_class_addMethod PPC_PARAMS(Class _Nonnull class, SEL _Nonnull selector, IMP _Nonnull implementation, const char *_Nullable typeEncoding);
extern IMP _Nullable glue_class_replaceMethod PPC_PARAMS(Class _Nonnull class, SEL _Nonnull selector, IMP _Nonnull implementation, const char *_Nullable typeEncoding);
extern Class _Nullable glue_object_getClass PPC_PARAMS(id _Nullable object);
extern Class _Nullable glue_object_setClass PPC_PARAMS(id _Nullable object, Class _Nonnull class);
extern const char *_Nullable glue_object_getClassName PPC_PARAMS(id _Nullable object);
extern const char *_Nonnull glue_protocol_getName PPC_PARAMS(Protocol *_Nonnull protocol);
extern bool glue_protocol_isEqual PPC_PARAMS(Protocol *_Nonnull protocol1, Protocol *_Nonnull protocol2);
extern bool glue_protocol_conformsToProtocol PPC_PARAMS(Protocol *_Nonnull protocol1, Protocol *_Nonnull protocol2);
extern _Nullable objc_uncaught_exception_handler glue_objc_setUncaughtExceptionHandler PPC_PARAMS(objc_uncaught_exception_handler _Nullable handler);
extern void glue_objc_setForwardHandler PPC_PARAMS(IMP _Nullable forward, IMP _Nullable stretForward);
extern void glue_objc_setEnumerationMutationHandler PPC_PARAMS(objc_enumeration_mutation_handler _Nullable hadler);
extern id _Nullable glue_objc_constructInstance PPC_PARAMS(Class _Nullable class, void *_Nullable bytes);
extern void glue_objc_deinit(void);
extern Ivar _Nullable *_Nullable glue_class_copyIvarList PPC_PARAMS(Class _Nullable class, unsigned int *_Nullable outCount);
extern const char *_Nonnull glue_ivar_getName PPC_PARAMS(Ivar _Nonnull ivar);
extern const char *_Nonnull glue_ivar_getTypeEncoding PPC_PARAMS(Ivar _Nonnull ivar);
extern ptrdiff_t glue_ivar_getOffset PPC_PARAMS(Ivar _Nonnull ivar);
extern Method _Nullable *_Nullable glue_class_copyMethodList PPC_PARAMS(Class _Nullable class, unsigned int *_Nullable outCount);
extern SEL _Nonnull glue_method_getName PPC_PARAMS(Method _Nonnull method);
extern const char *_Nullable glue_method_getTypeEncoding PPC_PARAMS(Method _Nonnull method);
extern objc_property_t _Nullable *_Nullable glue_class_copyPropertyList PPC_PARAMS(Class _Nullable class, unsigned int *_Nullable outCount);
extern const char *_Nonnull glue_property_getName PPC_PARAMS(objc_property_t _Nonnull property);
extern char *_Nullable glue_property_copyAttributeValue PPC_PARAMS(objc_property_t _Nonnull property, const char *_Nonnull name);
extern void *_Nullable glue_objc_destructInstance PPC_PARAMS(id _Nullable object);
extern void *_Null_unspecified glue_objc_autoreleasePoolPush(void);
extern void glue_objc_autoreleasePoolPop PPC_PARAMS(void *_Null_unspecified pool);
extern id _Nullable glue__objc_rootAutorelease PPC_PARAMS(id _Nullable object);
extern struct objc_hashtable *_Nonnull glue_objc_hashtable_new PPC_PARAMS(objc_hashtable_hash_func hash, objc_hashtable_equal_func equal, uint32_t size);
extern void glue_objc_hashtable_set PPC_PARAMS(struct objc_hashtable *_Nonnull table, const void *_Nonnull key, const void *_Nonnull object);
extern void *_Nullable glue_objc_hashtable_get PPC_PARAMS(struct objc_hashtable *_Nonnull table, const void *_Nonnull key);
extern void glue_objc_hashtable_delete PPC_PARAMS(struct objc_hashtable *_Nonnull table, const void *_Nonnull key);
extern void glue_objc_hashtable_free PPC_PARAMS(struct objc_hashtable *_Nonnull table);
extern void glue_objc_setTaggedPointerSecret PPC_PARAMS(uintptr_t secret);
extern int glue_objc_registerTaggedPointerClass PPC_PARAMS(Class _Nonnull class);
extern bool glue_object_isTaggedPointer PPC_PARAMS(id _Nullable object);
extern uintptr_t glue_object_getTaggedPointerValue PPC_PARAMS(id _Nonnull object);
extern id _Nullable glue_objc_createTaggedPointer PPC_PARAMS(int class, uintptr_t value);
