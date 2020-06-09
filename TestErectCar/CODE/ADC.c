/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : ADC.C
**     Project   : TestErectCar
**     Processor : 56F8013VFAE
**     Component : ADC
**     Version   : Component 01.580, Driver 02.05, CPU db: 2.87.097
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011-12-1, 16:09
**     Abstract  :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings  :
**         AD control register         : ADC_ADCR1   [61568]
**         AD control register         : ADC_ADCR2   [61569]
**         AD control register         : ADC_ADZCC   [61570]
**         AD control register         : ADC_ADLST1  [61571]
**         AD control register         : ADC_ADLST2  [61572]
**         AD control register         : ADC_ADSDIS  [61573]
**         AD control register         : ADC_ADSTAT  [61574]
**         AD control register         : ADC_ADLSTAT [61575]
**         AD control register         : ADC_ADZCSTAT [61576]
**         AD control register         : ADC_ADLLMT0 [61585]
**         AD control register         : ADC_ADLLMT1 [61586]
**         AD control register         : ADC_ADLLMT2 [61587]
**         AD control register         : ADC_ADLLMT3 [61588]
**         AD control register         : ADC_ADLLMT4 [61589]
**         AD control register         : ADC_ADLLMT5 [61590]
**         AD control register         : ADC_ADLLMT6 [61591]
**         AD control register         : ADC_ADLLMT7 [61592]
**         AD control register         : ADC_ADHLMT0 [61593]
**         AD control register         : ADC_ADHLMT1 [61594]
**         AD control register         : ADC_ADHLMT2 [61595]
**         AD control register         : ADC_ADHLMT3 [61596]
**         AD control register         : ADC_ADHLMT4 [61597]
**         AD control register         : ADC_ADHLMT5 [61598]
**         AD control register         : ADC_ADHLMT6 [61599]
**         AD control register         : ADC_ADHLMT7 [61600]
**         AD control register         : ADC_ADOFS0  [61601]
**         AD control register         : ADC_ADOFS1  [61602]
**         AD control register         : ADC_ADOFS2  [61603]
**         AD control register         : ADC_ADOFS3  [61604]
**         AD control register         : ADC_ADOFS4  [61605]
**         AD control register         : ADC_ADOFS5  [61606]
**         AD control register         : ADC_ADOFS6  [61607]
**         AD control register         : ADC_ADOFS7  [61608]
**         AD control register         : ADC_ADPOWER [61609]
**         AD control register         : ADC_CAL     [61610]
**         Interrupt name              : INT_ADCA_Complete
**         Priority                    : 1
**         User handling procedure     : ADC_OnEnd
**         Number of conversions       : 1
**         AD resolution               : 12-bit
**
**         Input pins
**
**              Port name              : GPIOC
**              Bit number (in port)   : 0
**              Bit mask of the port   : 1
**              Port data register     : GPIO_C_DR   [61729]
**              Port control register  : GPIO_C_DDR  [61730]
**              Port function register : GPIO_C_PER  [61731]
**
**              Port name              : GPIOC
**              Bit number (in port)   : 1
**              Bit mask of the port   : 2
**              Port data register     : GPIO_C_DR   [61729]
**              Port control register  : GPIO_C_DDR  [61730]
**              Port function register : GPIO_C_PER  [61731]
**
**              Port name              : GPIOC
**              Bit number (in port)   : 4
**              Bit mask of the port   : 16
**              Port data register     : GPIO_C_DR   [61729]
**              Port control register  : GPIO_C_DDR  [61730]
**              Port function register : GPIO_C_PER  [61731]
**
**              Port name              : GPIOC
**              Bit number (in port)   : 5
**              Bit mask of the port   : 32
**              Port data register     : GPIO_C_DR   [61729]
**              Port control register  : GPIO_C_DDR  [61730]
**              Port function register : GPIO_C_PER  [61731]
**
**         Initialization:
**              Conversion             : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-6
**     Contents  :
**         Measure    - byte ADC_Measure(bool WaitForResult);
**         GetValue16 - byte ADC_GetValue16(word *Values);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE ADC. */

#include "Events.h"
#include "ADC.h"

#define IDLE            0              /* IDLE state           */
#define MEASURE         1              /* MESURE state         */
#define CONTINUOUS      2              /* CONTINUOUS state     */
#define SINGLE          3              /* SINGLE state         */

