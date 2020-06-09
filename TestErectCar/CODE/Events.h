/** ###################################################################
**     Filename  : Events.H
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : Events
**     Version   : Driver 01.03
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-1, 8:34
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         TI1_OnInterrupt     - void TI1_OnInterrupt(void);
**         ADC_OnEnd           - void ADC_OnEnd(void);
**         I2C1_OnReceiveData  - void I2C1_OnReceiveData(void);
**         I2C1_OnTransmitData - void I2C1_OnTransmitData(void);
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "SCI.h"
#include "I2C1.h"
#include "ADC.h"
#include "TI1.h"
#include "PWMC1.h"
#include "COUNTER1.h"
#include "COUNTER2.h"
#include "MOTOR1DIR.h"
#include "MOTOR2DIR.h"
#include "MOTOR1DIRVAL.h"
#include "MOTOR2DIRVAL.h"
#include "RC1.h"
#include "RC2.h"
#include "RC3.h"
#include "RC4.h"
#include "LED.h"

void TI1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void ADC_OnEnd(void);
/*
** ===================================================================
**     Event       :  ADC_OnEnd (module Events)
**
**     Component   :  ADC [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void MOTOR2DIR_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  MOTOR2DIR_OnInterrupt (module Events)
**
**     Component   :  MOTOR2DIR [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MOTOR1DIR_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  MOTOR1DIR_OnInterrupt (module Events)
**
**     Component   :  MOTOR1DIR [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
