/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : SCI.C
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

/* MODULE SCI. */

#include "SCI.h"


#define OVERRUN_ERR      1             /* Overrun error flag bit   */
#define FRAMING_ERR      2             /* Framing error flag bit   */
#define PARITY_ERR       4             /* Parity error flag bit    */
#define CHAR_IN_RX       8             /* Char is in RX buffer     */
#define FULL_TX          16            /* Full transmit buffer     */
#define RUNINT_FROM_TX   32            /* Interrupt is in progress */
#define FULL_RX          64            /* Full receive buffer      */
#define NOISE_ERR        128           /* Noise erorr flag bit     */
#define IDLE_ERR         256           /* Idle character flag bit  */
#define BREAK_ERR        512           /* Break detect             */

static word SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Framing error */
                                       /*       2 - Parity error (not used in this bean) */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Unused */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Noise error */
                                       /*       8 - Idle character  */
                                       /*       9 - Break detected  */
                                       /*      10 - Unused */
static int16_t SerSlv;                 /* Number of selected slave mode */

/*
** ===================================================================
**     Method      :  HWEnDi (component AsynchroMaster)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the bean.
**         The method is called automatically as a part of the Enable and 
**         Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  setRegBits(SCI_SCICR, (SCI_SCICR_TE_MASK | SCI_SCICR_RE_MASK)); /* Enable device */
}

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
byte SCI_RecvChar(SCI_TComData *Chr)
{
  register byte Result = ERR_OK;       /* Return error code */
  register word StatReg = getReg(SCI_SCISR); /* Read status register */

  setReg(SCI_SCISR, 0);                /* Reset error request flags */
  if (StatReg & (SCI_SCISR_OR_MASK|SCI_SCISR_NF_MASK|SCI_SCISR_FE_MASK|SCI_SCISR_PF_MASK)) { /* Is any error set? */
    Result = ERR_COMMON;               /* If yes then set common error value */
  } else if (!(StatReg & SCI_SCISR_RDRF_MASK)) { /* Is the reciver empty and no error is set? */
    return ERR_RXEMPTY;                /* If yes then error */
  }
  *Chr = (SCI_TComData)getReg(SCI_SCIDR); /* Read data from the receiver */
  return Result;                       /* Return error code */
}

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
byte SCI_SendChar(SCI_TComData Chr)
{
  if (!getRegBit(SCI_SCISR, TDRE)) {   /* Is the transmitter empty? */
    return ERR_TXFULL;
  }
  setReg(SCI_SCIDR, Chr);              /* Store char to transmitter register */
  return ERR_OK;                       /* OK */
}

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
word SCI_GetCharsInRxBuf(void)
{
  return (word)((getRegBit(SCI_SCISR, RDRF) != 0)? 1 : 0); /* Return number of chars in receive buffer */
}

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
word SCI_GetCharsInTxBuf(void)
{
  return (word)((getRegBit(SCI_SCISR, TDRE) == 0)? 1 : 0); /* Return number of chars in the transmitter buffer */
}

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
void SCI_Init(void)
{
  SerFlag = 0;                         /* Reset all flags */
  SerSlv = NOSLAVE;                    /* No slave is selected */
  /* SCI_SCICR: LOOP=0,SWAI=0,RSRC=0,M=1,WAKE=0,POL=0,PE=0,PT=0,TEIE=0,TIIE=0,RFIE=0,REIE=0,TE=0,RE=0,RWU=0,SBK=0 */
  setReg(SCI_SCICR, 4096);             /* Set the SCI configuration */
  /* SCI_SCIBR: ??=0,??=0,??=0,SBR=17 */
  setReg(SCI_SCIBR, 17);               /* Set prescaler bits */
  HWEnDi();                            /* Enable/disable device according to status flags */
}

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
byte SCI_SetIdle(void)
{
  clrRegBit(SCI_SCICR, TE);            /* Disable the transmitter */
  setRegBit(SCI_SCICR, TE);            /* Enable the transmitter */
  return ERR_OK;                       /* OK */
}

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
byte SCI_SelectSlave(byte Slv)
{
  if (SerFlag & FULL_TX) {             /* Is any char in the transmitter? */
    return ERR_BUSY;                   /* If yes then error */
  }
  SerSlv = (int16_t)Slv;               /* Save identification of the selected slave */
  EnterCritical();                     /* Disable global interrupts */
  setReg(SCI_SCIDR, Slv | 256);        /* Store char to the transmitter register */
  ExitCritical();                      /* Enable global interrupts */
  return ERR_OK;                       /* OK */
}

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
byte SCI_GetSelected(byte *Slv)
{
  if (SerSlv == NOSLAVE) {             /* Is any slave selected? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Slv = (byte)SerSlv;                 /* Return address of the selected slave */
  return ERR_OK;                       /* OK */
}


/* END SCI. */


/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
