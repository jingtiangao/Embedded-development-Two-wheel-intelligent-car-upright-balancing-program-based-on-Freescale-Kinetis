#include <hidef.h>      /* common defines and macros */
#include "derivative.h" 
#include"Init.h"
#include"define.h"

/************************************************************************************
Func Name:    PLL_Init                                
FuncDescrip:  ���������ٶ� 80M
************************************************************************************/
void PLL_Init(void)
  {                            
      CLKSEL=0X00;                  //disengage PLL to system 
      PLLCTL_PLLON=1;               //turn on PLL 
      SYNR =0xc0 | 0x13;                         
      REFDV=0x80 | 0x02;  
      POSTDIV=0x00;                 //pllclock=2*osc*(1+SYNR)/(1+REFDV)=1
      _asm(nop);                    //BUS CLOCK=80M 
      _asm(nop); 
      while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;
      CLKSEL_PLLSEL =1;             //engage PLL to system;
   }


/************************************************************************************
Func Name:    PIT_Init                                
FuncDescrip:  �������жϳ�ʼ�� ͨ�� 0 ��ʼ��,20ms��ʱ�ж�����  
              time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS.
              For example, for a 40 MHz bus clock, the maximum time-out period equals:
              256 * 65536 * 25 ns = 419.43 ms.
************************************************************************************/
 void PIT_Init(void) 
  {   
      PITCFLMT_PITE=0;        //��ʱ�ж�ͨ����,4 ��ͨ�����ر� 
      PITCE_PCE0=1;           //��ʱ��ͨ�� 0 ʹ�� 
      PITMUX_PMUX0=0;         //0:8λ��ʱ�� 0 ��16λ��ʱ����ͬ���ã����趨 PITMTLD0 ��ֵ 
                              //1:8λ��ʱ��1 ��16 λ��ʱ����ͬ���ã����趨 PITMTLD1 ��ֵ 
      PITMTLD0=46-1;          //8 λ��ʱ��0 ��ֵ�趨��88MHzBusClock��Ϊ 1us���趨һ�μ��� 
      PITLD0=PITTIME0-1;      //16λ��ʱ����ֵ�趨��time=PITTIME*0.001MS  
      PITINTE_PINTE0=1;       //1��ʱ���ж�ͨ�� 0�ж�ʹ�� 
      PITCFLMT_PITE=1;        //��ʱ��ʹ��,��ʼ��һ�μ���
  } 


/************************************************************************************
Func Name:    ECT0_init����                                
FuncDescrip:  ����ͷ�жϳ�ʼ��
**************************************************/
 void ECT0_init()  //����ͷ���У�3����(1)�ж�
  { 
      IRQCR_IRQEN=0; 
      TSCR1=0X80;          //�������벶׽���������־λ
      TSCR2=0X01;
      TIE_C0I=0;           //���ж�
      TIE_C1I=0;           //��ż���ж�
      TIE_C2I=0;           //���ж�
      TIE_C3I=0;           //�������ж�
      TIE_C4I=0;           //�½����ж�
      TIOS=0X00;
      TCTL4_EDG0A=1;       //�������ز�׽  ���ж�
      TCTL4_EDG0B=0; 
      TCTL4_EDG2A=1;       //�������ش���  ���ж�
      TCTL4_EDG2B=0;
      TCTL4_EDG3A=1;       //�������ش���  ����������ж� 
      TCTL4_EDG3B=0;
      TCTL3_EDG4A=0;       //�½��ش���  ����������ж�  
      TCTL3_EDG4B=1;       
      TFLG1=0XFF;          //���־λ */
  }
/************************************************************************************
Func Name:    ECT_Init                                
FuncDescrip:  ECT�ⲿ�����жϳ�ʼ��
**************************************************/
void ECT_Init(void) 
  {
      PACTL=0x40;              //ʹ�������ۼ���������ģʽ���½��ؼ�����ʱ��TCNT��ʱ��
      PACNT=0x00;              //�����ۼ���ʹ�õ���PT7��
                               //Ҫע����������paenҪ��1��ͬʱ��������ڣ�Ҫ���뵽ioc7Ҳ����˵pt7�ӿ�
                               //ֻҪ��ʹ��PT7����ΪPWM7 ͬʱʹ����TCNT����PACNT �Ϳ���ʹPT7���뵽IOC7��
  }

