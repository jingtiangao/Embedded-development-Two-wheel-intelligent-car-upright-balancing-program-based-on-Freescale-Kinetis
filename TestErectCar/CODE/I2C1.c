/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : I2C1.C
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : InternalI2C
**     Version   : Component 01.268, Driver 02.03, CPU db: 2.87.097
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-1, 16:32
**     Abstract  :
**          This component encapsulates the internal I2C communication 
**          interface. The implementation of the interface is based 
**          on the Philips I2C-bus specification version 2.0. 
**          Interface features:
**          MASTER mode
**            - Multi master communication
**            - The combined format of communication possible 
**              (see "Automatic stop condition" property)
**            - 7-bit slave addressing (10-bit addressing can be made as well)
**            - Acknowledge polling provided
**            - No wait state initiated when a slave device holds the SCL line low
**            - Holding of the SCL line low by slave device recognized as 'not available bus'
**            - Invalid start/stop condition detection provided
**          SLAVE mode
**            - 7-bit slave addressing
**            - General call address detection provided
**     Settings  :
**         Serial channel              : I2C
**
**         Protocol
**             Mode                    : MASTER
**             Auto stop condition     : yes
**             SCL frequency           : 200 kHz
**
**         Initialization
**
**             Target slave address    : 76
**             Bean function           : Enabled
**             Events                  : Enabled
**
**         Registers
**             Input buffer            : IBDR      [61652]
**             Output buffer           : IBDR      [61652]
**             Control register        : IBCR      [61650]
**             Status register         : IBSR      [61651]
**             Baud setting reg.       : IBFD      [61649]
**             Address register        : IBAD      [61648]
**
**
**         Used pins                   :
**       ----------------------------------------------------------
**            Function    | On package |    Name
**       ----------------------------------------------------------
**              SDA       |     2      |  GPIOB1_SS_B_SDA
**              SCL       |     21     |  GPIOB0_SCLK_SCL
**       ----------------------------------------------------------
**     Contents  :
**         SendChar    - byte I2C1_SendChar(byte Chr);
**         RecvChar    - byte I2C1_RecvChar(byte *Chr);
**         SendBlock   - byte I2C1_SendBlock(void* Ptr, word Siz, word *Snt);
**         RecvBlock   - byte I2C1_RecvBlock(void* Ptr, word Siz, word *Rcv);
**         SelectSlave - byte I2C1_SelectSlave(byte Slv);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE I2C1. */


#include "I2C1.h"

#define OVERRUN_ERR      1             /* Overrun error flag bit   */
#define WAIT_RX_CHAR     2             /* Wait for received char. flag bit (Master)  */
#define CHAR_IN_TX       4             /* Char is in TX buffer (Master)    */
#define CHAR_IN_RX       8             /* Char is in RX buffer     */
#define FULL_TX          16            /* Full transmit buffer     */
#define IN_PROGRES       32            /* Communication is in progress (Master) */
#define FULL_RX          64            /* Full receive buffer      */
#define MSxSL            128           /* Master x Slave flag bit  */

static byte I2C1_SlaveAddr;            /* Variable for Slave address */
static word InpLenM;                   /* Length of input bufer's content */
static byte *InpPtrM;                  /* Pointer to input buffer for Master mode */
static word OutLenM;                   /* Length of output bufer's content */
static byte *OutPtrM;                  /* Pointer to output buffer for Master mode */
volatile word I2C1_SndRcvTemp;         /* Temporary variable for SendChar (RecvChar) when they call SendBlock (RecvBlock) */
static word *PtrSndRcv;                /* Pointer to Snd/Rcv counter for SendBlock/RecvBlock */
static byte ChrTemp;                   /* Temporary variable for SendChar method */
volatile byte I2C1_SerFlag;            /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Wait for received char. flag bit (Master) */
                                       /*       2 - Char is in TX buffer (Master) */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Master x Slave */

