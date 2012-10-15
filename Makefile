AS := nasm
CC := i386-elf-gcc
LD := i386-elf-ld
AR := i386-elf-ar

ASFLAGS := 
CCFLAGS := -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -Isrc/include -MMD -g
LDFLAGS := 
ARFLAGS := 

LDLIBS := -Lbuild/lib -lc

ASFLAGS_kernel := -f elf
CCFLAGS_kernel := -Isrc/kernel/include
LDFLAGS_kernel := -Tsrc/kernel/src/kernel.ld

OUTPUT_kernel := kernel.bin
SOURCE_kernel := $(notdir $(shell find src/kernel/src -name '*.[cS]'))

OUTPUT_iso := bootable.iso
SOURCE_iso := build/kernel/$(OUTPUT_kernel) src/grub/menu.lst src/grub/stage2_eltorito

CCFLAGS_lib := 
ARFLAGS_lib := rc

OUTPUT_lib := libc.a
SOURCE_lib := $(shell find src/lib -name '*.c')

all: dirs lib kernel iso

dirs:
	@mkdir -p build/kernel/obj
	@mkdir -p build/iso/files/boot/grub
	@mkdir -p build/lib/obj/string

kernel: build/kernel/$(OUTPUT_kernel)

build/kernel/obj/%.o: src/kernel/src/%.S
	@$(AS) $(ASFLAGS) $(ASFLAGS_kernel) -o $@ $<

build/kernel/obj/%.o: src/kernel/src/%.c
	@$(CC) $(CCFLAGS) $(CCFLAGS_kernel) -c $< -o $@

build/kernel/$(OUTPUT_kernel): $(patsubst %,build/kernel/obj/%.o,$(basename $(SOURCE_kernel))) build/lib/$(OUTPUT_lib)
	@$(LD) $(LDFLAGS) $(LDFLAGS_kernel) -o $@ $^ $(LDLIBS)

iso: build/iso/$(OUTPUT_iso)

build/iso/$(OUTPUT_iso): $(SOURCE_iso)
	@cp -u build/kernel/$(OUTPUT_kernel) build/iso/files/boot/
	@cp -u src/grub/menu.lst build/iso/files/boot/grub/
	@cp -u src/grub/stage2_eltorito build/iso/files/boot/grub/
	@genisoimage -input-charset utf-8 -quiet -R -b boot/grub/stage2_eltorito \
	-no-emul-boot -boot-load-size 4 -boot-info-table -o build/iso/$(OUTPUT_iso) build/iso/files

lib: build/lib/$(OUTPUT_lib)

build/lib/$(OUTPUT_lib): $(patsubst src/lib/%.c,build/lib/obj/%.o,$(SOURCE_lib))
	@$(AR) $(ARFLAGS) $(ARFLAGS_lib) $@ $?

build/lib/obj/%.o: src/lib/%.c
	@$(CC) $(CCFLAGS) $(CCFLAGS_lib) -c $< -o $@

.PHONY: clean rebuild

rebuild: clean all

clean:
	@rm -rf build/*

DEPENDENCIES := $(DEPENDENCIES) $(shell find build -name '*.d')
-include $(sort $(DEPENDENCIES))
