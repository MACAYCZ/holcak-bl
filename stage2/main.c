#include <stdnoreturn.h>

#include <stage2/memory/info.h>
#include <stage2/console.h>
#include <stage2/define.h>

__cdecl noreturn void main(void)
{
	printf("[DEBUG] Entered protected mode!\n\r");
	mem_init();
	while (1);
}
