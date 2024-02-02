#include <stage2/protocol/chainload.h>
#include <stage2/interface/console.h>
#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
#include <stage2/driver/idt.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

typedef struct __packed {
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t sp;
	uint32_t ss;
} int_frame_32_t;

#pragma GCC push_options
#pragma GCC target("general-regs-only")
__interrupt void foo(int_frame_32_t *frame)
{
	(void)frame;
	puts("Exception: Division Error!");
	while (1);
}
#pragma GCC pop_options

__cdecl noreturn void main(void)
{
	console_init();

	disk_t disk;
	if (!disk_init(&disk, 0x00)) {
		puts("ERROR: Could not initialize disk!");
		while (1);
	}

	if (!chainload_init(disk)) {
		puts("ERROR: Could not chainload!");
		while (1);
	}

	chainload_jump(disk.id);
}
