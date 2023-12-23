#include <stdint.h>
#include <stddef.h>

uint64_t __udivmoddi4(uint64_t divident, uint64_t divisor, uint64_t *reminder)
{
	uint64_t quotient = 0;
	uint64_t count = 1;

	if (divisor == 0) {
		return 1 / ((unsigned)divisor);
	}

	while ((int64_t)divisor >= 0) {
		divisor <<= 1;
		count <<= 1;
	}

	while (count) {
		if (divisor <= divident) {
			divident -= divisor;
			quotient += count;
		}
		divisor >>= 1;
		count >>= 1;
	}

	if (reminder) {
		*reminder = divident;
	}

	return quotient;
}

uint64_t __umoddi3(uint64_t divident, uint64_t divisor)
{
	uint64_t reminder;
	__udivmoddi4(divident, divisor, &reminder);
	return reminder;
}

uint64_t __udivdi3(uint64_t divident, uint64_t divisor)
{
	return __udivmoddi4(divident, divisor, NULL);
}
