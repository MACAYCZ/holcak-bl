#include "ctype.h"

int isdigit(int value)
{
	return value >= '0' && value <= '9';
}

int isspace(int value)
{
	return value == ' ' || value == '\f' || value == '\n' || value == '\r' || value == '\t' || value == '\v';
}

int islower(int value)
{
	return value >= 'a' && value <= 'z';
}

int isupper(int value)
{
	return value >= 'A' && value <= 'Z';
}

int tolower(int value)
{
	if (isupper(value)) value += 'a' - 'A';
	return value;
}

int toupper(int value)
{
	if (islower(value)) value -= 'a' - 'A';
	return value;
}
