/** ###################################################################
**     Filename  : TestErectCar.C
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Version   : Driver 01.14
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-1, 8:34
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE TestErectCar */

//==============================================================================
//
// Test Erect Intelligent Car for Running.			-- by Dr. ZhuoQing 2011,12,01
//
//------------------------------------------------------------------------------
// The port configration is :
// 1. AIN0:    MMA7260:YOUT
// 2. AIN1:    MMA7660:ZOUT
// 3. BIN0:    GYROSCROP
// 4. BIN1:    Vref. Come from a POT voltage(0~3.3v)
//
//------------------------------------------------------------------------------


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
//------------------------------------------------------------------------------
#include "f8013.h"
#include "MMA7660.h"
#include "carsub.h"

//------------------------------------------------------------------------------
void main(void)
{
	unsigned int nADCValue[6];
	unsigned int i;
//	unsigned char ucXYZ[3];
//	long lnValue;
//	int nLeft, nRight, nSetSpeed;
	
	//--------------------------------------------------------------------------
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  	//--------------------------------------------------------------------------
  	for(i = 0; i < 6; i ++) nADCValue[i] = 0;
  	MMA7660Init();
  	CarInit();
  	//--------------------------------------------------------------------------
	  	
  	SetMotorSpeed(0, 0);
  	//--------------------------------------------------------------------------
  	TestCarErect();
  	CAR_START;
  	
  	//--------------------------------------------------------------------------
#define TIME_DELAY	 10
	g_nLeftSpeedSet = 0;
	g_nRightSpeedSet = 0;
	g_nMotorSpeedSet = 0;
  	for(;;) {
  		if(CAR_STOP_FLAG)
  			TestCarErect();
  			
  		WaitTime(TIME_DELAY);
  		LED_ON;
  		WaitTime(TIME_DELAY);
  		LED_OFF;
  		//----------------------------------------------------------------------
//  		if(ADC_Measure(1) != ERR_OK) continue;
//  		if(ADC_GetValue16(nADCValue) != ERR_OK) continue;
//  		if(MMA7660GetXYZ(ucXYZ) != 0) continue;
/*		ucXYZ[0] = 0;
		ucXYZ[1] = 0;
		ucXYZ[2] = 0;
		nADCValue[0] = g_nCarVoltage[0];
		nADCValue[1] = g_nCarVoltage[1];
		nADCValue[2] = g_nCarVoltage[2];
		nADCValue[3] = g_nCarVoltage[3];
*/
		nADCValue[0] = (unsigned int)(g_nCarAcceVal + 20000);
		nADCValue[1] = (unsigned int)(g_nCarGyroVal + 30000);
		nADCValue[2] = (unsigned int)(g_nCarMagneticLeftAverage);
		nADCValue[3] = (unsigned int)(g_nCarMagneticRightAverage);
		nADCValue[4] = (unsigned int)(g_nCarMagneticLeftAverage - g_nCarMagneticRightAverage + 10000);
		nADCValue[5] = (unsigned int)(g_nMotorLeftRightDiff + 10000);
//		nADCValue[4] = g_nLeftMotorDir;
//		nADCValue[5] = g_nRightMotorDir;
		//----------------------------------------------------------------------  		

  		//----------------------------------------------------------------------
		
  		//----------------------------------------------------------------------
  		SendChar(0x55);
  		SendWord(nADCValue[0]);
  		SendWord(nADCValue[1]);
  		SendWord(nADCValue[2]);
  		SendWord(nADCValue[3]);
  		SendWord(nADCValue[4]);
  		SendWord(nADCValue[5]);
  		
/*  		PWMC1_SetRatio16(1, 0);
  		PWMC1_SetRatio16(0, nADCValue[1]);
  		PWMC1_SetRatio16(3, 0);
  		PWMC1_SetRatio16(2, 0);//nADCValue[1]);
  		PWMC1_Load();
*/
		//----------------------------------------------------------------------
#define MOTOR_SET_SPEED_MAX			500
#define MOTOR_SET_INC				1
#define MOTOR_SET_DEC				2
		//----------------------------------------------------------------------
		if(RC2_GetVal()) {                      // 'A'
			if(g_nMotorSpeedSet < MOTOR_SET_SPEED_MAX - MOTOR_SET_INC)
				g_nMotorSpeedSet += MOTOR_SET_INC;
		}
		//----------------------------------------------------------------------
		if(RC1_GetVal()) {                      // 'C'
			if(g_nMotorSpeedSet > - MOTOR_SET_SPEED_MAX + MOTOR_SET_INC)
				g_nMotorSpeedSet -= MOTOR_SET_INC;
		}
		//----------------------------------------------------------------------
		if(RC3_GetVal()) {                      // 'B'
			if(g_nMotorSpeedSet > MOTOR_SET_DEC) g_nMotorSpeedSet -= MOTOR_SET_DEC;
			else if(g_nMotorSpeedSet < -MOTOR_SET_DEC) g_nMotorSpeedSet += MOTOR_SET_DEC;
			else g_nMotorSpeedSet = 0;
			
			if(g_nMotorSpeedSet == 0) {
//				g_nMotorLeftRightDiff = 10;
				g_lnLeftRightPositionDiff = 1000;
			}
			
			CMA_STOP;
		} else {
			if(IF_CMA_STOP) {
				g_lnLeftRightPositionDiff = 0;
			}
		}
		//----------------------------------------------------------------------
		if(RC4_GetVal()) {                      // 'D'
												// Stop Car Magnetic Adjust
			CMA_START;
		}
/*		if(RC1_GetVal()) {
			if(g_nMotorSpeedSet < MOTOR_SET_SPEED_MAX - MOTOR_SET_INC)
				g_nMotorSpeedSet += MOTOR_SET_INC;
		} else if(RC2_GetVal()) {
			if(g_nMotorSpeedSet > - MOTOR_SET_SPEED_MAX + MOTOR_SET_INC)
				g_nMotorSpeedSet -= MOTOR_SET_INC;
		} else {
			if(g_nMotorSpeedSet > MOTOR_SET_DEC) g_nMotorSpeedSet -= MOTOR_SET_DEC;
			else if(g_nMotorSpeedSet < -MOTOR_SET_DEC) g_nMotorSpeedSet += MOTOR_SET_DEC;
			else g_nMotorSpeedSet = 0;
		}
*/
		//----------------------------------------------------------------------
/*#define MOTOR_DIFF			50
		if(RC3_GetVal()) {
			g_lnLeftRightPositionDiff += MOTOR_DIFF;
		} else if(RC4_GetVal()) {
			g_lnLeftRightPositionDiff -= MOTOR_DIFF;
		}
*/
  	}
 
  	//--------------------------------------------------------------------------

  for(;;) {}
}

/* END TestErectCar */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
