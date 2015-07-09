/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : DMA1.c
**     Project     : ModuleHub
**     Processor   : MKV10Z16VFM7
**     Component   : DMAController
**     Version     : Component 01.039, Driver 01.02, CPU db: 3.50.001
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-01, 20:53, # CodeGen: 1
**     Abstract    :
**          This embedded component implements initialization
**          and runtime handling of an on-chip DMA controller.
**     Settings    :
**          Component name                                 : DMA1
**          OnError                                        : yes
**          OnComplete                                     : yes
**          RWTransactionSizeGtThenOne                     : no
**          RWTransactionSize                              : 1
**          RWTransactionUnitSameInAllComponent            : yes
**          HasCycleStealMode                              : yes
**          HasHalfCompleteInterrupt                       : no
**          Device                                         : DMA
**          Settings                                       : 
**            Minor loop mapping                           : Disabled
**            Continuous mode                              : Disabled
**            Halt on error                                : Disabled
**            Debug                                        : Disabled
**          Channel arbitration type                       : Fixed
**            Autoset channel priority                     : 
**          Statically allocated channels                  : 1
**            Channel settings                             : Chan: 0
**              Channel                                    : DMA_Channel0
**              Allocate channel                           : no
**              Channel shared                             : no
**              Used by component(s)                       : DMACH1
**              Arbitration type                           : Fixed
**                Priority                                 : 0
**                Can be preempted                         : no
**                Can preempt                              : no
**              Interrupt service                          : Enabled
**                Transfer complete interrupt              : 
**                  Interrupt vector                       : INT_DMA0
**                  Interrupt priority                     : 64
**                  ISR Name                               : DMA1_INT_DMA0_TransferComplete_ISR
**                Error interrupt                          : 
**                  Interrupt vector                       : INT_DMA_Error
**                  Interrupt priority                     : medium priority
**                  ISR Name                               : DMA1_INT_DMA_Error_TransferError_ISR
**              Component name0                            : DMACH1
**          Dynamically allocatable channels               : 0
**     Contents    :
**         Init            - LDD_TDeviceData* DMA1_Init(void);
**         InitChannel     - LDD_TDeviceData* DMA1_InitChannel(LDD_TDeviceData *DeviceDataPtr,...
**         StartTransfer   - LDD_TError DMA1_StartTransfer(DMA1_TChanDeviceData *ChanDeviceDataPtr);
**         GetError        - LDD_DMA_TErrorFlags DMA1_GetError(DMA1_TChanDeviceData *ChanDeviceDataPtr);
**         SetRequestCount - LDD_TError DMA1_SetRequestCount(DMA1_TChanDeviceData *ChanDeviceDataPtr,...
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
** @file DMA1.c
** @version 01.02
** @brief
**          This embedded component implements initialization
**          and runtime handling of an on-chip DMA controller.
*/         
/*!
**  @addtogroup DMA1_module DMA1 module documentation
**  @{
*/         

/* MODULE DMA1. */

#include "DMA1.h"
#include "DMA_PDD.h"
#include "DMAMUX_PDD.h"
#include "SIM_PDD.h"

/* {Default RTOS Adapter} No RTOS includes */
/* Total number of channel supported by DMA device */
#define DMA1_CHANNEL_COUNT                                  0x01U
/* Number of statically allocated DMA channels */
#define DMA1_STATIC_CHANNEL_COUNT                           0x01U
/* Number of dynamically allocated DMA channels */
#define DMA1_DYNAMIC_CHANNEL_COUNT                          0x00U

/* Logical channel constants */
#define DMA1_CHN_0_PHY_NUM                                  (0U)
#define DMA1_CHN_0_PHY_MASK                                 (1U)
#define DMA1_CHN_0_DMA_REGS_PTR                             (DMA_BASE_PTR)
#define DMA1_CHN_0_TCD_PTR                                  ((DMA1_TTCD*)(void*)&DMA_TCD0_SADDR)
#define DMA1_CHN_0_DMAMUX_CFG_REG_PTR                       ((uint8_t*)(void*)&DMAMUX_CHCFG0)


#define DMA_PDD_CHN_ERROR_FLAGS  (DMA_ES_DBE_MASK | DMA_ES_SBE_MASK | DMA_ES_SGE_MASK | DMA_ES_NCE_MASK | \
                                        DMA_ES_DOE_MASK  | DMA_ES_DAE_MASK | DMA_ES_SOE_MASK | DMA_ES_SAE_MASK | DMA_ES_ECX_MASK)
#define DMA_PDD_DEV_ERROR_FLAGS  (DMA_ES_CPE_MASK)
#define DMA_PDD_InitChannelsPriority(_DmaMemMapPtr,_ChnPriorities) (*(DMA1_TChnPriorities *)((void *)&(_DmaMemMapPtr->DCHPRI3)) = _ChnPriorities)

typedef struct DMA1_TChnPriorities_struct {
  uint8_t DMA1_TChnPriorities[4];                          /* Channel initial priorities */
} DMA1_TChnPriorities, * DMA1_TChnPrioritiesPtr;

/* Channel constants structure prototype */
typedef struct DMA1_TChnDevConst_struct {
  uint32_t                         PhyChnNum;              /*!< Physical channel number */
  uint32_t                         PhyChnMask;             /*!< Physical channel mask */
  DMA_MemMapPtr                    DmaMemMapPtr;           /*!< DMA registers address */
  DMA1_TTCD                       *TCDPtr;                 /*!< TCD registers address */
  uint8_t                         *DmaMuxRegPtr;           /*!< DMA mux. register address */
} DMA1_TChnDevConst, *DMA1_TChnDevConstPtr;

/* Device constants structure prototype */
typedef struct DMA1_TDevConst_struct {
  DMA1_TChnDevConst const          ChnDevConst[DMA1_CHANNEL_COUNT];
  DMA1_TChnPriorities              ChnPriorities;          /*!< Channel initial priorities */
} DMA1_TDevConst, *DMA1_TDevConstPtr;

/* Channel device data structure prototype */
typedef struct DMA1_TChnDevData_stuct {
  DMA1_TTCD                       *TCDPtr;                 /*!< Channel TCD address */
  DMA1_TChnDevConst const         *ChnDevConstPtr;         /*!< Channel device constants structure address */
  LDD_DMA_TErrorFlags              ErrorFlags;             /*!< Channel error flags */
  LDD_TUserData                   *UserDataPtr;            /*!< User data pointer */
  DMA1_TEvents                     Events;                 /*!< Events state variable */
} DMA1_TChnDevData, *DMA1_TChnDevDataPtr;

/* Device data structure prototype */
typedef struct {
  DMA1_TChnDevData                 ChnDevData[DMA1_CHANNEL_COUNT]; /*!< Channel device data array */
} DMA1_TDeviceData, *DMA1_TDeviceDataPtr;

/* Channel constants array */
DMA1_TDevConst const DMA1_DevConst = {
  {
    { /* Channel 0 (DMA_Channel0) */
      DMA1_CHN_0_PHY_NUM,                                  /*!< Physical channel number */
      DMA1_CHN_0_PHY_MASK,                                 /*!< Physical channel mask */
      DMA1_CHN_0_DMA_REGS_PTR,                             /*!< DMA registers address */
      DMA1_CHN_0_TCD_PTR,                                  /*!< TCD registers address */
      DMA1_CHN_0_DMAMUX_CFG_REG_PTR                        /*!< Mux cfg. register address */
    }
  },
  /* Channel priorities */
  {
    {
      DMA_DCHPRI0_CHPRI(0x03),         /* DCHPR3 register initial value */
      DMA_DCHPRI0_CHPRI(0x02),         /* DCHPR2 register initial value */
      DMA_DCHPRI0_CHPRI(0x01),         /* DCHPR1 register initial value */
      (DMA_DCHPRI0_DPA_MASK | DMA_DCHPRI0_CHPRI(0x00)) /* DCHPR0 register initial value */
    }
  }
};


/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static DMA1_TDeviceData DevDataPtr__DEFAULT_RTOS_ALLOC;
/* {Default RTOS Adapter} Global variable used for passing a parameter into ISR */
static DMA1_TDeviceDataPtr INT_DMA0__DEFAULT_RTOS_ISRPARAM;
/* {Default RTOS Adapter} Global variable used for passing a parameter into ISR */
static DMA1_TDeviceDataPtr INT_DMA_Error__DEFAULT_RTOS_ISRPARAM;
static void HandleInterrupt(DMA1_TChnDevData *ChnDevDataPtr);
/*
** ===================================================================
**     Method      :  _CancelTransfer (component DMAController)
**
**     Description :
**         CancelTransfer
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void CancelTransfer(DMA1_TChnDevConst const *ChnDevConstPtr)
{
  uint8_t                         PhyChnNum = ChnDevConstPtr->PhyChnNum;
  
  /* Disable new request from peripheral in DMA MUX */
  DMAMUX_PDD_EnableChannel(DMAMUX_BASE_PTR, PhyChnNum, PDD_DISABLE);
  DMA_PDD_EnablePeripheralRequest(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum, PDD_DISABLE); /* Disable new request from peripheral. */
  if (DMA_PDD_GetActiveFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum)) { /* Channel active? */
    DMA_PDD_HaltOperations(DMA_BASE_PTR); /* Yes, stall start of any new channels */
    DMA_PDD_CancelTransfer(DMA_BASE_PTR); /* Cancel current transfer */
    DMA_PDD_ResumeOperations(DMA_BASE_PTR); /* Resume DMA operation */
  }
  DMA_PDD_ClearDoneFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
  DMA_PDD_ClearChannelInterruptFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
  DMA_PDD_ClearChannelErrorFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
  /* Enable new request from peripheral in DMA MUX */
  DMAMUX_PDD_EnableChannel(DMAMUX_BASE_PTR, PhyChnNum, PDD_ENABLE);
}

