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

	SECTION    .text:CODE(2)
	THUMB
	REQUIRE8
	PRESERVE8
    
    IMPORT DumpCore
    EXPORT coredump_cmd_asm
coredump_cmd_asm:
	MRS      r0, xpsr
	PUSH {r0}
	ADR r0, .
	PUSH {r0}  /* pc */
	PUSH {r0-r3,r12,lr}
	PUSH {r4-r11}
#if defined ( __ARMVFP__ )
	MOV r0, #0
	PUSH {r0}  /* flag, 表示是否使用FPU,我们写入0表示不使用FPU */
#endif
	MOV r0, sp
	BL DumpCore
#if defined ( __ARMVFP__ )
	POP  {r0}  /* flag */
#endif
	POP {r4-r11}
	POP  {r0-r3,r12,lr}
	POP {r0} /* pc */
	POP {r0} /* xpsr */
        BX      LR

	IMPORT rt_hw_hard_fault_exception
	EXPORT HardFault_Handler
HardFault_Handler:

	; get current context
	MRS 	r0, msp 				; get fault context from handler.
	TST 	lr, #0x04				; if(!EXC_RETURN[2])
	BEQ 	_get_sp_done
	MRS 	r0, psp 				; get fault context from thread.
_get_sp_done

	STMFD	r0!, {r4 - r11} 		; push r4 - r11 register
	;STMFD	 r0!, {lr}				 ; push exec_return register
#if defined ( __ARMVFP__ )
	MOV  R1, #0   ; not use FPU
	TST  lr, #(1<<4)
	BNE  _set_flag_done
	MOV  R1, #1   ; use FPU
_set_flag_done	  
	SUB 	r0, r0, #0x04			; push dummy for flag
	STR 	R1, [r0]
#endif
	SUB 	r0, r0, #0x04
	STR 	lr, [r0]

	TST 	lr, #0x04				; if(!EXC_RETURN[2])
	BEQ 	_update_msp
	MSR 	psp, r0 				; update stack pointer to PSP.
	B		_update_done
_update_msp
	MSR 	msp, r0 				; update stack pointer to MSP.
_update_done

	PUSH	{lr}
	BL		rt_hw_hard_fault_exception
	POP 	{lr}

	ORR 	lr, lr, #0x04
	BX		lr
        
        END

