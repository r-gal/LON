/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : boot.s
Purpose : Jumps to main application

*/
        .syntax unified  
//
// Declare a regular function.
// Functions from the startup are placed in the init section.
//
.macro START_FUNC Name
        .section .init.\Name, "ax"
        .global \Name
        .balign 2
        .thumb_func
        .code 16
\Name:
.endm

START_FUNC JumpToApplication
        ldr     LR, =0x00000000
        ldr     SP, [R0, #0]
        ldr     PC, [R0, #4]