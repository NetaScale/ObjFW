subdir('runtime')

kobjc = static_library('kobjc',
    'forwarding/forwarding.S', 'OFObject.m',
	dependencies: [ kobjcrt_dep ],
	c_args: [ kern_c_args ],
	objc_args: [ kern_objc_args, '-DOBJC_COMPILING_RUNTIME',
	    '-DOF_COMPILING_OBJFW' ],
	link_args: [ '-lgcc' ],
	include_directories : [ libc_incs, vxkern_incs, 'exceptions' ] )

libkobjc_dep = declare_dependency(link_with : [kobjc],
	dependencies: [ kobjcrt_dep ],
    compile_args: [ '-DOF_COMPILING_OBJFW' ],
	include_directories : [ '.' ] )
