/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : RC1.H
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : InputPin
**     Version   : Component 01.005, Driver 01.12, CPU db: 2.87.097
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-8, 11:11
**     Abstract  :
**         This bean implements a one-bit input. Use of the bean 
**         is convenient if selected pin is used by some on-chip 
**         peripheral device (asychronous serial channel, 
**         synchronous serial channel,...) and there is some reason 
**         to read state on this pin. This is why bean neither 
**         allocate nor inicialize this pin. This bean can be used 
**         for autodiagnostics, where on-chip peripheral device is 
**         verified on functionality, for example sending and 
**         receiving of data by asychronous serial channel or 
**         synchronous serial channel or generating periodic signal 
**         by Timer.
**     Settings  :
**
**     Contents  :
**         GetVal - bool RC1_GetVal(void);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __RC1
#define __RC1

/* MODULE RC1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "Cpu.h"


#define RC1_PIN_MASK            ((byte)8) /* Pin mask */

/*
** ===================================================================
**     Method      :  RC1_GetVal (component InputPin)
**
**     Description :
**         Returns the value of the Input bean. If direction is
**         [input] then reads the input value of the pin and returns
**         it.
**     Parameters  : None
**     Returns     :
**         ---             - Input value
** ===================================================================
*/
#define RC1_GetVal() ((bool)(getRegBits(GPIO_B_RAWDATA,RC1_PIN_MASK)))

/* END RC1. */

#endif /* ifndef __RC1 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/