/************************************************************************************
Func Name:    PWM_Init                                
FuncDescrip:  PWM��ʼ��                            
************************************************************************************/
void PWM_Init()
 {
      PWME    =0x00;	     //��ֹPWM ��ΪPT7Ҫ�������ۼ��� ��������PWM�� ���Բ�����ʹ��PWME = 0xff
      //DDRM=0XFF;
      PWMPRCLK=0x33;         //SA SB ʱ�� Ϊ����ʱ�ӵ�4��Ƶ  80M/8 = 10M              
      PWMSCLA =0x02;         //ʱ��SAΪ2MHz   Clock SA = Clock A / (2 * PWMSCLA)
      PWMSCLB =0x02;         //ʱ��SBΪ2MHz   Clock SB = Clock B / (2 * PWMSCLB)
      PWMCAE  =0x00;         //��������ģʽ           
      PWMPOL  =0xff;	       //ͨ��������ο�ʼ����Ϊ1   
      PWMCLK  =0xff;         //PWMʱ��Դ                  SA:01  SB:23 SA:45
      PWMCTL  =0xB0;         //67 01 23����
      PWMPER01=500;          //  2.5M/500=5khz
      PWMPER23=500;          //  2.5M/500=5khz

      //�������
      PWMPER67=50000;        //  100hz
         	
      PWME_PWME0=1;          //���ʹ��
      PWME_PWME1=1;          //���ʹ��
      PWME_PWME2=1;          //���ʹ��
      PWME_PWME3=1;          //���ʹ��
      PWME_PWME7=1;          //���ʹ�� 
      PWMDTY01=0;            //rightռ�ձȳ�ʼ��               Duty is 50%,and PWM waveform's frequent is 16                     //ռ�ձȳ�ʼ��               Duty is 50%,and PWM waveform's frequent is 16  
      PWMDTY23=0;            //leftռ�ձȳ�ʼ��                Duty is 50%,and PWM waveform's frequent is 16  
   
      PWMDTY67=Steer_Mid;
      
      
      DDRP_DDRP5=1;          //Ӳ��ʹ�� 
      PTP_PTP5=1; 
 }
/***********************************************************************************************
Func Name:       PIT1_init
FuncDescrip:     �����Զ�ʱ����ʼ��
************************************************************************************************/
void PIT1_init() 
{
      PITCFLMT_PITE=0;                  //��ʼ��֮ǰ�ȹض�ʱ��
      PITMUX_PMUX1=1;                   //ͨ��һ����ʱ��1��ͨ��0����ʱ��0��ͨ��0���ڲ��٣�ͨ��1���ڼ�ʱ
      PITMTLD1=PITMLD1temp;             //�趨��Ƶ��������ķ�Ƶ�������жϵ�ʱ�估����Ƶ��ȷ��
      PITLD1=PITLD1temp;                //�趨��ʱ���ĳ�ʼֵ
      PITCE_PCE1=1;                     //ʹ��ͨ��һ
      PITINTE_PINTE1=0;                 //��ֹͨ��һ�����жϣ�ֻ����ͨ��һ��ʱ�����湦��
      PITCFLMT_PITE=1;                  //��ʼ��������ʹ��PIT�ж�
}
/***************************************************************************************************
Func Name       :Dial_Switch_init
FuncDescrip     :�Բ��뿪�����ӵ�PORTAD�ڽ��г�ʼ��
���뿪�ص����Ž��ڵ�Ƭ����AD���ϣ��ű����ȶԵ�Ƭ����AD�����Ž��г�ʼ����ʹ����Ϊ�����
***************************************************************************************************/
void  Dial_Switch_init() 
{
      ATD0DIEN=0xffff;                  //ATD0DIEN(ATD0 digital input enable),��AD����Ϊ��ͨ�������ʱ����ʹ�ܸüĴ���
      DDR0AD0=0x00;                     //1----output        0--input
      DDR1AD0=0x00;                     //1----output        0--input
}