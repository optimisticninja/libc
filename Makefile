BINARIES := build/libc.a build/libk.a build/libg.a

CC := x86_64-elf-gcc
AR := x86_64-elf-ar
CSTD := -std=gnu11
WARNINGS := -Wall -Werror -Wextra
INCLUDES := -Iinclude -I../kernel/include
CFLAGS := $(CSTD) $(WARNINGS) -nostdlib

LIBC_SOURCES := $(shell find src -name **.c )
LIBC_OBJECTS := $(shell echo $(addprefix build/, $(shell echo $(LIBC_SOURCES:.c=.o) | sed 's/src\///g')))
LIBK_OBJECTS := $(shell echo $(LIBC_OBJECTS:.o=.libk.o))
 
 CPP_FLAGS := -D__is_libc
LIBK_CFLAGS := $(CFLAGS)
LIBK_CPPFLAGS := $(CPP_FLAGS) -D__is_kernel

LIBK_INCLUDES := $(INCLUDES)

DESTDIR := sysroot
INCLUDEDIR := /usr/include
LIBDIR := /usr/lib

all-libc:
	$(MAKE) $(LIBC_OBJECTS)
	$(MAKE) $(LIBK_OBJECTS)
	$(MAKE) $(BINARIES)
	$(MAKE) install-libc

build/libc.a: $(LIBC_OBJECTS) $(LIBC_ARCH_ASM_OBJECTS)
	$(AR) rcs $@ $^

build/libg.a:
	$(AR) rcs $@

build/libk.a: $(LIBK_OBJECTS)
	$(AR) rcs $@ $^

$(LIBC_OBJECTS): build%.o: src%.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
 
$(LIBK_OBJECTS): build%libk.o: src%c
	$(CC) $(LIBK_INCLUDES) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -c $< -o $@

install-libc-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-libc-libs:
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

install-libc: install-libc-headers install-libc-libs
# 
# #$(LIBC_ARCH_ASM_OBJECTS): build%.o: libc%.asm
# #	nasm -felf32 $< -o $@
# 
clean-libc:
	rm -fv $(LIBC_OBJECTS) $(LIBC_ARCH_ASM_OBJECTS) $(LIBK_OBJECTS) $(BINARIES)
	find sysroot/usr/lib -name '*.a' -type f -delete
