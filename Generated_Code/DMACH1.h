/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : DMACH1.h
**     Project     : ModuleHub
**     Processor   : MKV10Z16VFM7
**     Component   : DMAChannel_LDD
**     Version     : Component 01.053, Driver 01.01, CPU db: 3.50.001
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-01, 20:53, # CodeGen: 1
**     Abstract    :
**          This embedded component implements
**          a DMA transfer channel descriptor definition.
**     Settings    :
**          Component name                                 : DMACH1
**          Hardware request                               : Disabled
**          Channel select                                 : Fixed
**            DMA controller device                        : DMA1
**            Channel                                      : DMA_Channel0
**            Allocate channel                             : yes
**            Arbitration type                             : Fixed
**              Priority                                   : 0
**            Interrupt service                            : Enabled
**              Transfer complete interrupt                : 
**                Interrupt vector                         : INT_DMA0
**                Interrupt priority                       : 64
**                ISR Name                                 : DMA1_INT_DMA0_TransferComplete_ISR
**              Error interrupt                            : 
**                Interrupt vector                         : INT_DMA_Error
**                Interrupt priority                       : medium priority
**                ISR Name                                 : DMA1_INT_DMA_Error_TransferError_ISR
**          External object declaration                    : (string list)
**          Source transaction settings                    : 
**            Start address                                : 0
**            Transaction size                             : 8-bits
**            Address adjustment                           : 0
**            Address modulo                               : Buffer disabled
**          Destination transaction settings               : 
**            Start address                                : 0
**            Transaction size                             : 8-bits
**            Address adjustment                           : 0
**            Address modulo                               : Buffer disabled
**          Transfer settings                              : 
**            Bandwidth control                            : No stalls
**            Transaction size                             : 8-bits
**            Transactions count                           : 1
**            Request count                                : 1
**            After request complete actions               : 
**              Channel linking                            : Disabled
**              Address adjustment                         : Disabled
**            After transfer complete actions              : 
**              Disable peripheral request                 : no
**              Channel linking                            : Disabled
**              Address adjustment                         : Disabled
**              Scatter/gather                             : Disabled
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Half complete                                : Disabled
**            Event mask                                   : 
**              OnComplete                                 : Enabled
**              OnError                                    : Enabled
**     Contents    :
**         Init            - LDD_TDeviceData* DMACH1_Init(LDD_TUserData *UserDataPtr);
**         StartTransfer   - LDD_TError DMACH1_StartTransfer(LDD_TDeviceData *DeviceDataPtr);
**         GetError        - LDD_DMA_TErrorFlags DMACH1_GetError(LDD_TDeviceData *DeviceDataPtr);
**         SetRequestCount - LDD_TError DMACH1_SetRequestCount(LDD_TDeviceData *DeviceDataPtr,...
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
** @file DMACH1.h
** @version 01.01
** @brief
**          This embedded component implements
**          a DMA transfer channel descriptor definition.
*/         
/*!
**  @addtogroup DMACH1_module DMACH1 module documentation
**  @{
*/         

#ifndef __DMACH1_H
#define __DMACH1_H

/* MODULE DMACH1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "DMA1.h"
#include "PE_LDD.h"
#include "DMA_PDD.h"

#include "Cpu.h"

/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define DMACH1_PRPH_BASE_ADDRESS  0x40008000U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define DMACH1_Init_METHOD_ENABLED     /*!< Init method of the component DMACH1 is enabled (generated) */
#define DMACH1_StartTransfer_METHOD_ENABLED /*!< StartTransfer method of the component DMACH1 is enabled (generated) */
#define DMACH1_GetError_METHOD_ENABLED /*!< GetError method of the component DMACH1 is enabled (generated) */
#define DMACH1_SetRequestCount_METHOD_ENABLED /*!< SetRequestCount method of the component DMACH1 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */
#define DMACH1_OnComplete_EVENT_ENABLED /*!< OnComplete event of the component DMACH1 is enabled (generated) */
#define DMACH1_OnError_EVENT_ENABLED   /*!< OnError event of the component DMACH1 is enabled (generated) */

/* Source circular buffer base address mask. Represents required 0-modulo-size mask of base source address when using circular buffer. */
#define DMACH1_SOURCE_CIRCULAR_BUFFER_ADDRESS_MASK 0U
/* Destination circular buffer base address mask. Represents required 0-modulo-size mask of base destination address when using circular buffer. */
#define DMACH1_DESTINATION_CIRCULAR_BUFFER_ADDRESS_MASK 0U

/*
** ===================================================================
**     Method      :  DMACH1_Init (component DMAChannel_LDD)
*/
/*!
**     @brief
**         Initializes the DMAChannel_LDD component. This method
**         allocates no memory. Memory for all DMA used channles is
**         allocated by DMAController componet .
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* DMACH1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  DMACH1_StartTransfer (component DMAChannel_LDD)
*/
/*!
**     @brief
**         The method starts DMA transfer on the selected DMA channel.
**         This methods initiates explicit SW transfer request. Please
**         note that call of this method if peripheral request is
**         enabled can cause unpredictable behaviour.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK. 
**                           - ERR_DISABLED - Component is disabled.
*/
/* ===================================================================*/
LDD_TError DMACH1_StartTransfer(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  DMACH1_GetError (component DMAChannel_LDD)
*/
/*!
**     @brief
**         Returns channel error flags and clears these flags. 
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - DMA Error flags.
*/
/* ===================================================================*/
LDD_DMA_TErrorFlags DMACH1_GetError(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  DMACH1_SetRequestCount (component DMAChannel_LDD)
*/
/*!
**     @brief
**         Sets number of requests required to complete transfer.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         RequestCount    - Number of requests
**                           required to complete transfer.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK. 
**                           - ERR_DISABLED - Component is disabled.
*/
/* ===================================================================*/
LDD_TError DMACH1_SetRequestCount(LDD_TDeviceData *DeviceDataPtr, LDD_DMA_TRequestCount RequestCount);

/* END DMACH1. */
#endif
/* ifndef __DMACH1_H */
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