static bool OutFlg;                    /* Measurement finish flag */
/*lint -esym(765,ADC_ModeFlg) Disable MISRA rule (23) checking for symbols (ADC_ModeFlg). */
volatile byte ADC_ModeFlg;             /* Current state of device */

/*
** ===================================================================
**     Method      :  ADC_InterruptCC (component ADC)
**
**     Description :
**         The method services the conversion complete interrupt of the 
**         selected peripheral(s) and eventually invokes the beans 
**         event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma interrupt alignsp saveall
void ADC_InterruptCC(void)
{
  setRegBits(ADC_ADSTAT,2048);         /* Clear EOSI flag */
  OutFlg = TRUE;                       /* Measured values are available */
  ADC_ModeFlg = IDLE;                  /* Set the bean to the idle mode */
  ADC_OnEnd();                         /* If yes then invoke user event */
}

/*
** ===================================================================
**     Method      :  HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the bean.
**         The method is called automatically as a part of Enable, 
**         Disable and Init methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  if (ADC_ModeFlg) {                   /* Launch measurement? */
    OutFlg = FALSE;                    /* Measured values are available */
    clrRegBit(ADC_ADCR1,STOP0);        /* Normal operation mode */
    setRegBit(ADC_ADCR1,START0);       /* Launching of conversion */
  }
  else {
    setRegBit(ADC_ADCR1,STOP0);        /* Stop command issued */
  }
}

