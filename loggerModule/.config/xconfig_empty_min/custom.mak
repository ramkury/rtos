## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e430X linker.cmd package/cfg/empty_min_pe430X.oe430X

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/empty_min_pe430X.xdl
	$(SED) 's"^\"\(package/cfg/empty_min_pe430Xcfg.cmd\)\"$""\"D:/UnB/Topicos/SO_MSP430/CodeC/rtos/loggerModule/.config/xconfig_empty_min/\1\""' package/cfg/empty_min_pe430X.xdl > $@
	-$(SETDATE) -r:max package/cfg/empty_min_pe430X.h compiler.opt compiler.opt.defs
