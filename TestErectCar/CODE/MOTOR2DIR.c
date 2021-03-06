/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : MOTOR2DIR.C
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : ExtInt
**     Version   : Component 02.094, Driver 02.04, CPU db: 2.87.097
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-4, 23:11
**     Abstract  :
**         This bean "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The bean uses one pin which generates interrupt on 
**         selected edge.
**     Settings  :
**         Interrupt name              : INT_GPIO_A
**         User handling procedure     : MOTOR2DIR_OnInterrupt
**
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       20            |  GPIOA5_PWM5_FAULT2_T3
**             ----------------------------------------------------
**
**         Port name                   : GPIOA
**
**         Bit number (in port)        : 5
**         Bit mask of the port        : 32
**
**         Signal edge/level           : rising
**         Priority                    : 1
**         Pull option                 : off
**         Initial state               : Enabled
**
**         Edge register               : GPIO_A_IPOLR [61702]
**         Priority register           : INTC_IPR2 [61538]
**         Enable register             : GPIO_A_IENR [61701]
**         Request register            : GPIO_A_IESR [61704]
**
**         Port data register          : GPIO_A_DR [61697]
**         Port control register       : GPIO_A_DDR [61698]
**         Port function register      : GPIO_A_PER [61699]
**     Contents  :
**         GetVal - bool MOTOR2DIR_GetVal(void);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE MOTOR2DIR. */

#include "MOTOR2DIR.h"
/*Including shared modules, which are used for all project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  MOTOR2DIR_Interrupt (component ExtInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and invokes the beans event.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma interrupt alignsp saveall
void MOTOR2DIR_Interrupt(void)
{
  setReg(GPIO_A_IESR,MOTOR2DIR_PIN_MASK); /* Clear flag */
  MOTOR2DIR_OnInterrupt();
}

/*
** ===================================================================
**     Method      :  MOTOR2DIR_GetVal (component ExtInt)
**
**     Description :
**         Returns the actual value of the input pin of the bean.
**     Parameters  : None
**     Returns     :
**         ---             - Returned input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
** ===================================================================
*/
/*
bool MOTOR2DIR_GetVal(void)

**      This method is implemented as macro      **
*/

/* END MOTOR2DIR. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