/*
** ===================================================================
**     Method      :  ADC_Measure (component ADC)
**
**     Description :
**         This method performs one measurement on all channels that
**         are set in the bean inspector. (Note: If the <number of
**         conversions> is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     Parameters  :
**         NAME            - DESCRIPTION
**         WaitForResult   - Wait for a result of a
**                           conversion. If <interrupt service> is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the <number of channel> is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the <interrupt
**                           service> is disabled and a <number of
**                           conversions> is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
** ===================================================================
*/
byte ADC_Measure(bool WaitForResult)
{
  if (ADC_ModeFlg != IDLE) {           /* Is the device in running mode? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ADC_ModeFlg = MEASURE;               /* Set state of device to the measure mode */
  HWEnDi();                            /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ADC_ModeFlg == MEASURE) {}  /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ADC_GetValue16 (component ADC)
**
**     Description :
**         This method returns the last measured values of all channels
**         justified to the left. Compared with <GetValue> method this
**         method returns more accurate result if the <number of
**         conversions> is greater than 1 and <AD resolution> is less
**         than 16 bits. In addition, the user code dependency on <AD
**         resolution> is eliminated.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Values          - Pointer to the array that contains
**                           the measured data.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
** ===================================================================
*/
byte ADC_GetValue16(word *Values)
{
  if (!OutFlg) {                       /* Is measured value(s) available? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Values++ = (getReg(ADC_ADRSLT0) + 0) << 1; /* Store value from result register of device to user buffer */
  *Values++ = (getReg(ADC_ADRSLT1) + 0) << 1; /* Store value from result register of device to user buffer */
  *Values++ = (getReg(ADC_ADRSLT2) + 0) << 1; /* Store value from result register of device to user buffer */
  *Values = (getReg(ADC_ADRSLT3) + 0) << 1; /* Store value from result register of device to user buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ADC_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void ADC_Init(void)
{
  volatile word i;

  OutFlg = FALSE;                      /* No measured value */
  ADC_ModeFlg = IDLE;                  /* Device isn't running */
  /* ADC_ADCR1: ??=0,STOP0=1,START0=0,SYNC0=0,EOSIE0=1,ZCIE=0,LLMTIE=0,HLMTIE=0,CHNCFG=0,??=0,SMODE=0 */
  setReg(ADC_ADCR1,18432);             /* Set control register 1 */
  /* ADC_CAL: SEL_VREFH=0,SEL_VREFLO=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg(ADC_CAL,0);                   /* Set calibration register */
  /* ADC_ADPOWER: ASB=0,??=0,??=0,PSTS2=0,PSTS1=0,PSTS0=0,PUDELAY=13,APD=0,PD2=0,PD1=0,PD0=0 */
  setReg(ADC_ADPOWER,208);             /* Enable device */
  /* ADC_ADOFS0: ??=0,OFFSET=0,??=0,??=0,??=0 */
  setReg(ADC_ADOFS0,0);                /* Set offset reg. 0 */
  /* ADC_ADOFS1: ??=0,OFFSET=0,??=0,??=0,??=0 */
  setReg(ADC_ADOFS1,0);                /* Set offset reg. 1 */
  /* ADC_ADOFS2: ??=0,OFFSET=0,??=0,??=0,??=0 */
  setReg(ADC_ADOFS2,0);                /* Set offset reg. 2 */
  /* ADC_ADOFS3: ??=0,OFFSET=0,??=0,??=0,??=0 */
  setReg(ADC_ADOFS3,0);                /* Set offset reg. 3 */
  /* ADC_ADHLMT0: ??=0,HLMT=4095,??=0,??=0,??=0 */
  setReg(ADC_ADHLMT0,32760);           /* Set high limit reg. 0 */
  /* ADC_ADHLMT1: ??=0,HLMT=4095,??=0,??=0,??=0 */
  setReg(ADC_ADHLMT1,32760);           /* Set high limit reg. 1 */
  /* ADC_ADHLMT2: ??=0,HLMT=4095,??=0,??=0,??=0 */
  setReg(ADC_ADHLMT2,32760);           /* Set high limit reg. 2 */
  /* ADC_ADHLMT3: ??=0,HLMT=4095,??=0,??=0,??=0 */
  setReg(ADC_ADHLMT3,32760);           /* Set high limit reg. 3 */
  /* ADC_ADLLMT0: ??=0,LLMT=0,??=0,??=0,??=0 */
  setReg(ADC_ADLLMT0,0);               /* Set low limit reg. 0 */
  /* ADC_ADLLMT1: ??=0,LLMT=0,??=0,??=0,??=0 */
  setReg(ADC_ADLLMT1,0);               /* Set low limit reg. 1 */
  /* ADC_ADLLMT2: ??=0,LLMT=0,??=0,??=0,??=0 */
  setReg(ADC_ADLLMT2,0);               /* Set low limit reg. 2 */
  /* ADC_ADLLMT3: ??=0,LLMT=0,??=0,??=0,??=0 */
  setReg(ADC_ADLLMT3,0);               /* Set low limit reg. 3 */
  /* ADC_ADZCSTAT: ??=1,??=1,??=1,??=1,??=1,??=1,??=1,??=1,ZCS7=1,ZCS6=1,ZCS5=1,ZCS4=1,ZCS3=1,ZCS2=1,ZCS1=1,ZCS0=1 */
  setReg(ADC_ADZCSTAT,65535);          /* Clear zero crossing status flags */
  /* ADC_ADLSTAT: HLS7=1,HLS6=1,HLS5=1,HLS4=1,HLS3=1,HLS2=1,HLS1=1,HLS0=1,LLS7=1,LLS6=1,LLS5=1,LLS4=1,LLS3=1,LLS2=1,LLS1=1,LLS0=1 */
  setReg(ADC_ADLSTAT,65535);           /* Clear high and low limit status */
  /* ADC_ADSTAT: CIP0=0,CIP1=0,??=0,EOSI1=0,EOSI0=1,ZCI=0,LLMTI=0,HLMTI=0,RDY7=0,RDY6=0,RDY5=0,RDY4=0,RDY3=0,RDY2=0,RDY1=0,RDY0=0 */
  setReg(ADC_ADSTAT,2048);             /* Clear EOSI flag */
  /* ADC_ADSDIS: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,DS7=1,DS6=1,DS5=1,DS4=1,DS3=0,DS2=0,DS1=0,DS0=0 */
  setReg(ADC_ADSDIS,240);              /* Enable/disable of samples */
  /* ADC_ADLST1: ??=0,SAMPLE3=5,??=0,SAMPLE2=4,??=0,SAMPLE1=1,??=0,SAMPLE0=0 */
  setReg(ADC_ADLST1,21520);            /* Set ADC channel list reg. */
  /* ADC_ADZCC: ZCE7=0,ZCE6=0,ZCE5=0,ZCE4=0,ZCE3=0,ZCE2=0,ZCE1=0,ZCE0=0 */
  setReg(ADC_ADZCC,0);                 /* Set zero crossing control reg. */
  /* ADC_ADCR2: ??=0,STOP1=0,START1=0,SYNC1=0,EOSIE1=0,??=0,??=0,??=0,??=0,??=0,SIMULT=1,DIV=2 */
  setReg(ADC_ADCR2,34);                /* Set prescaler */
  for (i=0;i<100;i++) {}               /* Recovery time of voltage reference */
  while (ADC_ADPOWER&(ADC_ADPOWER_PSTS_MASK)) {} /* Wait for device powered up */
}

/* END ADC. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