/*
** ===================================================================
**     Method      :  DMA1_Init (component DMAController)
*/
/*!
**     @brief
**         Initializes the associated peripheral(s) and the component
**         internal variables. The method is called automatically as a
**         part of the application initialization code.
**     @return
**                         - Pointer to the device data structure. 
*/
/* ===================================================================*/
LDD_TDeviceData* DMA1_Init(void)
{
  DMA1_TDeviceData                 *DevDataPtr;            /* LDD device structure */

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DevDataPtr = &DevDataPtr__DEFAULT_RTOS_ALLOC;
  /* {Default RTOS Adapter} Driver memory allocation: Fill the allocated memory by zero value */
  PE_FillMemory(DevDataPtr, 0U, sizeof(DMA1_TDeviceData));
  /* Transfer complete interrupt vector(INT_DMA0) allocation */
  /* {Default RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_DMA0__DEFAULT_RTOS_ISRPARAM =  DevDataPtr;
  /* Transfer error interrupt vector(INT_DMA_Error) allocation */
  /* {Default RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_DMA_Error__DEFAULT_RTOS_ISRPARAM =  DevDataPtr;
  /* Transfer complete interrupt vector(INT_DMA0) priority setting */
  /* NVIC_IPR0: PRI_0=1 */
  NVIC_IPR0 = (uint32_t)((NVIC_IPR0 & (uint32_t)~(uint32_t)(
               NVIC_IP_PRI_0(0x02)
              )) | (uint32_t)(
               NVIC_IP_PRI_0(0x01)
              ));
  /* NVIC_ISER: SETENA31=0,SETENA30=0,SETENA29=0,SETENA28=0,SETENA27=0,SETENA26=0,SETENA25=0,SETENA24=0,SETENA23=0,SETENA22=0,SETENA21=0,SETENA20=0,SETENA19=0,SETENA18=0,SETENA17=0,SETENA16=0,SETENA15=0,SETENA14=0,SETENA13=0,SETENA12=0,SETENA11=0,SETENA10=0,SETENA9=0,SETENA8=0,SETENA7=0,SETENA6=0,SETENA5=0,SETENA4=0,SETENA3=0,SETENA2=0,SETENA1=0,SETENA0=1 */
  NVIC_ISER = NVIC_ISER_SETENA0_MASK;
  /* NVIC_ICER: CLRENA31=0,CLRENA30=0,CLRENA29=0,CLRENA28=0,CLRENA27=0,CLRENA26=0,CLRENA25=0,CLRENA24=0,CLRENA23=0,CLRENA22=0,CLRENA21=0,CLRENA20=0,CLRENA19=0,CLRENA18=0,CLRENA17=0,CLRENA16=0,CLRENA15=0,CLRENA14=0,CLRENA13=0,CLRENA12=0,CLRENA11=0,CLRENA10=0,CLRENA9=0,CLRENA8=0,CLRENA7=0,CLRENA6=0,CLRENA5=0,CLRENA4=0,CLRENA3=0,CLRENA2=0,CLRENA1=0,CLRENA0=0 */
  NVIC_ICER = 0x00U;
  /* Transfer error interrupt vector(INT_DMA_Error) priority setting */
  /* NVIC_IPR1: PRI_4=1 */
  NVIC_IPR1 = (uint32_t)((NVIC_IPR1 & (uint32_t)~(uint32_t)(
               NVIC_IP_PRI_4(0x02)
              )) | (uint32_t)(
               NVIC_IP_PRI_4(0x01)
              ));
  /* NVIC_ISER: SETENA31=0,SETENA30=0,SETENA29=0,SETENA28=0,SETENA27=0,SETENA26=0,SETENA25=0,SETENA24=0,SETENA23=0,SETENA22=0,SETENA21=0,SETENA20=0,SETENA19=0,SETENA18=0,SETENA17=0,SETENA16=0,SETENA15=0,SETENA14=0,SETENA13=0,SETENA12=0,SETENA11=0,SETENA10=0,SETENA9=0,SETENA8=0,SETENA7=0,SETENA6=0,SETENA5=0,SETENA4=1,SETENA3=0,SETENA2=0,SETENA1=0,SETENA0=0 */
  NVIC_ISER = NVIC_ISER_SETENA4_MASK;
  /* NVIC_ICER: CLRENA31=0,CLRENA30=0,CLRENA29=0,CLRENA28=0,CLRENA27=0,CLRENA26=0,CLRENA25=0,CLRENA24=0,CLRENA23=0,CLRENA22=0,CLRENA21=0,CLRENA20=0,CLRENA19=0,CLRENA18=0,CLRENA17=0,CLRENA16=0,CLRENA15=0,CLRENA14=0,CLRENA13=0,CLRENA12=0,CLRENA11=0,CLRENA10=0,CLRENA9=0,CLRENA8=0,CLRENA7=0,CLRENA6=0,CLRENA5=0,CLRENA4=0,CLRENA3=0,CLRENA2=0,CLRENA1=0,CLRENA0=0 */
  NVIC_ICER = 0x00U;
  /* Enable clock gates */
  #ifdef SIM_PDD_CLOCK_GATE_DMA
  SIM_PDD_SetClockGate(SIM_BASE_PTR, SIM_PDD_CLOCK_GATE_DMA, PDD_ENABLE);
  #endif
  #ifdef SIM_PDD_CLOCK_GATE_DMAMUX
  SIM_PDD_SetClockGate(SIM_BASE_PTR, SIM_PDD_CLOCK_GATE_DMAMUX, PDD_ENABLE);
  #endif
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_DMA1_ID,DevDataPtr);
  /* Discard previous setting */
  DMA_PDD_ClearRequestEnable(DMA_BASE_PTR, DMA_PDD_ALL_CHANNELS, 0x00U); /* Disable all request from peripheral */
  DMA_PDD_ClearErrorInterruptEnable(DMA_BASE_PTR, DMA_PDD_ALL_CHANNELS, 0x00U); /* Disable all error interrupts */
  DMA_PDD_CancelTransfer(DMA_BASE_PTR);                    /* Cancel transfer (if any in progress) */
  DMA_PDD_ClearInterruptFlags(DMA_BASE_PTR, DMA_PDD_ALL_CHANNELS, 0x00U); /* Clear all channel interrupt flags */
  DMA_PDD_InitChannelsPriority(DMA_BASE_PTR,DMA1_DevConst.ChnPriorities); /* Initialize channel TCD (channel registers) */
  DMA_CR = 0x00U;                      /* initialize Control register */
  return DevDataPtr;
}

