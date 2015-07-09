/** ###################################################################
**      Filename    : Vectors_Config.h
**      Processor   : MKV10Z16VFM7
**      Version     : 1.00
**      Abstract    :
**
** 
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file Vectors_Config.h                                                  
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup Vectors_Config_module Vectors_Config module documentation
**  @{
*/         
    
#include "PE_Types.h"
#include "Init_Config.h"
#include "Vectors_Config.h"

#if PEX_VECTOR_TABLE /* Is Vector table handled by Processor Expert? */
  
/*
** ===================================================================
**     Method      :  UnhandledInterrupt (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled common interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(UnhandledInterrupt)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_NMI (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_NMI interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_NMI)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Hard_Fault (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Hard_Fault interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Hard_Fault)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved4 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved4 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved4)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved5 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved5 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved5)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved6 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved6 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved6)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved7 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved7 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved7)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved8 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved8 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved8)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved9 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved9 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved9)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved10 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved10 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved10)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_SVCall (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_SVCall interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_SVCall)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved12 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved12 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved12)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved13 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved13 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved13)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_PendableSrvReq (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_PendableSrvReq interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_PendableSrvReq)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_SysTick (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_SysTick interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_SysTick)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DMA0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DMA0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DMA0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DMA1 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DMA1 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DMA1)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DMA2 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DMA2 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DMA2)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DMA3 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DMA3 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DMA3)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DMA_Error (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DMA_Error interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DMA_Error)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_FTFA (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_FTFA interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_FTFA)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_PMC (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_PMC interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_PMC)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_LLWU (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_LLWU interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_LLWU)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_I2C0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_I2C0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_I2C0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved25 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved25 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved25)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_SPI0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_SPI0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_SPI0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved27 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved27 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved27)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_UART0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_UART0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_UART0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_UART1 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_UART1 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_UART1)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved30 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved30 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved30)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_ADC0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_ADC0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_ADC0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_ADC1 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_ADC1 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_ADC1)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_FTM0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_FTM0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_FTM0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_FTM1 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_FTM1 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_FTM1)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_FTM2 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_FTM2 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_FTM2)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_CMP0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_CMP0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_CMP0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_CMP1 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_CMP1 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_CMP1)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved38 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved38 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved38)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_WDOG_EWM (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_WDOG_EWM interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_WDOG_EWM)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved40 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved40 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved40)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_DAC0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_DAC0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_DAC0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_Reserved42 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_Reserved42 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_Reserved42)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_MCG (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_MCG interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_MCG)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_LPTMR0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_LPTMR0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_LPTMR0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_PDB0 (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_PDB0 interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_PDB0)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_PORTA (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_PORTA interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_PORTA)
{
  PE_DEBUGHALT();
}

/*
** ===================================================================
**     Method      :  Unhandled_ivINT_PORTBCDE (component MKV10Z16FM7)
**
**     Description :
**         This ISR services the unhandled ivINT_PORTBCDE interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(Unhandled_ivINT_PORTBCDE)
{
  PE_DEBUGHALT();
}


/* ISR prototype */
#if defined(__IAR_SYSTEMS_ICC__)
#ifdef __cplusplus
extern "C" void __iar_program_start( void );
#else
extern void __iar_program_start( void );
#endif

/* Referencing segment CSTACK in order to correctly initialize interrupt vector table. */
#pragma language=extended
#pragma segment="CSTACK"

/*lint -esym(765,__vector_table) Disable MISRA rule (8.10) checking for symbols (__vector_table). Definition of the interrupt vector table placed by linker on a predefined location. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. Need to explicitly cast pointers to the general ISR for Interrupt vector table */
/*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. Need to disable due to usage of @ operator in IAR compiler */

