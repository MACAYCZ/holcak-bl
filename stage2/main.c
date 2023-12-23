#include <stage2/interface/print.h>
#include <stage2/driver/x86_16.h>
#include <stage2/memory/info.h>
#include <stage2/driver/port.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

__cdecl noreturn void main(void)
{
	printf("\e\x34Hello, \e\x12%u!\n", 0x1234);

	mem_info_t mem_info = mem_init();
//	pmm_info_t pmm_info = pmm_init();
	while (1);
}