/*
** ===================================================================
**     Method      :  MainComm (component InternalI2C)
**
**     Description :
**         The method services the interrupt flags if Interrupt service 
**         is disabled.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define RXAK 1
#define SRW  4
#define IBAL 16
#define IAAS 64
static byte MainComm(void)
{
  register word Tr;
  register word Status;                /* Temporary variable for saving status register */

  *PtrSndRcv = 0;                      /* Clear Snd/Rcv counter */
  for (Tr=0; Tr<40000;Tr++) {
    if (getRegBit(IBSR,IBIF)) {
      Status = getReg(IBSR);           /* Save status register */
      setRegBit(IBSR,IBIF);            /* Clear interrupt flag */
      Tr = (word)-1;                   /* Clear trials */
      if (getRegBit(IBCR,MS_SL)) {     /* Is device in master mode? */
        if (getRegBit(IBCR,TX_RX)) {   /* Is device in Tx mode? */
          if (Status & IBSR_RXAK_MASK) { /* NACK received? */
            clrRegBit(IBCR,MS_SL);     /* Switch device to slave mode (stop signal sent) */
            clrRegBit(IBCR,TX_RX);     /* Switch to Rx mode */
            OutLenM = 0;               /* Any character is not for sent */
            InpLenM = 0;               /* Any character is not for reception */
            I2C1_SerFlag &= ~(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES); /* Any character is not for sent or reception*/
            return ERR_BUSY;           /* Return with error */
          }
          else {
            if (OutLenM) {             /* Is any char. for transmitting? */
              OutLenM--;               /* Decrease number of chars for the transmit */
              setReg(IBDR,*(OutPtrM)++); /* Send character */
              (*PtrSndRcv)++;          /* Increment Snd counter */
            }
            else {
              if (InpLenM) {           /* Is any char. for reception? */
                if (InpLenM == 1)      /* If only one char to receive */
                  setRegBit(IBCR,TXAK); /* then transmit ACK disable */
                else
                  clrRegBit(IBCR,TXAK); /* else transmit ACK enable */
                clrRegBit(IBCR,TX_RX); /* Switch to Rx mode */
                getReg(IBDR);          /* Dummy read character */
              }
              else {
                clrRegBit(IBCR,MS_SL); /* Switch device to slave mode (stop signal sent) */
                clrRegBit(IBCR,TX_RX); /* Switch to Rx mode */
                for (Tr=40000;Tr!=0;Tr--) {
                  if (!getRegBit(IBSR,IBB)) { /* Bus is busy? */
                    return ERR_OK;     /* Return without error */
                  }
                }
                return ERR_BUSOFF;     /* Return with error */
              }
            }
          }
        }
        else {
          InpLenM--;                   /* Decrease number of chars for the receive */
          if (InpLenM) {               /* Is any char. for reception? */
            if (InpLenM == 1)
              setRegBit(IBCR,TXAK);    /* Transmit ACK disable */
          }
          else {
            clrRegBit(IBCR,MS_SL);     /* If no, switch device to slave mode (stop signal sent) */
          }
          *(InpPtrM)++ = (byte)getReg(IBDR); /* Receive character */
          (*PtrSndRcv)++;              /* Increment Rcv counter */
          if (!InpLenM) {              /* Is any char. for reception? */
            for (Tr=40000;Tr!=0;Tr--) {
              if (!getRegBit(IBSR,IBB)) { /* Bus is busy? */
                return ERR_OK;         /* Return without error */
              }
            }
            return ERR_BUSOFF;         /* Return with error */
          }
        }
      }
      else {
        if (Status & IBSR_IBAL_MASK) { /* Arbitration lost? */
          OutLenM = 0;                 /* Any character is not for sent */
          InpLenM = 0;                 /* Any character is not for reception */
          I2C1_SerFlag &= ~(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES); /* Any character is not for sent or reception*/
          clrRegBit(IBCR,TX_RX);       /* Switch to Rx mode */
          return ERR_ARBITR;           /* Return with error */
        }
      }
    }
  }
  return ERR_BUSOFF;                   /* Return with error */
}

