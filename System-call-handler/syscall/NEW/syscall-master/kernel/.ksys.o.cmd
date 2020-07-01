cmd_/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o := gcc -Wp,-MD,/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/.ksys.o.d  -nostdinc -isystem /usr/lib/gcc/i686-linux-gnu/5/include  -I./arch/x86/include -I./arch/x86/include/generated  -I./include -I./arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__ -D__ASSEMBLY__ -fno-PIE -m32 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_AVX512=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1 -DCC_HAVE_ASM_GOTO -mfentry -DCC_USING_FENTRY -DMODULE  -c -o /home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o /home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.S

source_/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o := /home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.S

deps_/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o := \
  include/linux/compiler_types.h \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/alignment/16.h) \

/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o: $(deps_/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o)

$(deps_/home/m/Downloads/syscall-master/NEW/syscall-master/kernel/ksys.o):
