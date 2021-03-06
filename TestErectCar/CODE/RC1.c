/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : RC1.C
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

/* MODULE RC1. */

#include "RC1.h"

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
/*
bool RC1_GetVal(void)

**  This method is implemented as a macro. See RC1.h file.  **

*/

/* END RC1. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
