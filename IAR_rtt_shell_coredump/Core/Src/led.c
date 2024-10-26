
static void mydelay(volatile int d)
{
    while (d--);
}

void led_blink(void *parameter)
{
    volatile unsigned int *p = (volatile unsigned int *)0x40021414;
    
    while (1)
    {
        *p |= (1<<9);
        rt_thread_mdelay(500);
        //mydelay(10000000);
        
        *p &= ~(1<<9);
        rt_thread_mdelay(500);
       // mydelay(10000000);
    }
}