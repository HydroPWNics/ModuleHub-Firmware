/** ###################################################################
**      Filename    : CPU_Init.c
**      Processor   : MKV10Z16VFM7
**      Version     : 1.2
**      Abstract    :
**         This file implements the MCU initialization and CPU component runtime methods.
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
** @file CPU_Init.c                                                  
** @version 01.00
** @brief
**         This file implements the MCU initialization and CPU component runtime methods.
*/         
/*!
**  @addtogroup CPU_Init_module CPU_Init module documentation
**  @{
*/         

/* MODULE CPU_Init.c */

#include "PE_Types.h"
#include "MKV10Z7.h"
#include "Init_Config.h"
#include "Peripherals_Init.h"
#include "CPU_Init.h"
#include "Events.h"
#if CPU_OPEN_BACK_DOOR
#include "FTFA_PDD.h"
#endif /* CPU_OPEN_BACK_DOOR */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CPU_FLASH_CONFIG_FIELD
/* Flash configuration field */
  #if defined(__CWCC__)
/* Pragma to place the flash configuration field on correct location defined in linker file. */
#pragma define_section cfmconfig ".cfmconfig" ".cfmconfig" ".cfmconfig" far_abs R

static __declspec(cfmconfig) uint8_t _cfm[0x10] = {CPU_FLASH_CONFIG_FIELD};
  #elif defined(__IAR_SYSTEMS_ICC__)    
/* Pragma to place the flash configuration field on correct location defined in linker file. */
#pragma language=extended
#pragma location = "FlashConfig"

__root const uint8_t Config[0x10] @ "FlashConfig" = {CPU_FLASH_CONFIG_FIELD};
  #elif defined(__GNUC__)
__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10] = {CPU_FLASH_CONFIG_FIELD};
  #elif defined(__CC_ARM)
__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10] __attribute__((used)) = {CPU_FLASH_CONFIG_FIELD};
  #else
    #error Unsupported compiler!
  #endif
#endif    

extern void Common_Init(void);         /* Forward declaration of generated common initialization in generated CPU module */
extern void Components_Init(void);     /* Forward declaration of generated LDD components initialization in generated CPU module */
extern void LDD_SetClockConfiguration(LDD_TClockConfiguration ClockConfiguration); 
  
/* Global variables */

volatile uint8_t SR_reg;               /* Current value of the FAULTMASK register */
volatile uint8_t SR_lock = 0x00U;      /* Lock */
LDD_TClockConfiguration ClockConfigurationID = CPU_CLOCK_CONFIG_0; /* Active clock configuration */

static const CPU_TClockGenMode ClockGenModeMatrix[6][6] = {
/* This matrix defines which mode is next in the MCG Mode state diagram in transitioning from the
   current mode to a target mode*/
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FEI */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_BLPI, CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FBI */
  { CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_BLPI, CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI  }, /* BLPI */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE  }, /* FEE */
  { CPU_MCG_MODE_FEI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FBI,  CPU_MCG_MODE_FEE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_BLPE }, /* FBE */
  { CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_FBE,  CPU_MCG_MODE_BLPE }  /* BLPE */
};

static const CPU_TClockConfigDescriptor CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_NUMBER] = {
  #if defined(CPU_CLOCK_CONFIG_0)
  /* Clock configuration 0 */
  {
    CPU_MCG_MODE_CONFIG_0,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_0,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_0,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_0,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_0,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_0,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_0,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_0,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_0,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_0,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_0,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_0            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_1)
  /* Clock configuration 1 */
  ,{
    CPU_MCG_MODE_CONFIG_1,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_1,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_1,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_1,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_1,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_1,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_1,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_1,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_1,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_1,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_1,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_1            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_2)
  /* Clock configuration 2 */
  ,{
    CPU_MCG_MODE_CONFIG_2,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_2,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_2,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_2,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_2,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_2,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_2,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_2,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_2,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_2,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_2,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_2            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_3)
  /* Clock configuration 3 */
  ,{
    CPU_MCG_MODE_CONFIG_3,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_3,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_3,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_3,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_3,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_3,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_3,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_3,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_3,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_3,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_3,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_3            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_4)
  /* Clock configuration 4 */
  ,{
    CPU_MCG_MODE_CONFIG_4,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_4,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_4,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_4,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_4,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_4,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_4,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_4,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_4,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_4,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_4,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_4            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_5)
  /* Clock configuration 5 */
  ,{
    CPU_MCG_MODE_CONFIG_5,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_5,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_5,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_5,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_5,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_5,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_5,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_5,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_5,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_5,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_5,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_5            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_6)
  /* Clock configuration 6 */
  ,{
    CPU_MCG_MODE_CONFIG_6,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_6,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_6,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_6,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_6,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_6,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_6,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_6,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_6,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_6,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_6,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_6            /* Bus frequency */
  }
  #endif
  #if defined(CPU_CLOCK_CONFIG_7)
  /* Clock configuration 7 */
  ,{
    CPU_MCG_MODE_CONFIG_7,             /* Clock generator mode */
    CPU_CLOCK_VLP_CONFIG_7,            /* Very low power mode enable/disable */
    {
      CPU_MCG_C1_CONFIG_7,             /* MCG_C1 */
      CPU_MCG_C2_CONFIG_7,             /* MCG_C2 */
      CPU_MCG_C4_CONFIG_7,             /* MCG_C4 */
      CPU_MCG_C6_CONFIG_7,             /* MCG_C6 */
      CPU_OSC0_CR_CONFIG_7,            /* OSC0_CR */
    },
    {
      CPU_SIM_SOPT1_CONFIG_7,          /* SIM_SOPT1 */
      CPU_SIM_SOPT2_CONFIG_7,          /* SIM_SOPT2 */
      CPU_SIM_SOPT7_CONFIG_7,          /* SIM_SOPT7 */
      CPU_SIM_CLKDIV1_CONFIG_7,        /* SIM_CLKDIV1 */
    },
    CPU_BUS_CLK_HZ_CONFIG_7            /* Bus frequency */
  }
  #endif
};