/*
** ===================================================================
**     Method      :  I2C1_SendChar (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address
**         bytes inclusive of R/W bit = 0 to the I2C bus and then
**         writes one character (byte) to the bus. The slave address
**         must be specified before, by the "SelectSlave" or
**         "SelectSlave10" method or in the bean initialization
**         section, "Target slave address init" property. If
**         interrupt service is enabled and the method returns
**         ERR_OK, it doesn't mean that transmission was successful.
**         The state of transmission is obtainable from
**         (OnTransmitData, OnError or OnArbitLost) events. 
**         When working as a SLAVE, this method writes a character
**         to the internal output slave buffer and, after the master
**         starts the communication, to the I2C bus. If no character
**         is ready for a transmission (internal output slave buffer
**         is empty), the Empty character will be sent (see "Empty
**         character" property).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only
**                           in master mode and when interrupt
**                           service is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full (slave
**                           mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in
**                           master mode)
** ===================================================================
*/
byte I2C1_SendChar(byte Chr)
{
  if ((getRegBit(IBSR,IBB))||(InpLenM)||(I2C1_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  ChrTemp = Chr;                       /* Save character */
  return (I2C1_SendBlock(&ChrTemp, 1, (word*)&I2C1_SndRcvTemp)); /* Send character and return */
}

/*
** ===================================================================
**     Method      :  I2C1_RecvChar (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address
**         bytes inclusive of R/W bit = 1 to the I2C bus, then reads
**         one character (byte) from the bus and then sends the stop
**         condition. The slave address must be specified before, by
**         the "SelectSlave" or "SelectSlave10" method or in bean
**         initialization section, property "Target slave address
**         init". If interrupt service is enabled and the method
**         returns ERR_OK, it doesn't mean that transmission was
**         finished successfully. The state of transmission must be
**         tested by means of events (OnReceiveData, OnError or
**         OnArbitLost). In case of successful transmission,
**         received data is ready after OnReceiveData event is
**         called. 
**         When working as a SLAVE, this method reads a character
**         from the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Received character.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge
**                           (only in master mode and when interrupt
**                           service is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - No data in receiver (slave
**                           mode only)
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last character or block
**                           received (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in
**                           master mode)
**                           ERR_NOTAVAIL - Method is not available
**                           in current mode - see generated code
**                           comment
** ===================================================================
*/
/*
byte I2C1_RecvChar(byte *Chr)

**  This method is implemented as a macro. See I2C1.h file.  **
*/

/*
** ===================================================================
**     Method      :  I2C1_SendBlock (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address
**         bytes inclusive of R/W bit = 0 to the I2C bus and then
**         writes the block of characters to the bus. The slave
**         address must be specified before, by the "SelectSlave" or
**         "SlaveSelect10" method or in bean initialization section,
**         "Target slave address init" property. If interrupt
**         service is enabled and the method returns ERR_OK, it
**         doesn't mean that transmission was successful. The state
**         of transmission is detectable by means of events
**         (OnTransmitData, OnError or OnArbitLost). Data to be send
**         is not copied to an internal buffer and remains in the
**         original location. Therefore the content of the buffer
**         should not be changed until the transmission is complete.
**         Event OnTransmitData can be used to detect the end of the
**         transmission.
**         When working as a SLAVE, this method writes a block of
**         characters to the internal output slave buffer and then,
**         after the master starts the communication, to the I2C bus.
**         If no character is ready for a transmission (internal
**         output slave buffer is empty), the "Empty character" will
**         be sent (see "Empty character" property). In SLAVE mode
**         the data are copied to an internal buffer, if specified
**         by "Output buffer size" property.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send.
**         Siz             - Size of the block.
**       * Snt             - Amount of data sent (moved to a buffer).
**                           In master mode, if interrupt support is
**                           enabled, the parameter always returns
**                           the same value as the parameter 'Siz' of
**                           this method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge
**                           (only in master mode and when interrupt
**                           service is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full. Some
**                           data has not been sent. (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in
**                           master mode)
** ===================================================================
*/
byte I2C1_SendBlock(void* Ptr,word Siz,word *Snt)
{
  if (!Siz) {                          /* Test variable Size on zero */
    *Snt = 0;
    return ERR_OK;                     /* If zero then OK */
  }
  if ((getRegBit(IBSR,IBB))||(InpLenM)||(I2C1_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  PtrSndRcv = Snt;                     /* Safe Snd pointer */
  OutLenM = Siz;                       /* Set lenght of data */
  OutPtrM = (byte *)Ptr;               /* Save pointer to data for transmitting */
  setRegBit(IBCR,TX_RX);               /* Set TX mode */
  if (getRegBit(IBCR,MS_SL)) {         /* Is device in master mode? */
    setRegBit(IBCR,RSTA);              /* If yes then repeat start cycle generated */
  }
  else {
    setRegBit(IBCR,MS_SL);             /* If no then start signal generated */
  }
  setReg(IBDR,I2C1_SlaveAddr);         /* Send slave address */
  return (MainComm());                 /* Call main communication method and return */
}

/*
** ===================================================================
**     Method      :  I2C1_RecvBlock (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address
**         bytes inclusive of R/W bit = 1 to the I2C bus, then reads
**         the block of characters from the bus and then sends the
**         stop condition. The slave address must be specified
**         before, by the "SelectSlave" or "SelectSlave10" method or
**         in bean initialization section, "Target slave address
**         init" property. If interrupt service is enabled and the
**         method returns ERR_OK, it doesn't mean that transmission
**         was finished successfully. The state of transmission must
**         be tested by means of events (OnReceiveData, OnError or
**         OnArbitLost). In case of successful transmission,
**         received data is ready after OnReceiveData event is
**         called. 
**         When working as a SLAVE, this method reads a block of
**         characters from the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block space for
**                           received data.
**         Siz             - The size of the block.
**       * Rcv             - Amount of received data. In master mode,
**                           if interrupt support is enabled, the
**                           parameter always returns the same value
**                           as the parameter 'Siz' of this method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only
**                           in master mode and when interrupt
**                           service is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data.
**                           Only available data (or no data) has
**                           been returned  (slave mode only).
**                           ERR_OVERRUN - Overrun error was detected
**                           from last character or block receiving
**                           (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in
**                           master mode)
** ===================================================================
*/
byte I2C1_RecvBlock(void* Ptr,word Siz,word *Rcv)
{
  if (!Siz) {                          /* Test variable Size on zero */
    *Rcv = 0;
    return ERR_OK;                     /* If zero then OK */
  }
  if ((getRegBit(IBSR,IBB))||(InpLenM)||(I2C1_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  PtrSndRcv = Rcv;                     /* Safe Rcv pointer */
  InpLenM = Siz;                       /* Set lenght of data */
  InpPtrM = (byte *)Ptr;               /* Save pointer to data for reception */
  setRegBit(IBCR,TX_RX);               /* Set TX mode */
  if (getRegBit(IBCR,MS_SL)) {         /* Is device in master mode? */
    setRegBit(IBCR,RSTA);              /* If yes then repeat start cycle generated */
  }
  else {
    setRegBit(IBCR,MS_SL);             /* If no then start signal generated */
  }
  setReg(IBDR,(byte)(I2C1_SlaveAddr+1)); /* Send slave address */
  return (MainComm());                 /* Call main communication method and return */
}

/*
** ===================================================================
**     Method      :  I2C1_SelectSlave (component InternalI2C)
**
**     Description :
**         This method selects a new slave for communication by its
**         7-bit slave address value. Any send or receive method
**         directs to or from selected device, until a new slave
**         device is selected by this method. This method is not
**         available for the SLAVE mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Slv             - 7-bit slave address value.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_BUSY - The device is busy, wait
**                           until the current operation is finished.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  The device is disabled
** ===================================================================
*/
byte I2C1_SelectSlave(byte Slv)
{
  if (getRegBit(IBCR,MS_SL)) {         /* Is the device in the active state? */
    return ERR_BUSY;                   /* If yes then error */
  }
  I2C1_SlaveAddr = (byte)(Slv << 1);   /* Set slave address */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  I2C1_Init (component InternalI2C)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void I2C1_Init(void)
{
  /* IBCR: IBEN=0,IBIE=0,MS_SL=0,TX_RX=0,TXAK=0,RSTA=0,??=0,??=0 */
  setReg(IBCR,0);                      /* Clear control register */
  I2C1_SerFlag = 128;                  /* Reset all flags */
  I2C1_SlaveAddr = 152;                /* Set variable for slave address */
  setReg(IBNR,0);                      /* Set Noise filter register */
  /* IBFD: IBC=135 */
  setReg(IBFD,135);                    /* Set prescaler bits */
  setRegBit(IBCR,IBEN);                /* Enable device */
}


/* END I2C1. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
