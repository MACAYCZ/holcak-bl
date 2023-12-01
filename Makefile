include ${CONFIG}
AS := nasm
CC ?= cc
LD ?= ld
OBJCOPY ?= objcopy
INCLUDE ?= /usr/lib/gcc/*/*/include/

.PHONY: build clean
build: build/stage1.bin build/stage2.bin

build/stage1.bin: $(shell find stage1/ -type f -name '*.asm')
	@mkdir -p ${@D}
	${AS} -fbin -I $(dir $^) -l $(addsuffix .lst,$(basename $@)) -o $@ $^

build/stage2.bin: script/linker.ld $(subst stage2/,build/stage2/,$(addsuffix .o,$(shell find stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter %.o,$^)
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/%.asm.o: %.asm
	@mkdir -p ${@D}
	${AS} -felf32 -g -Fdwarf -I $(dir $<) -MD $(addsuffix .d,$(basename $@)) -o $@ $<

build/%.c.o: %.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -std=gnu17 -ggdb -ffreestanding -nostartfiles -MD -fno-pie -fno-pic -nostdlib -nostdinc -c -masm=intel -m32 -I ${INCLUDE} -o $@ $<

ifeq (1,$(shell if [ -d build/ ]; then echo 1; fi))
    -include $(shell find build/ -type f -name '*.d')
endif

clean:
	rm -rf build/