static const CPU_TClockGenRegs CPU_DefaultFEI = {
  CPU_DEFAULT_FEI_MCG_C1,
  CPU_DEFAULT_FEI_MCG_C2,
  CPU_DEFAULT_FEI_MCG_C4,
  CPU_DEFAULT_FEI_MCG_C6,
  CPU_DEFAULT_FEI_OSC0_CR
};
static const CPU_TClockGenRegs CPU_DefaultFBI = {
  CPU_DEFAULT_FBI_MCG_C1,
  CPU_DEFAULT_FBI_MCG_C2,
  CPU_DEFAULT_FBI_MCG_C4,
  CPU_DEFAULT_FBI_MCG_C6,
  CPU_DEFAULT_FBI_OSC0_CR
};
static const CPU_TClockGenRegs CPU_DefaultBLPI = {
  CPU_DEFAULT_BLPI_MCG_C1,
  CPU_DEFAULT_BLPI_MCG_C2,
  CPU_DEFAULT_BLPI_MCG_C4,
  CPU_DEFAULT_BLPI_MCG_C6,
  CPU_DEFAULT_BLPI_OSC0_CR
};
static const CPU_TClockGenRegs CPU_DefaultFEE = {
  CPU_DEFAULT_FEE_MCG_C1,
  CPU_DEFAULT_FEE_MCG_C2,
  CPU_DEFAULT_FEE_MCG_C4,
  CPU_DEFAULT_FEE_MCG_C6,
  CPU_DEFAULT_FEE_OSC0_CR
};
static const CPU_TClockGenRegs CPU_DefaultFBE = {
  CPU_DEFAULT_FBE_MCG_C1,
  CPU_DEFAULT_FBE_MCG_C2,
  CPU_DEFAULT_FBE_MCG_C4,
  CPU_DEFAULT_FBE_MCG_C6,
  CPU_DEFAULT_FBE_OSC0_CR
};
static const CPU_TClockGenRegs CPU_DefaultBLPE = {
  CPU_DEFAULT_BLPE_MCG_C1,
  CPU_DEFAULT_BLPE_MCG_C2,
  CPU_DEFAULT_BLPE_MCG_C4,
  CPU_DEFAULT_BLPE_MCG_C6,
  CPU_DEFAULT_BLPE_OSC0_CR
};

static const CPU_TClockGenRegs *CPU_ClockGenModeRegs[CPU_MCG_MODES] = {
  &CPU_DefaultFEI,
  &CPU_DefaultFBI,
  &CPU_DefaultBLPI,
  &CPU_DefaultFEE,
  &CPU_DefaultFBE,
  &CPU_DefaultBLPE
};

#if CPU_OPEN_BACK_DOOR
/* FTFA start command and wait for command done routine code */
static const uint16_t FTFA_StartCmdAndWaitForCmdDoneRoutine[] = {
  /* FTFA_PDD_StartCmd(FTFA_BASE_PTR)                               */
  0xb500U,          /*  push            {lr}                        */
  0x4a04U,          /*  ldr             r2, [pc, #16]       ; (bc <FSTAT_RegAddress) */
  0x2380U,          /*  movs            r3, #128            ; 0x80  */
  0x7013U,          /*  strb            r3, [r2, #0]                */
  /* while(!FTFA_PDD_GetCmdCompleteFlag(FTFL_BASE_PTR)){}           */
  /* <LoadFlags>:                                                   */
  0x7811U,          /*  ldrb            r1, [r2, #0]                */
  0xb248U,          /*  sxtb            r0, r1                      */
  0x2800U,          /*  cmp             r0, #0                      */
  0xdafbU,          /*  bge.n           b0 <LoadFlags>              */
  0xbd00U,          /*  pop             {pc}                        */
  0x46c0U,          /*  nop             (mov r8, r8)                */
  /* <FSTAT_RegAddress>: */
  (uint16_t)((((uint32_t)&FTFA_FSTAT_REG(FTFA_BASE_PTR))>>0) & 0xFFFFU),
  (uint16_t)((((uint32_t)&FTFA_FSTAT_REG(FTFA_BASE_PTR))>>16) & 0xFFFFU)
};

typedef struct{
  uint8_t SafeRoutine[sizeof(FTFA_StartCmdAndWaitForCmdDoneRoutine)];
} FTFA_TStartCmdAndWaitForCmdRoutine;

typedef void (* FTFA_TSafeRoutinePtr)(void); /* Safe routine pointer type */
#endif /* CPU_OPEN_BACK_DOOR */

