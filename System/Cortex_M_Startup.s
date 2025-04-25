/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2014 - 2022 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* - Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File      : Cortex_M_Startup.s
Purpose   : Generic startup and exception handlers for Cortex-M devices.

Additional information:
  Preprocessor Definitions
    __NO_SYSTEM_INIT
      If defined, 
        SystemInit is not called.
      If not defined,
        SystemInit is called.
        SystemInit is usually supplied by the CMSIS files.
        This file declares a weak implementation as fallback.
        
    __SUPPORT_RESET_HALT_AFTER_BTL
      If != 0 (default)
        Support J-Link's reset strategy Reset and Halt After Bootloader.
        https://wiki.segger.com/Reset_and_Halt_After_Bootloader
      If == 0,
        Disable support for Reset and Halt After Bootloader.

    __SOFTFP__
      Defined by the build system.
      If not defined, the FPU is enabled for floating point operations.
*/

        .syntax unified  

        
#ifndef   __SUPPORT_RESET_HALT_AFTER_BTL
  #define __SUPPORT_RESET_HALT_AFTER_BTL  1
#endif
        
/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
//
// Just place a vector (word) in the table
//
.macro VECTOR Name=
        .section .vectors, "a"
        .word \Name
.endm
//
// Declare an interrupt handler
//
.macro ISR_HANDLER Name=
        //
        // Insert vector in vector table
        //
        .section .vectors, "a"
        .word \Name
        //
        // Insert dummy handler in init section
        //
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b   // Endless loop
        END_FUNC \Name
.endm

//
// Place a reserved vector in vector table
//
.macro ISR_RESERVED
        .section .vectors, "a"
        .word 0
.endm

//
// Mark the end of a function and calculate its size
//
.macro END_FUNC name
        .size \name,.-\name
.endm

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/
/*********************************************************************
*
*  Setup of the vector table and weak definition of interrupt handlers
*
*/
        .section .vectors, "a"
        .code 16
        .balign 4
        .global _vectors
