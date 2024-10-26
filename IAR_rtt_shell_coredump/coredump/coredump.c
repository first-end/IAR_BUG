// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */
 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 免责声明: 百问网编写的文档, 仅供学员学习使用, 可以转发或引用(请保留作者信息),禁止用于商业用途！
 * 免责声明: 百问网编写的程序, 可以用于商业用途, 但百问网不承担任何后果！
 * 
 * 本程序遵循GPL V3协议, 请遵循协议
 * 百问网学习平台   : https://www.100ask.net
 * 百问网交流社区   : https://forums.100ask.net
 * 百问网官方B站    : https://space.bilibili.com/275908810
 * 百问网官方淘宝   : https://100ask.taobao.com
 * 联系我们(E-mail): weidongshan@qq.com
 *
 *          版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2024.10.17      v01         百问科技      创建文件
 *-----------------------------------------------------
 */
 
#include <rtthread.h>
#include <string.h>
#include <stdint.h>
#include "coredump.h"


#if               /* ARMCC */ (  (defined ( __CC_ARM ) && defined ( __TARGET_FPU_VFP ))    \
                  /* Clang */ || (defined ( __clang__ ) && defined ( __VFP_FP__ ) && !defined(__SOFTFP__)) \
                  /* IAR */   || (defined ( __ICCARM__ ) && defined ( __ARMVFP__ ))        \
                  /* GNU */   || (defined ( __GNUC__ ) && defined ( __VFP_FP__ ) && !defined(__SOFTFP__)) )
#define USE_FPU   1
#else
#define USE_FPU   0
#endif

struct exception_stack_frame
{
    rt_uint32_t r0;
    rt_uint32_t r1;
    rt_uint32_t r2;
    rt_uint32_t r3;
    rt_uint32_t r12;
    rt_uint32_t lr;
    rt_uint32_t pc;
    rt_uint32_t psr;
};

struct stack_frame
{
#if USE_FPU
    rt_uint32_t flag;
#endif /* USE_FPU */

    /* r4 ~ r11 register */
    rt_uint32_t r4;
    rt_uint32_t r5;
    rt_uint32_t r6;
    rt_uint32_t r7;
    rt_uint32_t r8;
    rt_uint32_t r9;
    rt_uint32_t r10;
    rt_uint32_t r11;

    struct exception_stack_frame exception_stack_frame;
};

struct exception_stack_frame_fpu
{
    rt_uint32_t r0;
    rt_uint32_t r1;
    rt_uint32_t r2;
    rt_uint32_t r3;
    rt_uint32_t r12;
    rt_uint32_t lr;
    rt_uint32_t pc;
    rt_uint32_t psr;

#if USE_FPU
    /* FPU register */
    rt_uint32_t S0;
    rt_uint32_t S1;
    rt_uint32_t S2;
    rt_uint32_t S3;
    rt_uint32_t S4;
    rt_uint32_t S5;
    rt_uint32_t S6;
    rt_uint32_t S7;
    rt_uint32_t S8;
    rt_uint32_t S9;
    rt_uint32_t S10;
    rt_uint32_t S11;
    rt_uint32_t S12;
    rt_uint32_t S13;
    rt_uint32_t S14;
    rt_uint32_t S15;
    rt_uint32_t FPSCR;
    rt_uint32_t NO_NAME;
#endif
};

struct stack_frame_fpu
{
    rt_uint32_t flag;

    /* r4 ~ r11 register */
    rt_uint32_t r4;
    rt_uint32_t r5;
    rt_uint32_t r6;
    rt_uint32_t r7;
    rt_uint32_t r8;
    rt_uint32_t r9;
    rt_uint32_t r10;
    rt_uint32_t r11;

#if USE_FPU
    /* FPU register s16 ~ s31 */
    rt_uint32_t s16;
    rt_uint32_t s17;
    rt_uint32_t s18;
    rt_uint32_t s19;
    rt_uint32_t s20;
    rt_uint32_t s21;
    rt_uint32_t s22;
    rt_uint32_t s23;
    rt_uint32_t s24;
    rt_uint32_t s25;
    rt_uint32_t s26;
    rt_uint32_t s27;
    rt_uint32_t s28;
    rt_uint32_t s29;
    rt_uint32_t s30;
    rt_uint32_t s31;
#endif