/*
** ===================================================================
**     Method      :  __init_hardware (component MKV10Z16FM7)
**
**     Description :
**         This method is called from the _startup() function and 
**         provides necessary system initialization such as PLL, and 
**         external bus.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#if defined(__IAR_SYSTEMS_ICC__)
int __low_level_init()
#elif defined(__CWCC__) | defined(__GNUC__) | defined(__CC_ARM)
void __init_hardware()
#endif
{
  /* Interrupt vector placement initialization */
  #if defined(__IAR_SYSTEMS_ICC__)
  SCB_VTOR = (uint32_t)(&__vector_table); /* Set the interrupt vector table position */
  #elif defined(__CWCC__) | defined(__GNUC__) | defined(__CC_ARM)
  SCB_VTOR = (uint32_t)(&__vect_table); /* Set the interrupt vector table position */
  #endif

  /* Disable the WDOG module */
  #if STARTUP_WDOG
  WDOG_UNLOCK = (uint16_t)STARTUP_WDOG_KEY_1; /* Key 1 */
  WDOG_UNLOCK = (uint16_t)STARTUP_WDOG_KEY_2; /* Key 2 */
  WDOG_STCTRLH = (uint16_t)STARTUP_WDOG_STCTRLH_VALUE; /* Disable WDOG */
  #endif /* STARTUP_WDOG */
  
  /* System clock initialization */
  
  /* Internal reference clock trim initialization */
  #if (defined(STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS) || defined(STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS))
    #if defined(STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS)
  if ( *((uint8_t*)STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS) != 0xFFU) { /* Skip if non-volatile flash memory is erased */
    MCG_C3 = *((uint8_t*)STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS);
    #endif /* defined(STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS) */
    #if defined(STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS)
      #if defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)
        #if (STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS == STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS)
    MCG_C4 = (MCG_C4 & ~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)) |
             ((*((uint8_t*) STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)) & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK));
        #else /* (STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS == STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS) */
    MCG_C4 = (MCG_C4 & ~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)) | 
             ((*((uint8_t*) STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)) & MCG_C4_FCTRIM_MASK) | 
             ((*((uint8_t*) STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS)) & MCG_C4_SCFTRIM_MASK);
        #endif /* (STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS == STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS) */
        #if defined(STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS)
    MCG_C2 = (MCG_C2 & ~(MCG_C2_FCFTRIM_MASK)) | ((*((uint8_t*)STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)) & MCG_C2_FCFTRIM_MASK);
        #endif /* defined(STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS) */
      #else /* defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) */
    MCG_C4 = (MCG_C4 & ~(MCG_C4_SCFTRIM_MASK)) | ((*((uint8_t*)STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS)) & MCG_C4_SCFTRIM_MASK);
      #endif /* defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) */
    #endif /* STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS */
    #if defined(STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS)
  }
    #endif /* defined(STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS) */
  #elif (defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) || defined(STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS))
    #if defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)
  if ( *((uint8_t*)STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) != 0xFFU) { /* Skip if non-volatile flash memory is erased */
    MCG_C4 = (MCG_C4 & ~(MCG_C4_FCTRIM_MASK)) | ((*((uint8_t*)STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)) & MCG_C4_FCTRIM_MASK);
    #endif /* defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) */
    #if defined(STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS)
    MCG_C2 = (MCG_C2 & ~(MCG_C2_FCFTRIM_MASK)) | ((*((uint8_t*)STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS)) & MCG_C2_FCFTRIM_MASK);
    #endif /* defined(STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS) */
    #if defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS)
  }
    #endif /* defined(STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS) */
  #endif /* STARTUP_CLOCK_INTERNAL_FAST_TRIM */
  /* SIM_CLKDIV1: OUTDIV1=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=3,OUTDIV5EN=1,OUTDIV5=4,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x01) |
                SIM_CLKDIV1_OUTDIV4(0x03) |
                SIM_CLKDIV1_OUTDIV5EN_MASK |
                SIM_CLKDIV1_OUTDIV5(0x04); /* Set the system prescalers to safe value */
  CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_0);
  SIM_CLKDIV1 = (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_CLKDIV1_value; /* Update system prescalers */
  SIM_SOPT1 = (uint32_t)((SIM_SOPT1 & (uint32_t)~(uint32_t)SIM_SOPT1_OSC32KSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_SOPT1_value); /* Update 32 kHz oscillator clock source (ERCLK32K) */
  SIM_SOPT2 = (uint32_t)((SIM_SOPT2 & (uint32_t)~(uint32_t)SIM_SOPT2_FTMFFCLKSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_SOPT2_value); /* Update FTM fixed frequency clock source */
  SIM_SOPT7 = (uint32_t)((SIM_SOPT7 & (uint32_t)~(uint32_t)(SIM_SOPT7_ADC0ALTCLKSRC_MASK | SIM_SOPT7_ADC1ALTCLKSRC_MASK)) | (uint32_t)CPU_ClockConfigDescriptors[CPU_CLOCK_CONFIG_0].SysRegs.SIM_SOPT7_value); /* Update ADC alternative clock sources */
  #if defined(__IAR_SYSTEMS_ICC__)    
  return 1;
  #endif
}

/*
** ===================================================================
**     Method      :  PE_low_level_init (component MKV10Z16FM7)
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
  /* RTOS initialization */

  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  
  /* NMI, reset and debug pins port clock gate enable */
  
  #if CPU_NMI_PIN | CPU_RESET_PIN | CPU_SWD_DIO_PIN | CPU_SWD_CLK_PIN
  SIM_SCGC5 |= (uint32_t)SIM_SCGC5_PORTA_MASK; /* NMI and reset pin clock gate enable */
  #endif /* CPU_NMI_PIN | CPU_RESET_PIN */

  /* Non-maskable interrupt pin initialization */
  
  #if CPU_NMI_PIN
  /* Non-maskable interrupt pin routing */
  /* PORTA_PCR4: ISF=0,MUX=7 */
  PORTA_PCR4 = (uint32_t)((PORTA_PCR4 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK
               )) | (uint32_t)(
                PORT_PCR_MUX(0x07)
               ));
  #endif /* CPU_NMI_PIN */

  /* Reset pin initialization */
  
  #if CPU_RESET_PIN
  /* Reset pin routing */
  /* PORTA_PCR20: ISF=0,MUX=7 */
  PORTA_PCR20 = (uint32_t)((PORTA_PCR20 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x07)
                ));
  #endif /* CPU_RESET_PIN */

  /* SWD DIO pin initialization */
  
  #if CPU_SWD_DIO_PIN
  /* SWD DIO pin routing */
  /* PORTA_PCR3: ISF=0,MUX=7 */
  PORTA_PCR3 = (uint32_t)((PORTA_PCR3 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK
               )) | (uint32_t)(
                PORT_PCR_MUX(0x07)
               ));
  #endif /* CPU_SWD_DIO_PIN */
  
  /* SWD CLK pin initialization */
  
  #if CPU_SWD_CLK_PIN
  /* SWD CLK pin routing */
  /* PORTA_PCR0: ISF=0,MUX=7 */
  PORTA_PCR0 = (uint32_t)((PORTA_PCR0 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK
               )) | (uint32_t)(
                PORT_PCR_MUX(0x07)
               ));
  #endif /* CPU_SWD_CLK_PIN */
  
  /* Low power modes protection */
  
  /* Disable very low power mode protection */
  #if CPU_LOW_POWER_ALLOW_VLP && CPU_LOW_POWER_ALLOW_VLLS
  /* SMC_PMPROT: AVLP=1,AVLLS=1 */
  SMC_PMPROT |= (SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AVLLS_MASK); /* VLPR VLPW VLPS and any VLLSx are allowed */
  #elif CPU_LOW_POWER_ALLOW_VLP
  /* SMC_PMPROT: AVLP=1 */
  SMC_PMPROT |= SMC_PMPROT_AVLP_MASK;  /* VLPR VLPW and VLPS are allowed */
  #elif CPU_LOW_POWER_ALLOW_VLLS
  /* SMC_PMPROT: AVLLS=1 */
  SMC_PMPROT |= SMC_PMPROT_AVLLS_MASK; /* Any VLLSx mode is allowed */
  #endif /* CPU_LOW_POWER_ALLOW_VLP && CPU_LOW_POWER_ALLOW_VLLS */
  
  /* Common initialization */
  
  #if CPU_COMMON_INIT
  Common_Init();
  #endif /* CPU_COMMON_INIT */
  
  /* Peripheral initialization components initialization */

  #if CPU_PERIPHERALS_INIT
  Peripherals_Init();
  #endif /* CPU_PERIPHERALS_INIT */
  
  /* OnReset event */
  
  #ifdef CPU_ON_RESET_EVENT_NAME
  CPU_ON_RESET_EVENT_NAME((uint16_t)(((uint16_t)RCM_SRS1) << 8U) | (uint16_t)RCM_SRS0); /* Invoke an user event */
  #endif /* CPU_ON_RESET_EVENT_NAME */
  
  /* Rest of components initialization */
  
  #if CPU_COMPONENTS_INIT
  Components_Init();
  #endif /* CPU_COMPONENTS_INIT */

  /* Interrupts priority level initialization */
  
  #if CPU_INT_PRIORITY == 0
  __DI();                              /* Disable interrupts */
  #elif CPU_INT_PRIORITY == 1
  __EI();                              /* Enable interrupts */
  #else
    #error Unsupported interrupt priority level
  #endif  
}