_vectors:
        VECTOR __stack_end__
        VECTOR Reset_Handler
        ISR_HANDLER NMI_Handler
        VECTOR HardFault_Handler
        ISR_HANDLER MemManage_Handler 
        ISR_HANDLER BusFault_Handler
        ISR_HANDLER UsageFault_Handler
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_HANDLER SVC_Handler
        ISR_RESERVED // Populate if using a debug monitor
        ISR_RESERVED
        ISR_HANDLER PendSV_Handler
        ISR_HANDLER SysTick_Handler

              /* External Interrupts */
        ISR_HANDLER      WWDG_IRQHandler                   /* Window WatchDog              */
        ISR_HANDLER      PVD_AVD_IRQHandler                /* PVD/AVD through EXTI Line detection */
        ISR_HANDLER      TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
        ISR_HANDLER      RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
        ISR_HANDLER      FLASH_IRQHandler                  /* FLASH                        */
        ISR_HANDLER      RCC_IRQHandler                    /* RCC                          */
        ISR_HANDLER      EXTI0_IRQHandler                  /* EXTI Line0                   */
        ISR_HANDLER      EXTI1_IRQHandler                  /* EXTI Line1                   */
        ISR_HANDLER      EXTI2_IRQHandler                  /* EXTI Line2                   */
        ISR_HANDLER      EXTI3_IRQHandler                  /* EXTI Line3                   */
        ISR_HANDLER      EXTI4_IRQHandler                  /* EXTI Line4                   */
        ISR_HANDLER      DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */
        ISR_HANDLER      DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */
        ISR_HANDLER      DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */
        ISR_HANDLER      DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */
        ISR_HANDLER      DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */
        ISR_HANDLER      DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */
        ISR_HANDLER      DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */
        ISR_HANDLER      ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */
        ISR_HANDLER      FDCAN1_IT0_IRQHandler             /* FDCAN1 interrupt line 0      */
        ISR_HANDLER      FDCAN2_IT0_IRQHandler             /* FDCAN2 interrupt line 0      */
        ISR_HANDLER      FDCAN1_IT1_IRQHandler             /* FDCAN1 interrupt line 1      */
        ISR_HANDLER      FDCAN2_IT1_IRQHandler             /* FDCAN2 interrupt line 1      */
        ISR_HANDLER      EXTI9_5_IRQHandler                /* External Line[9:5]s          */
        ISR_HANDLER      TIM1_BRK_IRQHandler               /* TIM1 Break interrupt         */
        ISR_HANDLER      TIM1_UP_IRQHandler                /* TIM1 Update interrupt        */
        ISR_HANDLER      TIM1_TRG_COM_IRQHandler           /* TIM1 Trigger and Commutation interrupt */
        ISR_HANDLER      TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
        ISR_HANDLER      TIM2_IRQHandler                   /* TIM2                         */
        ISR_HANDLER      TIM3_IRQHandler                   /* TIM3                         */
        ISR_HANDLER      TIM4_IRQHandler                   /* TIM4                         */
        ISR_HANDLER      I2C1_EV_IRQHandler                /* I2C1 Event                   */
        ISR_HANDLER      I2C1_ER_IRQHandler                /* I2C1 Error                   */
        ISR_HANDLER      I2C2_EV_IRQHandler                /* I2C2 Event                   */
        ISR_HANDLER      I2C2_ER_IRQHandler                /* I2C2 Error                   */
        ISR_HANDLER      SPI1_IRQHandler                   /* SPI1                         */
        ISR_HANDLER      SPI2_IRQHandler                   /* SPI2                         */
        ISR_HANDLER      USART1_IRQHandler                 /* USART1                       */
        ISR_HANDLER      USART2_IRQHandler                 /* USART2                       */
        ISR_HANDLER      USART3_IRQHandler                 /* USART3                       */
        ISR_HANDLER      EXTI15_10_IRQHandler              /* External Line[15:10]s        */
        ISR_HANDLER      RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
        ISR_HANDLER      ExternalReserved0                  /* Reserved                     */
        ISR_HANDLER      TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
        ISR_HANDLER      TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
        ISR_HANDLER      TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
        ISR_HANDLER      TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
        ISR_HANDLER      DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
        ISR_HANDLER      FMC_IRQHandler                    /* FMC                          */
        ISR_HANDLER      SDMMC1_IRQHandler                 /* SDMMC1                       */
        ISR_HANDLER      TIM5_IRQHandler                   /* TIM5                         */
        ISR_HANDLER      SPI3_IRQHandler                   /* SPI3                         */
        ISR_HANDLER      UART4_IRQHandler                  /* UART4                        */
        ISR_HANDLER      UART5_IRQHandler                  /* UART5                        */
        ISR_HANDLER      TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
        ISR_HANDLER      TIM7_IRQHandler                   /* TIM7                         */
        ISR_HANDLER      DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
        ISR_HANDLER      DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
        ISR_HANDLER      DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
        ISR_HANDLER      DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
        ISR_HANDLER      DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
        ISR_HANDLER      ETH_IRQHandler                    /* Ethernet                     */
        ISR_HANDLER      ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
        ISR_HANDLER      FDCAN_CAL_IRQHandler              /* FDCAN calibration unit interrupt*/
        ISR_HANDLER      ExternalReserved1                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved2                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved3                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved4                 /* Reserved                     */
        ISR_HANDLER      DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
        ISR_HANDLER      DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
        ISR_HANDLER      DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
        ISR_HANDLER      USART6_IRQHandler                 /* USART6                       */
        ISR_HANDLER      I2C3_EV_IRQHandler                /* I2C3 event                   */
        ISR_HANDLER      I2C3_ER_IRQHandler                /* I2C3 error                   */
        ISR_HANDLER      OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
        ISR_HANDLER      OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
        ISR_HANDLER      OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
        ISR_HANDLER      OTG_HS_IRQHandler                 /* USB OTG HS                   */
        ISR_HANDLER      DCMI_PSSI_IRQHandler              /* DCMI, PSSI                   */
        ISR_HANDLER      ExternalReserved5                 /* Reserved                     */
        ISR_HANDLER      RNG_IRQHandler                    /* Rng                          */
        ISR_HANDLER      FPU_IRQHandler                    /* FPU                          */
        ISR_HANDLER      UART7_IRQHandler                  /* UART7                        */
        ISR_HANDLER      UART8_IRQHandler                  /* UART8                        */
        ISR_HANDLER      SPI4_IRQHandler                   /* SPI4                         */
        ISR_HANDLER      SPI5_IRQHandler                   /* SPI5                         */
        ISR_HANDLER      SPI6_IRQHandler                   /* SPI6                         */
        ISR_HANDLER      SAI1_IRQHandler                   /* SAI1                         */
        ISR_HANDLER      LTDC_IRQHandler                   /* LTDC                         */
        ISR_HANDLER      LTDC_ER_IRQHandler                /* LTDC error                   */
        ISR_HANDLER      DMA2D_IRQHandler                  /* DMA2D                        */
        ISR_HANDLER      ExternalReserved6                 /* Reserved                     */
        ISR_HANDLER      OCTOSPI1_IRQHandler               /* OCTOSPI1                     */
        ISR_HANDLER      LPTIM1_IRQHandler                 /* LPTIM1                       */
        ISR_HANDLER      CEC_IRQHandler                    /* HDMI_CEC                     */
        ISR_HANDLER      I2C4_EV_IRQHandler                /* I2C4 Event                   */
        ISR_HANDLER      I2C4_ER_IRQHandler                /* I2C4 Error                   */
        ISR_HANDLER      SPDIF_RX_IRQHandler               /* SPDIF_RX                     */
        ISR_HANDLER      ExternalReserved7                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved8                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved9                 /* Reserved                     */
        ISR_HANDLER      ExternalReserved10                /* Reserved                     */
        ISR_HANDLER      DMAMUX1_OVR_IRQHandler            /* DMAMUX1 Overrun interrupt    */
        ISR_HANDLER      ExternalReserved11                /* Reserved                     */
        ISR_HANDLER      ExternalReserved12                /* Reserved                     */
        ISR_HANDLER      ExternalReserved13                /* Reserved                     */
        ISR_HANDLER      ExternalReserved14                /* Reserved                     */
        ISR_HANDLER      ExternalReserved15                /* Reserved                     */
        ISR_HANDLER      ExternalReserved16                /* Reserved                     */
        ISR_HANDLER      ExternalReserved17                /* Reserved                     */
        ISR_HANDLER      DFSDM1_FLT0_IRQHandler            /* DFSDM Filter0 Interrupt      */
        ISR_HANDLER      DFSDM1_FLT1_IRQHandler            /* DFSDM Filter1 Interrupt      */
        ISR_HANDLER      DFSDM1_FLT2_IRQHandler            /* DFSDM Filter2 Interrupt      */
        ISR_HANDLER      DFSDM1_FLT3_IRQHandler            /* DFSDM Filter3 Interrupt      */
        ISR_HANDLER      ExternalReserved18                 /* Reserved                     */
        ISR_HANDLER      SWPMI1_IRQHandler                 /* Serial Wire Interface 1 global interrupt */
        ISR_HANDLER      TIM15_IRQHandler                  /* TIM15 global Interrupt          */
        ISR_HANDLER      TIM16_IRQHandler                  /* TIM16 global Interrupt          */
        ISR_HANDLER      TIM17_IRQHandler                  /* TIM17 global Interrupt          */
        ISR_HANDLER      MDIOS_WKUP_IRQHandler             /* MDIOS Wakeup  Interrupt         */
        ISR_HANDLER      MDIOS_IRQHandler                  /* MDIOS global Interrupt          */
        ISR_HANDLER      ExternalReserved19                /* Reserved                        */
        ISR_HANDLER      MDMA_IRQHandler                   /* MDMA global Interrupt           */
        ISR_HANDLER      ExternalReserved20                /* Reserved                        */
        ISR_HANDLER      SDMMC2_IRQHandler                 /* SDMMC2 global Interrupt         */
        ISR_HANDLER      HSEM1_IRQHandler                  /* HSEM1 global Interrupt          */
        ISR_HANDLER      ExternalReserved21                /* Reserved                        */
        ISR_HANDLER      ADC3_IRQHandler                   /* ADC3 global Interrupt           */
        ISR_HANDLER      DMAMUX2_OVR_IRQHandler            /* DMAMUX Overrun interrupt        */
        ISR_HANDLER      BDMA_Channel0_IRQHandler          /* BDMA Channel 0 global Interrupt */
        ISR_HANDLER      BDMA_Channel1_IRQHandler          /* BDMA Channel 1 global Interrupt */
        ISR_HANDLER      BDMA_Channel2_IRQHandler          /* BDMA Channel 2 global Interrupt */
        ISR_HANDLER      BDMA_Channel3_IRQHandler          /* BDMA Channel 3 global Interrupt */
        ISR_HANDLER      BDMA_Channel4_IRQHandler          /* BDMA Channel 4 global Interrupt */
        ISR_HANDLER      BDMA_Channel5_IRQHandler          /* BDMA Channel 5 global Interrupt */
        ISR_HANDLER      BDMA_Channel6_IRQHandler          /* BDMA Channel 6 global Interrupt */
        ISR_HANDLER      BDMA_Channel7_IRQHandler          /* BDMA Channel 7 global Interrupt */
        ISR_HANDLER      COMP1_IRQHandler                  /* COMP1 global Interrupt          */
        ISR_HANDLER      LPTIM2_IRQHandler                 /* LP TIM2 global interrupt        */
        ISR_HANDLER      LPTIM3_IRQHandler                 /* LP TIM3 global interrupt        */
        ISR_HANDLER      LPTIM4_IRQHandler                 /* LP TIM4 global interrupt        */
        ISR_HANDLER      LPTIM5_IRQHandler                 /* LP TIM5 global interrupt        */
        ISR_HANDLER      LPUART1_IRQHandler                /* LP UART1 interrupt              */
        ISR_HANDLER      ExternalReserved22                /* Reserved                        */
        ISR_HANDLER      CRS_IRQHandler                    /* Clock Recovery Global Interrupt */
        ISR_HANDLER      ECC_IRQHandler                    /* ECC diagnostic Global Interrupt */
        ISR_HANDLER      SAI4_IRQHandler                   /* SAI4 global interrupt           */
        ISR_HANDLER      DTS_IRQHandler                    /* Digital Temperature Sensor  interrupt */
        ISR_HANDLER      ExternalReserved23                /* Reserved                              */
        ISR_HANDLER      WAKEUP_PIN_IRQHandler             /* Interrupt for all 6 wake-up pins      */
        ISR_HANDLER      OCTOSPI2_IRQHandler               /* OCTOSPI2 Interrupt       */
        ISR_HANDLER      ExternalReserved24                /* Reserved                 */
        ISR_HANDLER      ExternalReserved25                /* Reserved                 */
        ISR_HANDLER      FMAC_IRQHandler                   /* FMAC Interrupt           */
        ISR_HANDLER      CORDIC_IRQHandler                 /* CORDIC Interrupt         */
        ISR_HANDLER      UART9_IRQHandler                  /* UART9 Interrupt          */
        ISR_HANDLER      USART10_IRQHandler                /* UART10 Interrupt         */
        ISR_HANDLER      I2C5_EV_IRQHandler                /* I2C5 Event Interrupt     */
        ISR_HANDLER      I2C5_ER_IRQHandler                /* I2C5 Error Interrupt     */
        ISR_HANDLER      FDCAN3_IT0_IRQHandler             /* FDCAN3 interrupt line 0  */
        ISR_HANDLER      FDCAN3_IT1_IRQHandler             /* FDCAN3 interrupt line 1  */
        ISR_HANDLER      TIM23_IRQHandler                  /* TIM23 global interrupt   */
        ISR_HANDLER      TIM24_IRQHandler                  /* TIM24 global interrupt   */
        //
        // Add external interrupt vectors here.
        // Example:
        //   ISR_HANDLER ExternalISR0
        //   ISR_HANDLER ExternalISR1
        //   ISR_HANDLER ExternalISR2
        //   ISR_HANDLER ExternalISR3
        //

        .section .vectors, "a"
        .size _vectors, .-_vectors
