
#include "mystring.h"
#include "myuart.h"

int myputs(const char *s)
{
	while (*s)
	{
		myputchar(*s);
		s++;
	}
	return 0;
}

void myputhex(unsigned int val)
{
	/* 0x76543210 */
	int i, j;

	myputs("0x");
	for (i = 7; i >= 0; i--)
	{
		j = (val >> (i*4)) & 0xf;
		if ((j >= 0) && (j <= 9))
			myputchar('0' + j);
		else
			myputchar('A' + j - 0xA);
	}	
}

void myput_s_hex(const char *s, unsigned int val)
{
	myputs(s);
	myputhex(val);
	//myputs("\r\n");
}

