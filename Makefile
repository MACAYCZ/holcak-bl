$(if ${TARGET},,$(error Toolchain TARGET wasn't provided!))
$(if ${PREFIX},,$(error Toolchain PREFIX wasn't provided!))

AS := ${PREFIX}/${TARGET}/bin/${TARGET}-as
CC := ${PREFIX}/${TARGET}/bin/${TARGET}-cc
LD := ${PREFIX}/${TARGET}/bin/${TARGET}-ld
OBJCOPY := ${PREFIX}/${TARGET}/bin/${TARGET}-objcopy

.PHONY: build clean
build: build/stage1/build.bin build/stage2/build.bin

build/stage1/build.bin: script/stage1.ld $(subst stage1/,build/stage1/,$(addsuffix .o,$(shell find stage1/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter %.o,$^)
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/stage2/build.bin: script/stage2.ld $(subst stage2/,build/stage2/,$(addsuffix .o,$(shell find stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T $< --gc-sections -Map $(addsuffix .map,$(basename $@)) -nostdlib -o $(addsuffix .elf,$(basename $@)) $(filter %.o,$(filter-out build/stage1/%,$^))
	${OBJCOPY} -O binary --strip-debug $(addsuffix .elf,$(basename $@)) $@

build/%.asm.o: %.asm
	@mkdir -p ${@D}
	${AS} -g -MD $(addsuffix .d,$(basename $@)) -I $(dir $<) -o $@ $<

build/%.c.o: source/%.c
	$(error Not implemented yet!)

ifeq (1,$(shell if [ -d build/ ]; then echo 1; fi))
    -include $(shell find build/ -type f -name '*.d')
endif

clean:
	rm -rf build/