_vectors_end:

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
/*********************************************************************
*
*       Reset_Handler
*
*  Function description
*    Exception handler for reset.
*    Generic bringup of a Cortex-M system.
*
*  Additional information
*    The stack pointer is expected to be initialized by hardware,
*    i.e. read from vectortable[0].
*    For manual initialization add
*      ldr R0, =__stack_end__
*      mov SP, R0
*/
        .global reset_handler
        .global Reset_Handler
        .equ reset_handler, Reset_Handler
        .section .init.Reset_Handler, "ax"
        .balign 2
        .thumb_func
Reset_Handler:
#if __SUPPORT_RESET_HALT_AFTER_BTL != 0
        //
        // Perform a dummy read access from address 0x00000008 followed by two nop's
        // This is needed to support J-Links reset strategy: Reset and Halt After Bootloader.
        // https://wiki.segger.com/Reset_and_Halt_After_Bootloader
        //
        movs    R0, #8
        ldr     R0, [R0]
        nop
        nop
#endif
#ifndef __NO_SYSTEM_INIT
        //
        // Call SystemInit
        //
        bl      SystemInit
#endif
#if !defined(__SOFTFP__)
        //
        // Enable CP11 and CP10 with CPACR |= (0xf<<20)
        //
        movw    R0, 0xED88
        movt    R0, 0xE000
        ldr     R1, [R0]
        orrs    R1, R1, #(0xf << 20)
        str     R1, [R0]