const tVectorTable __vector_table @ ".intvec" = {

#elif defined(__CWCC__)
#ifdef __cplusplus
extern "C" void __thumb_startup( void );
#else
extern void __thumb_startup( void );
#endif

extern uint32_t __SP_INIT[];

/* Pragma to place the interrupt vector table on correct location defined in linker file. */
#pragma define_section vectortable ".vectortable" ".vectortable" ".vectortable" far_abs R

/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. Need to explicitly cast pointers to the general ISR for Interrupt vector table */

__declspec(vectortable) const tVectorTable __vect_table = { /* Interrupt vector table */

#elif defined(__GNUC__)
#ifdef __cplusplus
extern "C" void __thumb_startup( void );
#else
extern void __thumb_startup( void );
#endif

extern uint32_t __SP_INIT;

/*lint -esym(765,__vect_table) Disable MISRA rule (8.10) checking for symbols (__vect_table). Definition of the interrupt vector table placed by linker on a predefined location. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. Need to explicitly cast pointers to the general ISR for Interrupt vector table */

__attribute__ ((section (".vectortable"))) const tVectorTable __vect_table = { /* Interrupt vector table */

#elif defined(__CC_ARM)
#ifdef __cplusplus
extern "C" void __thumb_startup( void );
#else
extern void __thumb_startup( void );
#endif

extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;

/*lint -esym(765,__vect_table) Disable MISRA rule (8.10) checking for symbols (__vect_table). Definition of the interrupt vector table placed by linker on a predefined location. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. Need to explicitly cast pointers to the general ISR for Interrupt vector table */

__attribute__ ((section (".vectortable"))) const tVectorTable __vect_table __attribute__((used)) = { /* Interrupt vector table */

#else /* defined(compiler ID) */
  #error Unsupported compiler
#endif /* defined(compiler ID) */

  /* ISR address                          No.  Name */
  VECTOR_SP_MAIN,                      /* 0x00 ivINT_Initial_Stack_Pointer */
  {
  VECTOR_1,                            /* 0x01 ivINT_Initial_Program_Counter */
  VECTOR_2,                            /* 0x02 ivINT_NMI */
  VECTOR_3,                            /* 0x03 ivINT_Hard_Fault */
  VECTOR_4,                            /* 0x04 ivINT_Reserved4 */
  VECTOR_5,                            /* 0x05 ivINT_Reserved5 */
  VECTOR_6,                            /* 0x06 ivINT_Reserved6 */
  VECTOR_7,                            /* 0x07 ivINT_Reserved7 */
  VECTOR_8,                            /* 0x08 ivINT_Reserved8 */
  VECTOR_9,                            /* 0x09 ivINT_Reserved9 */
  VECTOR_10,                           /* 0x0A ivINT_Reserved10 */
  VECTOR_11,                           /* 0x0B ivINT_SVCall */
  VECTOR_12,                           /* 0x0C ivINT_Reserved12 */
  VECTOR_13,                           /* 0x0D ivINT_Reserved13 */
  VECTOR_14,                           /* 0x0E ivINT_PendableSrvReq */
  VECTOR_15,                           /* 0x0F ivINT_SysTick */
  VECTOR_16,                           /* 0x10 ivINT_DMA0 */
  VECTOR_17,                           /* 0x11 ivINT_DMA1 */
  VECTOR_18,                           /* 0x12 ivINT_DMA2 */
  VECTOR_19,                           /* 0x13 ivINT_DMA3 */
  VECTOR_20,                           /* 0x14 ivINT_DMA_Error */
  VECTOR_21,                           /* 0x15 ivINT_FTFA */
  VECTOR_22,                           /* 0x16 ivINT_PMC */
  VECTOR_23,                           /* 0x17 ivINT_LLWU */
  VECTOR_24,                           /* 0x18 ivINT_I2C0 */
  VECTOR_25,                           /* 0x19 ivINT_Reserved25 */
  VECTOR_26,                           /* 0x1A ivINT_SPI0 */
  VECTOR_27,                           /* 0x1B ivINT_Reserved27 */
  VECTOR_28,                           /* 0x1C ivINT_UART0 */
  VECTOR_29,                           /* 0x1D ivINT_UART1 */
  VECTOR_30,                           /* 0x1E ivINT_Reserved30 */
  VECTOR_31,                           /* 0x1F ivINT_ADC0 */
  VECTOR_32,                           /* 0x20 ivINT_ADC1 */
  VECTOR_33,                           /* 0x21 ivINT_FTM0 */
  VECTOR_34,                           /* 0x22 ivINT_FTM1 */
  VECTOR_35,                           /* 0x23 ivINT_FTM2 */
  VECTOR_36,                           /* 0x24 ivINT_CMP0 */
  VECTOR_37,                           /* 0x25 ivINT_CMP1 */
  VECTOR_38,                           /* 0x26 ivINT_Reserved38 */
  VECTOR_39,                           /* 0x27 ivINT_WDOG_EWM */
  VECTOR_40,                           /* 0x28 ivINT_Reserved40 */
  VECTOR_41,                           /* 0x29 ivINT_DAC0 */
  VECTOR_42,                           /* 0x2A ivINT_Reserved42 */
  VECTOR_43,                           /* 0x2B ivINT_MCG */
  VECTOR_44,                           /* 0x2C ivINT_LPTMR0 */
  VECTOR_45,                           /* 0x2D ivINT_PDB0 */
  VECTOR_46,                           /* 0x2E ivINT_PORTA */
  VECTOR_47                            /* 0x2F ivINT_PORTBCDE */
  }
};
#if defined(__IAR_SYSTEMS_ICC__)  
/*lint -restore Enable MISRA rule (11.4,2.1,1.1) checking. */
#else
/*lint -restore Enable MISRA rule (11.4) checking. */
#endif

#endif /* PEX_VECTOR_TABLE */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file is a part of Processor Expert static initialization
**     library for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/