/*
** ===================================================================
**     Method      :  DMA1_InitChannel (component DMAController)
*/
/*!
**     @brief
**         The method allocates DMA channel for the DMA transfer
**         defined by the transfer descriptor input parameter
**         DescriptorPtr. This method must be called before DMA channel
**         is initialized. Otherwise ERR_NOTAVAIL error code is
**         returned after call of DMA_LDD channel methods.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         ChIntPtr        - Pointer to a DMA transfer
**                           descriptor structure.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Error code, possible codes:
**                           - ERR_OK - OK.
**                           - ERR_SPEED - This device does not work in
**                           the active clock configuration.
**                           - ERR_DISABLED - Component or device is
**                           disabled.
**                           - ERR_NOTAVAIL - There is not any possible
**                           channel with desired features to be
**                           allocated.
*/
/* ===================================================================*/
LDD_TDeviceData* DMA1_InitChannel(LDD_TDeviceData *DeviceDataPtr, DMA1_TChnInit *ChIntPtr, LDD_TUserData *UserDataPtr)
{
  uint32_t                         LogChnNum      = ChIntPtr->ChnNum;
  DMA1_TChnDevData                *ChnDevDataPtr;
  DMA1_TChnDevConst const         *ChnDevConstPtr;
  DMA1_TTCD                       *TCDPtr;
  uint8_t                         PhyChnNum;

  ChnDevDataPtr  = &((DMA1_TDeviceData *)DeviceDataPtr)->ChnDevData[LogChnNum];
  ChnDevConstPtr = &(DMA1_DevConst.ChnDevConst[LogChnNum]);
  TCDPtr         = ChnDevConstPtr->TCDPtr;
  PhyChnNum = ChnDevConstPtr->PhyChnNum;
  DMA_PDD_EnablePeripheralRequest(DMA_BASE_PTR, PhyChnNum, PDD_DISABLE); /* Disable channel peripheral request (in the DMAMUX) */
  CancelTransfer(ChnDevConstPtr);                          /* Cancel transfer (if pending) */
  *TCDPtr = ChIntPtr->TCD;                                 /* Initialize channel TCD (channel registers) */
  ChnDevDataPtr->ChnDevConstPtr = ChnDevConstPtr;          /* Remember channel constants address */
  ChnDevDataPtr->TCDPtr = TCDPtr;                          /* Remember channel TCD address */
  ChnDevDataPtr->UserDataPtr = UserDataPtr;                /* Remember channel User data pointer */
  ChnDevDataPtr->Events = ChIntPtr->Events;                /* Initialize events */
  DMA_PDD_EnableErrorInterrupt(DMA_BASE_PTR, PhyChnNum);
  DMAMUX_PDD_SetChannelSource(DMAMUX_BASE_PTR, PhyChnNum, ChIntPtr->MUX_CHCFG_Reg); /* Set channel peripheral request (in the DMAMUX) */
  return ChnDevDataPtr;
}