#endif
        //
        // Call runtime initialization, which calls main().
        //
        bl      _start
END_FUNC Reset_Handler

        //
        // Weak only declaration of SystemInit enables Linker to replace bl SystemInit with a NOP,
        // when there is no strong definition of SystemInit.
        //
        .weak SystemInit

/*********************************************************************
*
*       HardFault_Handler
*
*  Function description
*    Simple exception handler for HardFault.
*    In case of a HardFault caused by BKPT instruction without 
*    debugger attached, return execution, otherwise stay in loop.
*
*  Additional information
*    The stack pointer is expected to be initialized by hardware,
*    i.e. read from vectortable[0].
*    For manual initialization add
*      ldr R0, =__stack_end__
*      mov SP, R0
*/

#undef L
#define L(label) .LHardFault_Handler_##label

        .weak HardFault_Handler
        .section .init.HardFault_Handler, "ax"
        .balign 2
        .thumb_func
HardFault_Handler:
        //
        // Check if HardFault is caused by BKPT instruction
        //
        ldr     R1, =0xE000ED2C         // Load NVIC_HFSR
        ldr     R2, [R1]
        cmp     R2, #0                  // Check NVIC_HFSR[31]

L(hfLoop):
        bmi     L(hfLoop)               // Not set? Stay in HardFault Handler.
        //
        // Continue execution after BKPT instruction
        //
