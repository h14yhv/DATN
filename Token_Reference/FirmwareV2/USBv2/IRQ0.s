;/*****************************************************************************/
;/* IRQ0.S: IRQ 0 Nested Interrupt                                            */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

                AREA    IRQ0, CODE, READONLY
                ARM

                PRESERVE8
                ALIGN
                IMPORT  irq0_nint
                EXPORT  irq0_int
irq0_int        FUNCTION

                SUB     LR, LR, #4             ; Update Link Register
                STMFD   SP!, {R0-R12, LR}      ; Save Workspace & LR to Stack
                MRS     R0, SPSR               ; Copy SPSR to R0
                STMFD   SP!, {R0, R1}          ; Save SPSR to Stack (8-byte)
                MSR     CPSR_c, #0x1F          ; Enable IRQ (Sys Mode)
                STMFD   SP!, {R0, LR}          ; Save LR_sys to Stack (8-byte)
                LDR     R0, =irq0_nint         ; Get IRQ Function Address
                MOV     LR, PC                 ; Return Address
                BX      R0                     ; Call IRQ Function
                LDMFD   SP!, {R0, LR}          ; Restore LR_sys
                MSR     CPSR_c, #0x92          ; Disable IRQ (IRQ Mode)
                LDMFD   SP!, {R0, R1}          ; Restore SPSR to R0
                MSR     SPSR_cxsf, R0          ; Copy R0 to SPSR
                LDMFD   SP!, {R0-R12, PC}^     ; Return to program

                ENDFUNC

                END