CPU_TClockGenMode CPU_GetClockGenMode(void)
{
  switch (MCG_C1  & MCG_C1_CLKS_MASK) {
    case  0x00U:
      /* Output of FLL is selected */
      if ((MCG_C1 & MCG_C1_IREFS_MASK) == 0x00U) {
        /* External reference clock is selected */
        return CPU_MCG_MODE_FEE;
      } else {
        /* Internal reference clock is selected */
        return CPU_MCG_MODE_FEI;
      }
    case 0x40U:
      /* Internal reference clock is selected */
      if ((MCG_C2 & MCG_C2_LP_MASK) == 0x00U) {
        /* Low power mode is disabled */
        return CPU_MCG_MODE_FBI;
      } else {
        /* Low power mode is enabled */
        return CPU_MCG_MODE_BLPI;
      }
    case 0x80U:
      /* External reference clock is selected */
      if ((MCG_C2 & MCG_C2_LP_MASK) == 0x00U) {
        /* FLL is selected */
        return CPU_MCG_MODE_FBE;
      } else {
        /* Low power mode is enabled */
        return CPU_MCG_MODE_BLPE;
      }
    default:
      return (CPU_TClockGenMode)0x00U;
  }
}

void CPU_SetClockConfigGenMode(LDD_TClockConfiguration ConfigID)
{
  CPU_TClockGenMode NextMode;
  CPU_TClockGenRegs NextModeRegs;
  CPU_TClockGenMode TargetMode = CPU_ClockConfigDescriptors[ConfigID].Mode;
  
  /* External clock source pin routing */
  if ((TargetMode & CPU_CLOCK_EXTERNAL_MASK) != 0) { /* Is external clock source used in targeted clock configuration? */
    /* If yes, initialize the EXTAL pin routing */
    SIM_SCGC5 |= (uint32_t)SIM_SCGC5_PORTA_MASK; /* Enable EXTAL/XTAL pins clock gate */
    /* PORTA_PCR18: ISF=0,MUX=0 */
    PORTA_PCR18 &= (uint32_t)~(uint32_t)(
                    PORT_PCR_ISF_MASK |
                    PORT_PCR_MUX(0x07)
                   );
    if ((TargetMode & CPU_CLOCK_EXTERNAL_CRYSTAL_MASK) != 0) { /* Is external crystal/resonator used in targeted clock configuration? */
      /* If yes, initialize also XTAL pin routing */
      /* PORTA_PCR19: ISF=0,MUX=0 */
      PORTA_PCR19 &= (uint32_t)~(uint32_t)(
                      PORT_PCR_ISF_MASK |
                      PORT_PCR_MUX(0x07)
                     );
    }
  }
  NextMode = CPU_GetClockGenMode(); /* Identify the currently active MCG mode */
  do {
    NextMode = ClockGenModeMatrix[NextMode & CPU_MCG_MODE_INDEX_MASK][TargetMode & CPU_MCG_MODE_INDEX_MASK];
    if (NextMode == TargetMode) {
      NextModeRegs = CPU_ClockConfigDescriptors[ConfigID].GenRegs;
    } else {
      NextModeRegs = *CPU_ClockGenModeRegs[NextMode & CPU_MCG_MODE_INDEX_MASK];
    }
    if ((NextMode & CPU_CLOCK_INTERNAL_MASK) != 0) {
      /* FEI, FBI, BLPI */
      if ((NextMode & CPU_CLOCK_LOW_POWER_MASK) == 0) {
        /* FEI, FBI */
        MCG_C2 &= (uint8_t)(~(MCG_C2_LP_MASK)); /* Enable FLL in bypass mode */
      }
      MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
      while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
      }
      MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection etc.; trim values not changed) */
      if ((NextMode & CPU_CLOCK_LOW_POWER_MASK) == 0) {
        /* FEI, FBI */
        MCG_C4 = ((NextModeRegs.MCG_C4_value) & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG_C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
      }
      OSC0_CR = NextModeRegs.OSC0_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
    } else {
      /* FEE, FBE, BLPE */
      MCG_C2 = ((NextModeRegs.MCG_C2_value) & (uint8_t)(~(MCG_C2_FCFTRIM_MASK))) | (MCG_C2 & MCG_C2_FCFTRIM_MASK); /* Set C2 (freq. range, ext. and int. reference selection, FLL enable in bypass mode etc.; trim values not changed) */
      OSC0_CR = NextModeRegs.OSC0_CR_value; /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
      MCG_C1 = NextModeRegs.MCG_C1_value; /* Set C1 (clock source selection, FLL ext. reference divider, int. reference enable etc.) */
      if ((NextMode & CPU_CLOCK_EXTERNAL_CRYSTAL_MASK) != 0) {
        while((MCG_S & MCG_S_OSCINIT0_MASK) == 0x00U) { /* Check that the oscillator is running */
        }
      }
      while((MCG_S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
      }
      if ((NextMode & CPU_CLOCK_LOW_POWER_MASK) == 0) {
        /* FEE, FBE */
        MCG_C4 = ((NextModeRegs.MCG_C4_value)  & (uint8_t)(~(MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK))) | (MCG_C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)); /* Set C4 (FLL output; trim values not changed) */
      }
    }
    switch (NextMode) {
      case CPU_MCG_MODE_FEI:
      case CPU_MCG_MODE_FEE:
        while((MCG_S & 0x0CU) != 0x00U) { /* Wait until output of the FLL is selected */
        }
        break;
      case CPU_MCG_MODE_FBI:
      case CPU_MCG_MODE_BLPI:
        while((MCG_S & 0x0CU) != 0x04U) { /* Wait until internal reference clock is selected as MCG output */
        }
        break;
      case CPU_MCG_MODE_FBE:
      case CPU_MCG_MODE_BLPE:
        while((MCG_S & 0x0CU) != 0x08U) { /* Wait until external reference clock is selected as MCG output */
        }
        break;
    }
  } while (NextMode != TargetMode); /* Loop until the target MCG mode is set */
}

