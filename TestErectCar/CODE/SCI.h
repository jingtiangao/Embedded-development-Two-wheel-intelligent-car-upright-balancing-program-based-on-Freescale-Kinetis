/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : SCI.H
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : AsynchroMaster
**     Version   : Component 02.261, Driver 02.03, CPU db: 2.87.097
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-1, 8:34
**     Abstract  :
**         This bean "AsynchroMaster" implements MASTER part of asynchronous
**         serial master-slave communication.
**     Settings  :
**         Asynchro type               : MASTER
**
**         Serial channel              : SCI
**
**         Protocol
**             Init baud rate          : 117647_058824baud
**             Width                   : 9 bits
**             Stop bits               : 0
**             Parity                  : hardware wake-up
**             Breaks                  : Disabled
**             Input buffer size       : 0
**             Output buffer size      : 0
**
**         Registers
**             Input buffer            : SCI_SCIDR [61620]
**             Output buffer           : SCI_SCIDR [61620]
**             Control register        : SCI_SCICR [61617]
**             Mode register           : SCI_SCICR [61617]
**             Baud setting reg.       : SCI_SCIBR [61616]
**
**
**
**         Used pins:
**         ----------------------------------------------------------
**           Function | On package           |    Name
**         ----------------------------------------------------------
**            Input   |     1                |  GPIOB6_RXD_SDA_CLKIN
**            Output  |     3                |  GPIOB7_TXD_SCL
**         ----------------------------------------------------------
**
**     Contents  :
**         RecvChar        - byte SCI_RecvChar(SCI_TComData *Chr);
**         SendChar        - byte SCI_SendChar(SCI_TComData Chr);
**         GetCharsInRxBuf - word SCI_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word SCI_GetCharsInTxBuf(void);
**         SelectSlave     - byte SCI_SelectSlave(byte Slv);
**         GetSelected     - byte SCI_GetSelected(byte *Slv);
**         SetIdle         - byte SCI_SetIdle(void);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __SCI
#define __SCI

/* MODULE SCI. */

#include "Cpu.h"

#define NOSLAVE -1

#ifndef __BWUserType_SCI_TError
#define __BWUserType_SCI_TError
  typedef union {
    byte err;
    struct {
      bool OverRun  : 1;               /* Overrun error flag */
      bool Framing  : 1;               /* Framing error flag */
      bool Parity   : 1;               /* Parity error flag */
      bool RxBufOvf : 1;               /* Rx buffer full error flag */
      bool Noise    : 1;               /* Noise error flag */
      bool Break    : 1;               /* Break detect */
      bool LINSync  : 1;               /* LIN synchronization error */
    } errName;
  } SCI_TError;                        /* Error flags. For languages which don't support bit access is byte access only to error flags possible. */
#endif

#ifndef __BWUserType_SCI_TComData
#define __BWUserType_SCI_TComData
  typedef byte SCI_TComData;           /* User type for communication. Size of this type depends on the communication data witdh. */
#endif

byte SCI_RecvChar(SCI_TComData *Chr);
/*
** ===================================================================
**     Method      :  SCI_RecvChar (component AsynchroMaster)
**
**     Description :
**         If any data is received, this method returns one
**         character, otherwise it returns an error code (it does
**         not wait for data). This method is enabled only if the
**         receiver property is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence
**         the information about an exception in interrupt mode is
**         returned only if there is a valid character ready to be
**         read.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service>
**                           property is disabled and the <Break
**                           signal> property is enabled)
**                           ERR_COMMON - common error occurred (the
**                           GetError method can be used for error
**                           specification)
** ===================================================================
*/

byte SCI_SendChar(SCI_TComData Chr);
/*
** ===================================================================
**     Method      :  SCI_SendChar (component AsynchroMaster)
**
**     Description :
**         Send one character to the channel. This method is
**         available only if the transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/

word SCI_GetCharsInRxBuf(void);
/*
** ===================================================================
**     Method      :  SCI_GetCharsInRxBuf (component AsynchroMaster)
**
**     Description :
**         Returns the number of characters in the input buffer.
**         This method is available only if the receiver property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the input
**                           buffer.
** ===================================================================
*/

word SCI_GetCharsInTxBuf(void);
/*
** ===================================================================
**     Method      :  SCI_GetCharsInTxBuf (component AsynchroMaster)
**
**     Description :
**         Returns the number of characters in the output buffer.
**         This method is available only if the transmitter property
**         is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the output
**                           buffer.
** ===================================================================
*/

byte SCI_SelectSlave(byte Slv);
/*
** ===================================================================
**     Method      :  SCI_SelectSlave (component AsynchroMaster)
**
**     Description :
**         Select the slave - sends the address of the slave for
**         starting communication with it (the previous slave stops
**         communicating).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Slv             - The new slave identification
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - This device is disabled
**                           ERR_BUSY - Transmitter buffer is not
**                           empty
** ===================================================================
*/

byte SCI_GetSelected(byte *Slv);
/*
** ===================================================================
**     Method      :  SCI_GetSelected (component AsynchroMaster)
**
**     Description :
**         Return the identification of the selected slave  (0-254,
**         or -1 if no slave is selected).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Slv             - Pointer to identification of the slave
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Slave is not selected yet
** ===================================================================
*/

void SCI_Init(void);
/*
** ===================================================================
**     Method      :  SCI_Init (component AsynchroMaster)
**
**     Description :
**         Initializes the associated peripheral(s) and internal 
**         variables of the bean. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

byte SCI_SetIdle(void);
/*
** ===================================================================
**     Method      :  SCI_SetIdle (component AsynchroMaster)
**
**     Description :
**         Sends the idle character to the output line.
**         This method is available only if the transmitter property
**         is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - This bean is disabled by
**                           user
** ===================================================================
*/



/* END SCI. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/

#endif /* ifndef __SCI */
