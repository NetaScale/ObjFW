/* Functions that are only for the linklib. */
bool glue_objc_init(unsigned int, struct objc_libc *, FILE *, FILE *);
/* All other functions. */
void glue___objc_exec_class(struct objc_module *);
IMP glue_objc_msg_lookup(id, SEL);
IMP glue_objc_msg_lookup_stret(id, SEL);
IMP glue_objc_msg_lookup_super(struct objc_super *, SEL);
IMP glue_objc_msg_lookup_super_stret(struct objc_super *, SEL);
Class glue_objc_lookUpClass(const char *);
Class glue_objc_getClass(const char *);
Class glue_objc_getRequiredClass(const char *);
Class glue_objc_lookup_class(const char *);
Class glue_objc_get_class(const char *);
void glue_objc_exception_throw(id);
int glue_objc_sync_enter(id);
int glue_objc_sync_exit(id);
id glue_objc_getProperty(id, SEL, ptrdiff_t, bool);
void glue_objc_setProperty(id, SEL, ptrdiff_t, id, bool, signed char);
void glue_objc_getPropertyStruct(void *, const void *, ptrdiff_t, bool, bool);
void glue_objc_setPropertyStruct(void *, const void *, ptrdiff_t, bool, bool);
void glue_objc_enumerationMutation(id);
int glue___gnu_objc_personality(int, int, uint64_t, void *, void *);
id glue_objc_retain(id);
id glue_objc_retainBlock(id);
id glue_objc_retainAutorelease(id);
void glue_objc_release(id);
id glue_objc_autorelease(id);
id glue_objc_autoreleaseReturnValue(id);
id glue_objc_retainAutoreleaseReturnValue(id);
id glue_objc_retainAutoreleasedReturnValue(id);
id glue_objc_storeStrong(id *, id);
id glue_objc_storeWeak(id *, id);
id glue_objc_loadWeakRetained(id *);
id glue_objc_initWeak(id *, id);
void glue_objc_destroyWeak(id *);
id glue_objc_loadWeak(id *);
void glue_objc_copyWeak(id *, id *);
void glue_objc_moveWeak(id *, id *);
SEL glue_sel_registerName(const char *);
const char *glue_sel_getName(SEL);
bool glue_sel_isEqual(SEL, SEL);
Class glue_objc_allocateClassPair(Class, const char *, size_t);
void glue_objc_registerClassPair(Class);
unsigned int glue_objc_getClassList(Class *, unsigned int);
Class *glue_objc_copyClassList(unsigned int *);
bool glue_class_isMetaClass(Class);
const char *glue_class_getName(Class);
Class glue_class_getSuperclass(Class);
unsigned long glue_class_getInstanceSize(Class);
bool glue_class_respondsToSelector(Class, SEL);
bool glue_class_conformsToProtocol(Class, Protocol *);
IMP glue_class_getMethodImplementation(Class, SEL);
IMP glue_class_getMethodImplementation_stret(Class, SEL);
Method glue_class_getInstanceMethod(Class, SEL);
bool glue_class_addMethod(Class, SEL, IMP, const char *);
IMP glue_class_replaceMethod(Class, SEL, IMP, const char *);
Class glue_object_getClass(id);
Class glue_object_setClass(id, Class);
const char *glue_object_getClassName(id);
const char *glue_protocol_getName(Protocol *);
bool glue_protocol_isEqual(Protocol *, Protocol *);
bool glue_protocol_conformsToProtocol(Protocol *, Protocol *);
objc_uncaught_exception_handler_t glue_objc_setUncaughtExceptionHandler(objc_uncaught_exception_handler_t);
void glue_objc_setForwardHandler(IMP, IMP);
void glue_objc_setEnumerationMutationHandler(objc_enumeration_mutation_handler_t);
id glue_objc_constructInstance(Class, void *);
void glue_objc_exit(void);
Ivar *glue_class_copyIvarList(Class, unsigned int *);
const char *glue_ivar_getName(Ivar);
const char *glue_ivar_getTypeEncoding(Ivar);
ptrdiff_t glue_ivar_getOffset(Ivar);
Method *glue_class_copyMethodList(Class, unsigned int *);
SEL glue_method_getName(Method);
const char *glue_method_getTypeEncoding(Method);
objc_property_t *glue_class_copyPropertyList(Class, unsigned int *);
const char *glue_property_getName(objc_property_t);
char *glue_property_copyAttributeValue(objc_property_t, const char *);
void *glue_objc_destructInstance(id);
void *glue_objc_autoreleasePoolPush(void);
void glue_objc_autoreleasePoolPop(void *);
id glue__objc_rootAutorelease(id);
