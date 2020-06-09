/** ###################################################################
**     Filename  : Events.C
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
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "f8013.h"
#include "carsub.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

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
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
                         /* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)      */
void TI1_OnInterrupt(void)
{
	unsigned int nLeftMotorSpeed, nRightMotorSpeed;
  /* Write your code here ... */
  	//--------------------------------------------------------------------------
  	//----------------------------------------------------------------
  	// WAIT TIME 1ms Conter
  	//--------------------------------------------------------------------------
  	WAITTIME_INC;
  	
  	//----------------------------------------------------------------
  	g_nCarSpeedCount ++;
  	if(g_nCarSpeedCount >= CAR_SPEED_PERIOD) {   // Motor Speed Adjust
  		g_nCarSpeedCount = 0;
  		
	  	GetMotorSpeed(&nLeftMotorSpeed, &nRightMotorSpeed);
  		ClearMotorSpeed();
  		g_nLeftMotorSpeed = (int)nLeftMotorSpeed;
  		g_nRightMotorSpeed = (int)nRightMotorSpeed;
  		if(!MOTOR_LEFT_SPEED_POSITIVE) 	g_nLeftMotorSpeed = -g_nLeftMotorSpeed;
  		if(!MOTOR_RIGHT_SPEED_POSITIVE)	g_nRightMotorSpeed = -g_nRightMotorSpeed;
  		g_lnCarLeftPosition 	+= g_nLeftMotorSpeed;
  		g_lnCarRightPosition 	+= g_nRightMotorSpeed;
  		g_nLeftMotorSpeedCount += g_nLeftMotorSpeed;
  		g_nRightMotorSpeedCount += g_nRightMotorSpeed;
  		
  		
  	} else if(g_nCarSpeedCount == 1) {
  		ADC_Measure(0);
  	} else if(g_nCarSpeedCount == 2) {
  		CarVoltageGet();                        // Get all the car measure voltage 
  		CarAngleAdjust();
  	} else if(g_nCarSpeedCount == 3) {
  		g_nCarMotionCount ++;
  		if(g_nCarMotionCount >= CAR_MOTION_PERIOD) {
  			g_nCarMotionCount = 0;
  			MotorSpeedAdjust();
  			g_nLeftMotorSpeedCount = 0;
  			g_nRightMotorSpeedCount = 0;
  		} else if(g_nCarMotionCount == 1) {
			CarDirectionAdjust();
  		} else if(g_nCarMotionCount == 2) {
  		}
  		
  		CalculateMotorLeftRightDiff();              // --> Motor Left right Diff
  		CalculateMotorOutSpeed();               	// --> Left,Right Motor Out Speed
  	} else if(g_nCarSpeedCount == 4) {
  		g_lnCarMagneticLeftCount += CV_MAGNETLEFT_VAL;
  		g_lnCarMagneticRightCount += CV_MAGNETRIGHT_VAL;
  		if(g_lnCarMagneticLeftCount > CM_LEFT_OFFSET)
  			g_lnCarMagneticLeftCount -= CM_LEFT_OFFSET;
  		else g_lnCarMagneticLeftCount = 0;
  		if(g_lnCarMagneticRightCount > CM_RIGHT_OFFSET)
	  		g_lnCarMagneticRightCount -= CM_RIGHT_OFFSET;
	  	else g_lnCarMagneticRightCount = 0;
		
		g_nCarMagneticLeftAverage = (int)g_lnCarMagneticLeftCount;
  		g_nCarMagneticRightAverage = (int)g_lnCarMagneticRightCount;
  		g_lnCarMagneticLeftCount = 0;
  		g_lnCarMagneticRightCount = 0;
		CarMagneticAdjust();
  	}
  		
  	//----------------------------------------------------------------
}

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
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
                         /* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)      */
void ADC_OnEnd(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  I2C1_OnReceiveData (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of
**         the data successfully. This event is not available for
**         the SLAVE mode and if both RecvChar and RecvBlock are
**         disabled. This event is enabled only if interrupts/events
**         are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
/*
** ===================================================================
**     Event       :  I2C1_OnTransmitData (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission
**         of the data successfully. This event is not available for
**         the SLAVE mode and if both SendChar and SendBlock are
**         disabled. This event is enabled only if interrupts/events
**         are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
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
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
                         /* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)      */
void MOTOR2DIR_OnInterrupt(void)
{
  /* place your MOTOR2DIR interrupt procedure body here */
//  	D0_SetVal();
  	if(MOTOR2DIRVAL_GetVal()) g_nRightMotorDir = 1;
  	else g_nRightMotorDir = 0;
//  	D0_ClrVal();
}


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
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
                         /* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)      */
void MOTOR1DIR_OnInterrupt(void)
{
  /* place your MOTOR1DIR interrupt procedure body here */
//  	D0_SetVal();
  	if(MOTOR1DIRVAL_GetVal()) g_nLeftMotorDir = 1;
  	else g_nLeftMotorDir = 0;
//  	D0_ClrVal();
}


/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