/*
** ===================================================================
**     Method      :  Cpu_StartCmdAndWaitForCmdDoneOnTheStack (component MKV10Z16FM7)
**
**     Description :

**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#if CPU_OPEN_BACK_DOOR
static void CPU_StartCmdAndWaitForCmdDoneOnTheStack(void) 
{
  /* Allocate buffer for code on the stack. Code must be 16 bits aligned -> add one byte for address adjustment (if necessary) */
  uint8_t              SaveRoutineStackSpace[sizeof(FTFA_TStartCmdAndWaitForCmdRoutine) + 1];
  FTFA_TSafeRoutinePtr SafeRoutinePtr;

  /* Force buffer address to be 16 bits aligned */
  SafeRoutinePtr = (FTFA_TSafeRoutinePtr)(((uint32_t)&SaveRoutineStackSpace + 1U) & ~(uint32_t)1U);
  /* Copy code to the buffer on the stack */
  *(FTFA_TStartCmdAndWaitForCmdRoutine *)(void *)SafeRoutinePtr = *(FTFA_TStartCmdAndWaitForCmdRoutine *)(void *)&FTFA_StartCmdAndWaitForCmdDoneRoutine;
  /* Call code on the stack */
  ((FTFA_TSafeRoutinePtr)((uint32_t)(SafeRoutinePtr) | 1U))();
}
#endif /* CPU_OPEN_BACK_DOOR */

/*
** ===================================================================
**     Method      :  Cpu_SetClockConfiguration (component MKV10Z16FM7)
*/
/*!
**     @brief
**         Calling of this method will cause the clock configuration
**         change and reconfiguration of all components according to
**         the requested clock configuration setting.
**     @param
**         ModeID          - Clock configuration identifier
**     @return
**                         - ERR_OK - OK.
**                           ERR_RANGE - Mode parameter out of range
*/
/* ===================================================================*/
#if CPU_SET_CLOCK_CONFIGURATION
LDD_TError CPU_SetClockConfiguration(LDD_TClockConfiguration ModeID)
{
  if (ModeID > (CPU_CLOCK_CONFIG_NUMBER - 1U)) {
    return ERR_RANGE;
  }
  
  if (SMC_PMSTAT == 0x04U) {           /* Test if in VLPR mode */
    /* SMC_PMCTRL: ??=0,RUNM=0,??=0,STOPA=0,STOPM=0 */
    SMC_PMCTRL = (SMC_PMCTRL_RUNM(0x00) | SMC_PMCTRL_STOPM(0x00)); /* Disable very low power mode */
    while((PMC_REGSC & PMC_REGSC_REGONS_MASK) == 0x00U) { /* Wait until the regulator is in run regulation */
    }
    while(SMC_PMSTAT == 0x04U) {       /* Wait until the system is back in RUN mode */
    }
  }
  /* SIM_CLKDIV1: OUTDIV1=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=3,OUTDIV5EN=1,OUTDIV5=4,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x01) |
                SIM_CLKDIV1_OUTDIV4(0x03) |
                SIM_CLKDIV1_OUTDIV5EN_MASK |
                SIM_CLKDIV1_OUTDIV5(0x04); /* Set the system prescalers to safe value */
  switch (ModeID) {
  #if defined(CPU_CLOCK_CONFIG_0)
    case CPU_CLOCK_CONFIG_0:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_0);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_0) */
  #if defined(CPU_CLOCK_CONFIG_1)
    case CPU_CLOCK_CONFIG_1:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_1);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_1) */
  #if defined(CPU_CLOCK_CONFIG_2)
    case CPU_CLOCK_CONFIG_2:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_2);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_2) */
  #if defined(CPU_CLOCK_CONFIG_3)
    case CPU_CLOCK_CONFIG_3:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_3);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_3) */
  #if defined(CPU_CLOCK_CONFIG_4)
    case CPU_CLOCK_CONFIG_4:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_4);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_4) */
  #if defined(CPU_CLOCK_CONFIG_5)
    case CPU_CLOCK_CONFIG_5:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_5);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_5) */
  #if defined(CPU_CLOCK_CONFIG_6)
    case CPU_CLOCK_CONFIG_6:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_6);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_6) */
  #if defined(CPU_CLOCK_CONFIG_7)
    case CPU_CLOCK_CONFIG_7:
      CPU_SetClockConfigGenMode(CPU_CLOCK_CONFIG_7);
      break;
  #endif /* defined(CPU_CLOCK_CONFIG_7) */
    default:
      break;
  }
  SIM_CLKDIV1 = (uint32_t)CPU_ClockConfigDescriptors[ModeID].SysRegs.SIM_CLKDIV1_value; /* Update system prescalers */
  SIM_SOPT1 = (uint32_t)((SIM_SOPT1 & (uint32_t)~(uint32_t)SIM_SOPT1_OSC32KSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[ModeID].SysRegs.SIM_SOPT1_value); /* Update 32 kHz oscillator clock source (ERCLK32K) */
  SIM_SOPT2 = (uint32_t)((SIM_SOPT2 & (uint32_t)~(uint32_t)SIM_SOPT2_FTMFFCLKSEL_MASK) | (uint32_t)CPU_ClockConfigDescriptors[ModeID].SysRegs.SIM_SOPT2_value); /* Update FTM fixed frequency clock source */
  SIM_SOPT7 = (uint32_t)((SIM_SOPT7 & (uint32_t)~(uint32_t)(SIM_SOPT7_ADC0ALTCLKSRC_MASK | SIM_SOPT7_ADC1ALTCLKSRC_MASK)) | (uint32_t)CPU_ClockConfigDescriptors[ModeID].SysRegs.SIM_SOPT7_value); /* Update ADC alternative clock sources */
  LDD_SetClockConfiguration(ModeID);   /* Call all LDD components to update the clock configuration */
  ClockConfigurationID = ModeID;       /* Store clock configuration identifier */
  if ((CPU_ClockConfigDescriptors[ClockConfigurationID].LowPower) == (CPU_CLOCK_VLP_ENABLE_MASK | CPU_CLOCK_VLP_AUTO_ENABLE_MASK)) {
    /* SMC_PMCTRL: ??=0,RUNM=2,??=0,STOPA=0,STOPM=0 */
    SMC_PMCTRL = (SMC_PMCTRL_RUNM(0x02) | SMC_PMCTRL_STOPM(0x00)); /* Enable very low power mode */
    while(SMC_PMSTAT != 0x04U) {       /* Wait until in the Very Low Power run mode */
    }
  }
  return ERR_OK;
}
#endif /* CPU_SET_CLOCK_CONFIGURATION */

/*
** ===================================================================
**     Method      :  Cpu_GetClockConfiguration (component MKV10Z16FM7)
*/
/*!
**     @brief
**         Returns the active clock configuration identifier. The
**         method is automatically enabled if more than one clock
**         configuration is enabled in the component.
**     @return
**                         - Active clock configuration identifier
*/
/* ===================================================================*/
#if CPU_GET_CLOCK_CONFIGURATION
LDD_TClockConfiguration CPU_GetClockConfiguration(void)
{
#if CPU_CLOCK_CONFIG_NUMBER > 1
  return ClockConfigurationID;         /* Return the actual clock configuration identifier */
#else
  return CPU_CLOCK_CONFIG_0;           /* Return the actual clock configuration identifier */
#endif
}
#endif /* CPU_GET_CLOCK_CONFIGURATION */

