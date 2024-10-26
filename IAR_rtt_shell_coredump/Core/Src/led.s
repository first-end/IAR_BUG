                THUMB
                AREA    |.text|, CODE, READONLY

; Reset handler
led_blink    PROC
                 EXPORT  led_blink
			    DCW  0xffff
				LDR R2, =(0x40021414)
Loop				
				; set GPF9 output hight
				LDR R1, [R2]
				ORR R1, R1, #(1<<9)
				STR R1, [R2]
				
				LDR R0, =100000000
				BL mydelay
				
				; set GPF9 output low
				LDR R1, [R2]
				BIC R1, R1, #(1<<9)
				STR R1, [R2]
				
				LDR R0, =100000000
				BL mydelay
				
				B Loop
                ENDP

mydelay  PROC
				SUBS R0, R0, #1
				BNE mydelay
				BX LR
                ENDP
                
                END