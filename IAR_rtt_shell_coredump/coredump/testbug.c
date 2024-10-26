#include <rtthread.h>

void EnableDiv0Bug(void)
{
	/* 100ask add */
	/* 使能除0错误
	 * CCR(0xE000ED14)的bit4(DIV_0_TRP)设置为1
	 */
	volatile int *CCR = (volatile int *)0xE000ED14;
	*CCR |= (1<<4);

}

volatile int g_val = 0x12345678;

int CreateBug(int b, int n)
{
    int ret;
	int (*f)(int, int) = 0;
	
    rt_kprintf("in CreateBug\n");
	ret =  b / n;
	//f(1,2);
	for (int i =0; i < 10000000; i++)
		rt_thread_delay(10);
    rt_kprintf("leave CreateBug\n");
    return ret;
}

void D(int n, int m)
{
    rt_kprintf("in D\n");
	CreateBug(n, m);
    rt_kprintf("leave D\n");
}
void C(int n, int m)
{
    rt_kprintf("in C\n");
	D(n, m);
    rt_kprintf("leave C\n");
}

void B(int n, int m)
{
    rt_kprintf("in B\n");
	C(n, m);
    rt_kprintf("leave B\n");
}

void AA(void)
{
    volatile int a = 1;
}

void A(int n, int m)
{
    rt_kprintf("in A\n");
    AA();
	B(g_val*n, m);
    rt_kprintf("leave A\n");
}

void buf_overflow(int n, char val)
{
    volatile char buf[5];
    buf[0] = 0x5a;
    buf[n] = val;
    rt_kprintf("buf[0] is 0x%08x\n", buf[0]);
}


void test_softbreakpoint(void)
{
    volatile int a = 0x55;
    
    rt_kprintf("\n****** a's address is ***** 0x%08x\n", (unsigned int)&a);
    
    A(100, 1);
    
    buf_overflow(16, 'A');
    
    rt_kprintf("\n****** a is ***** 0x%08x\n", a);
}