#if defined(__thumb__) && !defined(__thumb2__)
        movs    R0, #4
        mov     R1, LR
        tst     R0, R1                  // Check EXC_RETURN in Link register bit 2.
        bne     L(Uses_PSP)
        mrs     R0, MSP                 // Stacking was using MSP.
        b       L(Pass_StackPtr)
L(Uses_PSP):
        mrs     R0, PSP                 // Stacking was using PSP.
L(Pass_StackPtr):
#else
        tst     LR, #4                  // Check EXC_RETURN[2] in link register to get the return stack
        ite     eq
        mrseq   R0, MSP                 // Frame stored on MSP
        mrsne   R0, PSP                 // Frame stored on PSP
#endif
        //
        // Reset HardFault Status
        //
#if defined(__thumb__) && !defined(__thumb2__)
        movs    R3, #1
        lsls    R3, R3, #31
        orrs    R2, R3
        str     R2, [R1]
#else
        orr R2, R2, #0x80000000
        str R2, [R1]
#endif
        //
        // Adjust return address
        //
        ldr     R1, [R0, #24]           // Get stored PC from stack
        adds    R1, #2                  // Adjust PC by 2 to skip current BKPT
        str     R1, [R0, #24]           // Write back adjusted PC to stack
        //
        bx      LR                      // Return
END_FUNC HardFault_Handler

/*************************** End of file ****************************/
