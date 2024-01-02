.PHONY: build clean
build: build/stage1.bin build/stage2.bin

build/stage1.bin: script/stage1.ld $(subst stage1/,build/stage1/,$(addsuffix .o,$(shell find stage1/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter build/stage1/%.o,$^)
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/stage2.bin: script/stage2.ld $(subst stage2/,build/stage2/,$(addsuffix .o,$(shell find stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter %.o,$^)
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/%.asm.o: %.asm
	@mkdir -p ${@D}
	${AS} -g -MD $(addsuffix .d,$(basename $@)) -I $(dir $<) -o $@ $<

build/%.c.o: %.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -std=gnu17 -ggdb -ffreestanding -nostartfiles -MD -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -I ${INCLUDE} -I . -o $@ $<

ifeq (1,$(shell if [ -d build/ ]; then echo 1; fi))
    -include $(shell find build/ -type f -name '*.d')
endif

clean:
	rm -rf build/