    struct exception_stack_frame_fpu exception_stack_frame;
};

struct exception_info
{
    rt_uint32_t exc_return;
    struct stack_frame stack_frame;
};

static void DumpRegisters(struct stack_frame *sp, char *thread)
{
#if USE_FPU
	if (sp->flag) /* 使用FPU */
	{
		struct stack_frame_fpu *sp_fpu = (struct stack_frame_fpu *)sp;
		rt_kprintf("Registers@%s\n", thread);
		rt_kprintf("R0: 0x%08x\n", sp_fpu->exception_stack_frame.r0);
		rt_kprintf("R1: 0x%08x\n", sp_fpu->exception_stack_frame.r1);
		rt_kprintf("R2: 0x%08x\n", sp_fpu->exception_stack_frame.r2);
		rt_kprintf("R3: 0x%08x\n", sp_fpu->exception_stack_frame.r3);
		rt_kprintf("R4: 0x%08x\n", sp_fpu->r4);
		rt_kprintf("R5: 0x%08x\n", sp_fpu->r5);
		rt_kprintf("R6: 0x%08x\n", sp_fpu->r6);
		rt_kprintf("R7: 0x%08x\n", sp_fpu->r7);
		rt_kprintf("R8: 0x%08x\n", sp_fpu->r8);
		rt_kprintf("R9: 0x%08x\n", sp_fpu->r9);
		rt_kprintf("R10: 0x%08x\n", sp_fpu->r10);
		rt_kprintf("R11: 0x%08x\n", sp_fpu->r11);
		rt_kprintf("R12: 0x%08x\n", sp_fpu->exception_stack_frame.r12);
		rt_kprintf("R13(sp_fpu): 0x%08x\n", (uint32_t)sp_fpu + sizeof(*sp_fpu));
		rt_kprintf("R14(LR): 0x%08x\n", sp_fpu->exception_stack_frame.lr);
		rt_kprintf("R15(PC): 0x%08x\n", sp_fpu->exception_stack_frame.pc);
		rt_kprintf("xPSR: 0x%08x\n", sp_fpu->exception_stack_frame.psr);
	}
	else
#endif
	{
		rt_kprintf("Registers@%s\n", thread);
		rt_kprintf("R0: 0x%08x\n", sp->exception_stack_frame.r0);
		rt_kprintf("R1: 0x%08x\n", sp->exception_stack_frame.r1);
		rt_kprintf("R2: 0x%08x\n", sp->exception_stack_frame.r2);
		rt_kprintf("R3: 0x%08x\n", sp->exception_stack_frame.r3);
		rt_kprintf("R4: 0x%08x\n", sp->r4);
		rt_kprintf("R5: 0x%08x\n", sp->r5);
		rt_kprintf("R6: 0x%08x\n", sp->r6);
		rt_kprintf("R7: 0x%08x\n", sp->r7);
		rt_kprintf("R8: 0x%08x\n", sp->r8);
		rt_kprintf("R9: 0x%08x\n", sp->r9);
		rt_kprintf("R10: 0x%08x\n", sp->r10);
		rt_kprintf("R11: 0x%08x\n", sp->r11);
		rt_kprintf("R12: 0x%08x\n", sp->exception_stack_frame.r12);
		rt_kprintf("R13(SP): 0x%08x\n", (uint32_t)sp + sizeof(*sp));
		rt_kprintf("R14(LR): 0x%08x\n", sp->exception_stack_frame.lr);
		rt_kprintf("R15(PC): 0x%08x\n", sp->exception_stack_frame.pc);
		rt_kprintf("xPSR: 0x%08x\n", sp->exception_stack_frame.psr);
	}
	
}