/*
** ===================================================================
**     Method      :  Cpu_SetOperationMode (component MKV10Z16FM7)
*/
/*!
**     @brief
**         This method requests to change the component's operation
**         mode (RUN, WAIT, SLEEP, STOP). The target operation mode
**         will be entered immediately. 
**         See [Operation mode settings] for further details of the
**         operation modes mapping to low power modes of the cpu.
**     @param
**         OperationMode   - Requested driver
**                           operation mode
**     @param
**         ModeChangeCallback - Callback to
**                           notify the upper layer once a mode has been
**                           changed. Parameter is ignored, only for
**                           compatibility of API with other components.
**     @param
**         ModeChangeCallbackParamPtr 
**                           - Pointer to callback parameter to notify
**                           the upper layer once a mode has been
**                           changed. Parameter is ignored, only for
**                           compatibility of API with other components.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_PARAM_MODE - Invalid operation mode
*/
/* ===================================================================*/
#if CPU_SET_OPERATION_MODE
LDD_TError CPU_SetOperationMode(LDD_TDriverOperationMode OperationMode, LDD_TCallback ModeChangeCallback, LDD_TCallbackParam *ModeChangeCallbackParamPtr)
{
  (void) ModeChangeCallback;           /* Parameter is not used, suppress unused argument warning */
  (void) ModeChangeCallbackParamPtr;   /* Parameter is not used, suppress unused argument warning */
  
    switch (OperationMode) {
    case DOM_RUN:
      /* SCB_SCR: SLEEPDEEP=0,SLEEPONEXIT=0 */
      SCB_SCR &= (uint32_t)~(uint32_t)(
                  SCB_SCR_SLEEPDEEP_MASK |
                  SCB_SCR_SLEEPONEXIT_MASK
                 );                    /* Set RUN or VLPR mode */
      break;
    case DOM_WAIT:
      /* SCB_SCR: SLEEPDEEP=0 */
      SCB_SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPDEEP_MASK); /* WAIT or VLPW mode can be entered after WFI instruction call */
  #if CPU_LOW_POWER_WAIT_SLEEP_ON_EXIT
      /* SCB_SCR: SLEEPONEXIT=1 */
      SCB_SCR |= SCB_SCR_SLEEPONEXIT_MASK; /* Enter wait state on ISR exit */
  #else /* CPU_LOW_POWER_WAIT_SLEEP_ON_EXIT */
      /* SCB_SCR: SLEEPONEXIT=0 */
      SCB_SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_MASK); /* Do not enter wait state on ISR exit */
  #endif /* CPU_LOW_POWER_WAIT_SLEEP_ON_EXIT */
      PE_WFI();                        /* Enter WAIT or VLPW state */
      break;
    case DOM_SLEEP:
      /* SCB_SCR: SLEEPDEEP=1 */
      SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK; /* STOP VLPS or VLLSx mode can be entered after WFI instruction call */
      SMC_STOPCTRL = (uint8_t)((SMC_STOPCTRL & (uint8_t)~(uint8_t)SMC_STOPCTRL_PSTOPO_MASK) | (uint8_t)CPU_LOW_POWER_SLEEP_MODE); /* Select (partial) stop mode enetered after WFI instruction call */
      /* SMC_PMCTRL: STOPM=0 */
      SMC_PMCTRL &= (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x07)); /* Set normal STOP mode */
      (void)(SMC_PMCTRL == 0U);        /* Dummy read of SMC_PMCTRL to ensure the register is written before entering low power mode */
  #if CPU_LOW_POWER_SLEEP_SLEEP_ON_EXIT
      /* SCB_SCR: SLEEPONEXIT=1 */
      SCB_SCR |= SCB_SCR_SLEEPONEXIT_MASK; /* Enter stop state on ISR exit */
  #else /* CPU_LOW_POWER_SLEEP_SLEEP_ON_EXIT */
      /* SCB_SCR: SLEEPONEXIT=0 */
      SCB_SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_MASK); /* Do not enter stop state on ISR exit */
  #endif /* CPU_LOW_POWER_SLEEP_SLEEP_ON_EXIT */
      PE_WFI();                        /* Enter STOP state */
      break;
    case DOM_STOP:
  #if CPU_LOW_POWER_STOP
      /* Clear LLWU flags */
      /* LLWU_F1: WUF7=1,WUF6=1,WUF5=1,WUF4=1,WUF3=1,WUF2=1,WUF1=1,WUF0=1 */
      LLWU_F1 = LLWU_F1_WUF7_MASK |
                LLWU_F1_WUF6_MASK |
                LLWU_F1_WUF5_MASK |
                LLWU_F1_WUF4_MASK |
                LLWU_F1_WUF3_MASK |
                LLWU_F1_WUF2_MASK |
                LLWU_F1_WUF1_MASK |
                LLWU_F1_WUF0_MASK;
      /* LLWU_F2: WUF15=1,WUF14=1,WUF13=1,WUF12=1,WUF11=1,WUF10=1,WUF9=1,WUF8=1 */
      LLWU_F2 = LLWU_F2_WUF15_MASK |
                LLWU_F2_WUF14_MASK |
                LLWU_F2_WUF13_MASK |
                LLWU_F2_WUF12_MASK |
                LLWU_F2_WUF11_MASK |
                LLWU_F2_WUF10_MASK |
                LLWU_F2_WUF9_MASK |
                LLWU_F2_WUF8_MASK;
      /* LLWU_F3: MWUF7=1,MWUF6=1,MWUF5=1,MWUF4=1,MWUF3=1,MWUF2=1,MWUF1=1,MWUF0=1 */
      LLWU_F3 = LLWU_F3_MWUF7_MASK |
                LLWU_F3_MWUF6_MASK |
                LLWU_F3_MWUF5_MASK |
                LLWU_F3_MWUF4_MASK |
                LLWU_F3_MWUF3_MASK |
                LLWU_F3_MWUF2_MASK |
                LLWU_F3_MWUF1_MASK |
                LLWU_F3_MWUF0_MASK;
      /* LLWU_FILT1: FILTF=1 */
      LLWU_FILT1 |= LLWU_FILT1_FILTF_MASK;
      /* LLWU_FILT2: FILTF=1 */
      LLWU_FILT2 |= LLWU_FILT2_FILTF_MASK;
      /* SCB_SCR: SLEEPONEXIT=0 */
      SCB_SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_MASK); /* Do not enter stop state on ISR exit */
      /* SCB_SCR: SLEEPDEEP=1 */
      SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK; /* STOP VLPS or VLLSx mode can be entered after WFI instruction call */
      /* SMC_PMCTRL: STOPM=4 */
      SMC_PMCTRL = (uint8_t)((SMC_PMCTRL & (uint8_t)~(uint8_t)(
                    SMC_PMCTRL_STOPM(0x03)
                   )) | (uint8_t)(
                    SMC_PMCTRL_STOPM(0x04)
                   ));                 /* Set Very-low-leakage mode */
    #if (CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS0)
      #if (CPU_LOW_POWER_STOP_POR_DETECT)
      /* SMC_STOPCTRL: PSTOPO=0,PORPO=0,??=0,LPOPO=0,VLLSM=0 */
      SMC_STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_VLLSM(0x00)); /* Set VLLS0 mode with POR detect enabled */
      #else /* CPU_LOW_POWER_STOP_POR_DETECT */
      /* SMC_STOPCTRL: PSTOPO=0,PORPO=1,??=0,LPOPO=0,VLLSM=0 */
      SMC_STOPCTRL = SMC_STOPCTRL_PSTOPO(0x00) |
                     SMC_STOPCTRL_PORPO_MASK |
                     SMC_STOPCTRL_VLLSM(0x00); /* Set VLLS0 mode with POR detect disabled */
      #endif /* CPU_LOW_POWER_STOP_POR_DETECT */
    #else /* CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS0 */
      #if (CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS3)
      /* SMC_STOPCTRL: PSTOPO=0,PORPO=0,??=0,LPOPO=0,VLLSM=3 */
      SMC_STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_VLLSM(0x03)); /* Set VLLS3 mode */
      #elif (CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS1)
      /* SMC_STOPCTRL: PSTOPO=0,PORPO=0,??=0,LPOPO=0,VLLSM=1 */
      SMC_STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_VLLSM(0x01)); /* Set VLLS1 mode */
      #else /* CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS3 */
            #error Unsupported STOP low power mode
      #endif /* CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS3 */
      #if !(CPU_LOW_POWER_STOP_LPO_CLOCK)
      /* SMC_STOPCTRL: LPOPO=1 */
      SMC_STOPCTRL |= SMC_STOPCTRL_LPOPO_MASK; /* Disable 1kHz LPO clock */
      #endif /* !(CPU_LOW_POWER_STOP_LPO_CLOCK) */
    #endif /* CPU_LOW_POWER_STOP == CPU_LOW_POWER_STOP_VLLS0 */
      (void)(SMC_PMCTRL == 0U);        /* Dummy read of SMC_PMCTRL to ensure the register is written before entering low power mode */
      PE_WFI();                        /* Enter STOP state */
  #endif /* CPU_LOW_POWER_STOP */
      break;
    default:
      return ERR_PARAM_MODE;
  }
  return ERR_OK;
}
#endif /* CPU_SET_OPERATION_MODE */

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MKV10Z16FM7)
*/
/*!
**     @brief
**         Enables all maskable interrupts.
*/
/* ===================================================================*/
#if CPU_ENABLE_INT
void CPU_EnableInt(void)
{
 __EI();
}
#endif /* CPU_ENABLE_INT */

