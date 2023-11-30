CONFIG ?= config.mk
include ${CONFIG}

.PHONY: build clean
build: build/stage1.bin build/stage2.bin

build/stage1.bin: script/stage1.ld $(subst stage1/,build/stage1/,$(addsuffix .o,$(shell find stage1/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter build/stage1/%.o,$^)
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/stage2.bin: script/stage2.ld $(subst stage2/,build/stage2/,$(addsuffix .o,$(shell find stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter %.o,$(filter-out build/stage1/%,$^))
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/%.asm.o: %.asm
	@mkdir -p ${@D}
	${AS} --32 -g -MD $(addsuffix .d,$(basename $@)) -I $(dir $<) -o $@ $<

build/%.16.c.o: %.16.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -std=gnu17 -ggdb -ffreestanding -nostartfiles -MD -fno-pie -fno-pic -nostdlib -nostdinc -c -masm=intel -m16 -I ${INCLUDE} -o $@ $<
	${CC} -S -Wall -Wextra -pedantic -std=gnu17 -ggdb -ffreestanding -nostartfiles -MD -fno-pie -fno-pic -nostdlib -nostdinc -c -masm=intel -m16 -I ${INCLUDE} -o $(addsuffix .asm,$(basename $@)) $<	

build/%.32.c.o: %.32.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -std=gnu17 -ggdb -ffreestanding -nostartfiles -MD -fno-pie -fno-pic -nostdlib -nostdinc -c -masm=intel -m32 -I ${INCLUDE} -o $@ $<

ifeq (1,$(shell if [ -d build/ ]; then echo 1; fi))
    -include $(shell find build/ -type f -name '*.d')
endif

clean:
	rm -rf build/