static void DumpMem(uint32_t addr, uint32_t len)
{
	uint32_t *paddr;
	uint32_t i;

	if (len == 0)
		return;
	
	paddr = (uint32_t *)addr;
	rt_kprintf("mem@0x%08x,0x%08x\n", addr, len);
	
	for (i = 0; i < len;)
	{
		rt_kprintf("0x%08x", *paddr);
		paddr++;

		i+= 4;
		if (i % 16 == 0)
			rt_kprintf("\n");
		else
			rt_kprintf(" ");
	}
	rt_kprintf("\n");
}

void DumpTasks(void)
{
    struct rt_object *object = RT_NULL;
    struct rt_list_node *node = RT_NULL;
    struct rt_object_information *information = RT_NULL;
	struct rt_thread *thread;
	rt_thread_t current_thread = rt_thread_self();
	struct stack_frame *sp;
	uint32_t sp_addr;
	uint32_t sp_len;

    information = rt_object_get_information(RT_Object_Class_Thread);

    /* parameter check */
    if (information == RT_NULL)
        return ;

    /* enter critical */
    rt_enter_critical();

    /* try to find object */
    rt_list_for_each(node, &(information->object_list))
    {
        object = rt_list_entry(node, struct rt_object, list);
		thread = (struct rt_thread *)object;
		if (thread != current_thread)
		{
			/* 打印线程的寄存器 */
			DumpRegisters(thread->sp, thread->name);

			/* 打印线程的栈 */
			rt_kprintf("Stack segment:\n");
			sp = (struct stack_frame *)thread->sp;
#if USE_FPU			
			if (sp->flag)
				sp_addr = (uint32_t)sp + sizeof(struct stack_frame_fpu);
			else
#endif				
				sp_addr = (uint32_t)sp + sizeof(struct stack_frame);

			sp_len = (uint32_t)thread->stack_addr + thread->stack_size - sp_addr;
			DumpMem(sp_addr, sp_len);
		}
    }

    /* leave critical */
    rt_exit_critical();

}

#pragma section = ".data"
#pragma section = ".bss"

void DumpCore(struct    stack_frame *sp)
{
	uint32_t cur_sp;
	uint32_t cur_sp_len;

	rt_thread_t current_thread = rt_thread_self();
	
	/* 打印当前任务的寄存器 */
	DumpRegisters(sp, "current_thread");

	/* 打印当前任务的栈 */
	rt_kprintf("Stack segment:\n");
	
#if USE_FPU			
	if (sp->flag)
		cur_sp = (uint32_t)sp + sizeof(struct stack_frame_fpu);
	else
#endif		
		cur_sp = (uint32_t)sp + sizeof(struct stack_frame);
	
	cur_sp_len = (uint32_t)current_thread->stack_addr + current_thread->stack_size - cur_sp;
	DumpMem(cur_sp, cur_sp_len);

	/* 打印数据段 */
	rt_kprintf("Data segment:\n");
	DumpMem((uint32_t)__section_begin(".data"), (uint32_t)__section_end(".data") - (uint32_t)__section_begin(".data"));

	/* 打印ZI段 */
	rt_kprintf("ZI segment:\n");
	DumpMem((uint32_t)__section_begin(".bss"), (uint32_t)__section_end(".bss") - (uint32_t)__section_begin(".bss"));

	/* 打印其他任务的寄存器和栈 */
	DumpTasks();
}

void rt_hw_hard_fault_exception(struct exception_info *exception_info)
{
	DumpCore(&exception_info->stack_frame);
    while (1);
}

void coredump_cmd_asm(void);
MSH_CMD_EXPORT_ALIAS(coredump_cmd_asm, coredump, coredump from user);