/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MKV10Z16FM7)
*/
/*!
**     @brief
**         Disables all maskable interrupts.
*/
/* ===================================================================*/
#if CPU_DISABLE_INT
void CPU_DisableInt(void)
{
 __DI();
}
#endif /* CPU_DISABLE_INT */

/*
** ===================================================================
**     Method      :  Cpu_OpenBackDoor (component MKV10Z16FM7)
*/
/*!
**     @brief
**         Tries to unsecure MCU until the next reset. 
**         Note:
**         This method is only available when the flash security and
**         protection properties are enabled.
**     @param
**         KeyPtr          - A pointer to the back door key. The
**                           key has to be composed from 8 partial keys
**                           of the 8bits size each. The pointer points
**                           to the partial key 0.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK MCU is now in unsecured state
**                           until next reset. 
**                           - ERR_FAILED - Command failed. Bad key has
**                           been provided.
*/
/* ===================================================================*/
#if CPU_OPEN_BACK_DOOR
LDD_TError CPU_OpenBackDoor(uint8_t *KeyPtr)
{
  /*
  ** "key" value is defined by Backdoor key 0 - 7 properties
  ** placed in the Flash configuration field.
  */

  /* Wait until previous operation is done */
  while(!FTFA_PDD_GetCmdCompleteFlag(FTFA_BASE_PTR)){}
  /* Start "Set FlexRAM function" command and wait for result */
  FTFA_PDD_ClearErrorFlags(FTFA_BASE_PTR);
  FTFA_PDD_Cmd_VerifyBackdoorAccessKey_Init(FTFA_BASE_PTR, KeyPtr[0], KeyPtr[1], KeyPtr[2], KeyPtr[3], KeyPtr[4], KeyPtr[5], KeyPtr[6], KeyPtr[7]);
  CPU_StartCmdAndWaitForCmdDoneOnTheStack();
  /* Check error code */
  if ((FTFA_PDD_GetFlags(FTFA_BASE_PTR) & (FTFA_PDD_ACCESS_ERROR | FTFA_PDD_COMMAND_COMPLETION_STATUS)) != 0x00U){ /* Is error detected during execution command? */
    /* Command failed. Bad key has been provided. */
    return ERR_FAILED;
  } else {
    /* MCU is now in unsecured state until next reset. */
    return ERR_OK;
  }
}
#endif /* CPU_OPEN_BACK_DOOR */