/*
** ===================================================================
**     Method      :  DMA1_StartTransfer (component DMAController)
*/
/*!
**     @brief
**         The method starts DMA transfer on the selected DMA channel.
**         This methods initiates explicit SW transfer request. Please
**         note that call of this method if peripheral request is
**         enabled can cause unpredictable behaviour.
**     @param
**         ChanDeviceDataPtr - DMA channel
**                           data structure pointer returned by
**                           [InitChannel()] method.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK. 
**                           - ERR_DISABLED - Component is disabled.
*/
/* ===================================================================*/
LDD_TError DMA1_StartTransfer(DMA1_TChanDeviceData *ChanDeviceDataPtr)
{
  DMA1_TChnDevData                *ChnDevDataPtr = (DMA1_TChnDevData *)ChanDeviceDataPtr;
  DMA1_TChnDevConst const         *ChnDevConstPtr = ChnDevDataPtr->ChnDevConstPtr;

  /* Start transfer */
  DMA_PDD_StartTransfer(DMA_BASE_PTR, PDD_DISABLE, ChnDevConstPtr->PhyChnNum);
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  DMA1_GetError (component DMAController)
*/
/*!
**     @brief
**         Returns last recorded channel error flags.
**     @param
**         ChanDeviceDataPtr - DMA channel
**                           data structure pointer returned by
**                           [InitChannel()] method.
**     @return
**                         - DMA Error flags.
*/
/* ===================================================================*/
LDD_DMA_TErrorFlags DMA1_GetError(DMA1_TChanDeviceData *ChanDeviceDataPtr)
{
  DMA1_TChnDevData                *ChnDevDataPtr = (DMA1_TChnDevData *)ChanDeviceDataPtr;
  LDD_DMA_TErrorFlags              ErrorFlags = ChnDevDataPtr->ErrorFlags;

  ChnDevDataPtr->ErrorFlags = 0U;                          /* Clear error mask */
  return (ErrorFlags);
}

/*
** ===================================================================
**     Method      :  DMA1_SetRequestCount (component DMAController)
*/
/*!
**     @brief
**         Sets number of request required to complete the transfer.
**     @param
**         ChanDeviceDataPtr - DMA channel
**                           data structure pointer returned by
**                           [InitChannel()] method.
**     @param
**         RequestCount    - Number of request
**                           required to complete transfer.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK. 
**                           - ERR_DISABLED - Component is disabled.
*/
/* ===================================================================*/
LDD_TError DMA1_SetRequestCount(DMA1_TChanDeviceData *ChanDeviceDataPtr, LDD_DMA_TRequestCount RequestCount)
{
  DMA1_TChnDevData                *ChnDevDataPtr = (DMA1_TChnDevData *)ChanDeviceDataPtr;
  uint8_t                         PhyChnNum = ChnDevDataPtr->ChnDevConstPtr->PhyChnNum;

  if (DMA_PDD_GetBeginningMinorLoopLinkingEnabled(DMA_BASE_PTR, PhyChnNum) != 0U) {
    DMA_PDD_SetBeginningMajorLoopCount9(DMA_BASE_PTR, PhyChnNum, RequestCount); /* Update beginning major loop count register */
    DMA_PDD_SetCurrentMajorLoopCount9(DMA_BASE_PTR, PhyChnNum, RequestCount); /* Update current major loop count register */
  } else {
    DMA_PDD_SetBeginningMajorLoopCount15(DMA_BASE_PTR, PhyChnNum, RequestCount); /* Update beginning major loop count register */
    DMA_PDD_SetCurrentMajorLoopCount15(DMA_BASE_PTR, PhyChnNum, RequestCount); /* Update current major loop count register */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  HandleInterrupt (component DMAController)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HandleInterrupt(DMA1_TChnDevData *ChnDevDataPtr)
{
  DMA1_TChnDevConst const         *ChnDevConstPtr = ChnDevDataPtr->ChnDevConstPtr;
  uint8_t                         PhyChnNum = ChnDevConstPtr->PhyChnNum;
  uint32_t                         ErrorStatus;

  if (DMA_PDD_GetChannelErrorFlag(DMA_BASE_PTR, PhyChnNum)) {
    /* Channel error detected */
    /* Transfer error */
    ErrorStatus = DMA_PDD_GetErrorStatusFlags(DMA_BASE_PTR); /* Get error flags */
    if (ErrorStatus != 0U) {                               /* Any error flag set? */
      if (DMA_PDD_GetErrorStatusChannel(DMA_BASE_PTR) == ChnDevConstPtr->PhyChnNum) { /* Channel error flags valid? */
        /* Remember all (device and channel) error flags */
        ChnDevDataPtr->ErrorFlags |= ErrorStatus;
      } else {
        /* Remember device error flags only (if any flag set) */
        ChnDevDataPtr->ErrorFlags |= (ErrorStatus & DMA_PDD_DEVICE_ERROR_FLAGS);
        /* Remember channel error flags are not valid */
        ChnDevDataPtr->ErrorFlags |= LDD_DMA_UNKNOWN_ERROR;
      }
    } else {
      /* Error flags not valid */
      ChnDevDataPtr->ErrorFlags |= LDD_DMA_UNKNOWN_ERROR;
    }
    DMAMUX_PDD_EnableChannel(DMAMUX_BASE_PTR, ChnDevConstPtr->PhyChnNum, PDD_DISABLE);
    DMA_PDD_ClearChannelErrorFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
    if (ChnDevDataPtr->Events.OnErrorFnPtr != NULL) {
      ChnDevDataPtr->Events.OnErrorFnPtr(ChnDevDataPtr->UserDataPtr);
    }
  } else if (DMA_PDD_GetDoneFlag(DMA_BASE_PTR, PhyChnNum)) {
    /* Transfer done without error */
    DMA_PDD_ClearChannelInterruptFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
    DMA_PDD_ClearDoneFlag(DMA_BASE_PTR, ChnDevConstPtr->PhyChnNum);
    if (ChnDevDataPtr->Events.OnCompleteFnPtr != NULL) {
      ChnDevDataPtr->Events.OnCompleteFnPtr(ChnDevDataPtr->UserDataPtr);
    }
  } else {
  }
}

/*
** ===================================================================
**     Method      :  DMA1_TransferComplete_ISR (component DMAController)
**
**     Description :
**         Transfer complete ISR
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(DMA1_INT_DMA0_TransferComplete_ISR)
{
  /* {Default RTOS Adapter} ISR parameter is passed through the global variable */
  DMA1_TDeviceDataPtr DevDataPtr = INT_DMA0__DEFAULT_RTOS_ISRPARAM;
  HandleInterrupt(&(DevDataPtr->ChnDevData[0]));
}

/*
** ===================================================================
**     Method      :  DMA1_TransferComplete_ISR (component DMAController)
**
**     Description :
**         Transfer complete ISR
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(DMA1_INT_DMA_Error_TransferError_ISR)
{
  /* {Default RTOS Adapter} ISR parameter is passed through the global variable */
  DMA1_TDeviceDataPtr DevDataPtr = INT_DMA_Error__DEFAULT_RTOS_ISRPARAM;
  HandleInterrupt(&(DevDataPtr->ChnDevData[0]));
}

/* END DMA1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
