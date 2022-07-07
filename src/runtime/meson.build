kobjcrt_srcs = [ 'arc.m', 'autorelease.m', 'category.m',
	  'class.m', 'dtable.m', 'exception.m', 'hashtable.m', 'init.m',
	  'instance.m', 'ivar.m', 'lookup.m', 'method.m', 'misc.m', 'property.m',
	  'protocol.m', 'selector.m', 'sparsearray.m', 'static-instances.m',
	  'synchronized.m', 'tagged-pointer.m',
	  'lookup-asm/lookup-asm-x86_64-elf.S' ]

kobjcrt = static_library('kobjcrt', kobjcrt_srcs,
	c_args: [ kernel_c_args ],
	objc_args: [ kernel_objc_args, '-DOBJC_COMPILING_RUNTIME',
	    '-DOF_COMPILING_OBJFW' ],
	link_args: [ '-lgcc' ],
	include_directories : [ libc_includes, kernel_includes, '../' ] )

kobjcrt_dep = declare_dependency(link_with : [ kobjcrt ],
	include_directories : [ '.' ] )