/*
** ===================================================================
**     Method      :  Cpu_MCGAutoTrim (component MKV10Z16FM7)
*/
/*!
**     @brief
**         This method uses MCG auto trim feature to trim internal
**         reference clock. This method can be used only in a clock
**         configuration which derives its bus clock from external
**         reference clock ([MCG mode] must be one of the following
**         modes - FEE, FBE, BLPE, PEE, PBE) and if value of [Bus clock]
**         is in the range <8; 16>MHz.
**         The slow internal reference clock is trimmed to the value
**         selected by [Slow internal reference clock [kHz]] property. 
**         The fast internal reference clock will be trimmed to value
**         4MHz.
**     @param
**         ClockSelect     - Selects which internal
**                           reference clock will be trimmed.
**                           0 ... slow (32kHz) internal reference clock
**                           will be trimmed
**                           > 0 ... fast (4MHz) internal reference
**                           clock will be trimmed
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_SPEED - The method does not work in the
**                           active clock configuration.
**                           ERR_FAILED - Autotrim process failed.
*/
/* ===================================================================*/
#if CPU_MCG_AUTO_TRIM
LDD_TError CPU_MCGAutoTrim(uint8_t ClockSelect)
{
  uint16_t CompareValue;               /* Autotrim compare value expected count */
  uint16_t BusClockMHz = (uint16_t)((uint32_t)(CPU_ClockConfigDescriptors[ClockConfigurationID].BusClock) /1000000UL); /* Bus clock in MHz */

  /* Calculate compare value */
  if (((CPU_ClockConfigDescriptors[ClockConfigurationID].Mode & CPU_CLOCK_EXTERNAL_MASK) != 0) && (BusClockMHz >= 8U) && (BusClockMHz <= 16U)) {
    if (ClockSelect == 0x00U) {
      CompareValue = (uint16_t)(21000 * BusClockMHz / CPU_INT_SLOW_CLK_HZ); /* Slow internal reference clock */
    } else {
      CompareValue = (uint16_t)(672U * BusClockMHz); /* Fast internal reference clock */
    }
  } else {
    return ERR_SPEED;                  /* MCU has to be clocked from external clock and has to have bus clock in allowed range */
  }
  MCG_ATCVH = (uint8_t)(CompareValue >> 8U); /* Set compare value high half */
  MCG_ATCVL = (uint8_t)(CompareValue & 255U); /* Set compare value low half */
  if (ClockSelect == 0x00U) {
    /* MCG_SC: ATME=1,ATMS=0 */
    MCG_SC = (uint8_t)((MCG_SC & (uint8_t)~(uint8_t)(
              MCG_SC_ATMS_MASK
             )) | (uint8_t)(
              MCG_SC_ATME_MASK
             ));                       /* Start trimming of the slow internal reference clock */
  } else {
    /* MCG_SC: ATME=1,ATMS=1 */
    MCG_SC |= (MCG_SC_ATME_MASK | MCG_SC_ATMS_MASK); /* Start trimming of the fast internal reference clock */
  }
  while ((MCG_SC & MCG_SC_ATME_MASK) != 0x00U) { /* Wait until autotrim completes */
  }
  if ((MCG_SC & MCG_SC_ATMF_MASK) == 0x00U) {
    return ERR_OK;                     /* Trim operation completed successfully */
  } else {
    return ERR_FAILED;                 /* Trim operation failed */
  }
}
#endif /* CPU_MCG_AUTO_TRIM */

/*
** ===================================================================
**     Method      :  Cpu_VLPModeEnable (component MKV10Z16FM7)
*/
/*!
**     @brief
**         This method enables Very Low Power (VLP) mode in the active
**         clock configuration. To enable the VLP mode the active clock
**         configuration has to have the [Very low power mode] property
**         enabled. The method waits until the entry to the VLP mode is
**         confirmed by a power management module status register.
**         This method can be enabled only when one of the clock
**         configurations modes have the [Very low power mode] property
**         enabled.
**     @return
**                         - Error code 
**                           ERR_OK - OK
**                           ERR_SPEED - the Very Low Power mode is not
**                           supported in the active clock configuration.
*/
/* ===================================================================*/
#if CPU_VLP_MODE_ENABLE
LDD_TError CPU_VLPModeEnable(void)
{
  if ((CPU_ClockConfigDescriptors[ClockConfigurationID].LowPower & CPU_CLOCK_VLP_ENABLE_MASK) == 0U) {
    return ERR_SPEED;                  /* Very low power mode not enabled in the active clock configuration. */
  }
  if (SMC_PMSTAT == 0x04U) {           /* If the system is already in VLP mode, return */
    return ERR_OK;
  }
  /* SMC_PMCTRL: ??=0,RUNM=2,??=0,STOPA=0,STOPM=0 */
  SMC_PMCTRL = (SMC_PMCTRL_RUNM(0x02) | SMC_PMCTRL_STOPM(0x00)); /* Enable very low power mode */
  while(SMC_PMSTAT != 0x04U) {         /* Wait until in the Very Low Power run mode */
  }
  return ERR_OK;
}
#endif /* CPU_VLP_MODE_ENABLE */

/*
** ===================================================================
**     Method      :  Cpu_VLPModeDisable (component MKV10Z16FM7)
*/
/*!
**     @brief
**         This method disables the Very Low Power (VLP) mode in the
**         active clock configuration. The method waits until the exit
**         of the VLP mode is confirmed by a power management module
**         status register.
**         This method can be enabled only when one of the clock
**         configurations have the [Very low power mode] property
**         enabled.
**     @return
**                         - Error code 
**                           ERR_OK - OK
**                           ERR_SPEED - the Very Low Power mode is not
**                           supported in the active clock configuration.
*/
/* ===================================================================*/
#if CPU_VLP_MODE_DISABLE
LDD_TError CPU_VLPModeDisable(void)
{
  if ((CPU_ClockConfigDescriptors[ClockConfigurationID].LowPower & CPU_CLOCK_VLP_ENABLE_MASK) == 0U) {
    return ERR_SPEED;                  /* Very low power mode not enabled in the active clock configuration. */
  }
  if (SMC_PMSTAT == 0x04U) {           /* Test if in VLPR mode */
    /* SMC_PMCTRL: ??=0,RUNM=0,??=0,STOPA=0,STOPM=0 */
    SMC_PMCTRL = (SMC_PMCTRL_RUNM(0x00) | SMC_PMCTRL_STOPM(0x00)); /* Disable very low power mode */
    while((PMC_REGSC & PMC_REGSC_REGONS_MASK) == 0x00U) { /* Wait until the regulator is in run regulation */
    }
    while(SMC_PMSTAT == 0x04U) {       /* Wait until the system is back in RUN mode */
    }
  }
  return ERR_OK;
}
#endif /* CPU_VLP_MODE_DISABLE */

/*
** ===================================================================
**     Method      :  Cpu_SystemReset (component MKV10Z16FM7)
*/
/*!
**     @brief
**         This method initiates a system reset request to reset the
**         CPU.
*/
/* ===================================================================*/
#if CPU_SYSTEM_RESET
void CPU_SystemReset(void)
{
  /* SCB_AIRCR: VECTKEY=0x05FA,SYSRESETREQ=1 */
  SCB_AIRCR = (uint32_t)((SCB_AIRCR & (uint32_t)~(uint32_t)(
               SCB_AIRCR_VECTKEY(0xFA05)
              )) | (uint32_t)(
               SCB_AIRCR_VECTKEY(0x05FA) |
               SCB_AIRCR_SYSRESETREQ_MASK
              ));                      /* Request system reset */
  while(1) {                           /* Wait until reset */
  }
}
#endif /* CPU_SYSTEM_RESET */


#ifdef __cplusplus
}
#endif

/* END CPU_Init.c */

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