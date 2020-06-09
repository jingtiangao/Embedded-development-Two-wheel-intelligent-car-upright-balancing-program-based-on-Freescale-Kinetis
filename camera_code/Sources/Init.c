#include <hidef.h>      /* common defines and macros */
#include "derivative.h" 
#include"Init.h"
#include"define.h"

/************************************************************************************
Func Name:    PLL_Init                                
FuncDescrip:  设置总线速度 80M
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
FuncDescrip:  计数器中断初始化 通道 0 初始化,20ms定时中断设置  
              time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS.
              For example, for a 40 MHz bus clock, the maximum time-out period equals:
              256 * 65536 * 25 ns = 419.43 ms.
************************************************************************************/
 void PIT_Init(void) 
  {   
      PITCFLMT_PITE=0;        //定时中断通道关,4 个通道均关闭 
      PITCE_PCE0=1;           //定时器通道 0 使能 
      PITMUX_PMUX0=0;         //0:8位定时器 0 与16位定时器共同作用，需设定 PITMTLD0 初值 
                              //1:8位定时器1 与16 位定时器共同作用，需设定 PITMTLD1 初值 
      PITMTLD0=46-1;          //8 位定时器0 初值设定，88MHzBusClock下为 1us，设定一次即可 
      PITLD0=PITTIME0-1;      //16位定时器初值设定。time=PITTIME*0.001MS  
      PITINTE_PINTE0=1;       //1定时器中断通道 0中断使能 
      PITCFLMT_PITE=1;        //定时器使能,初始化一次即可
  } 


/************************************************************************************
Func Name:    ECT0_init（）                                
FuncDescrip:  摄像头中断初始化
**************************************************/
 void ECT0_init()  //摄像头的行（3）场(1)中断
  { 
      IRQCR_IRQEN=0; 
      TSCR1=0X80;          //允许输入捕捉，快速请标志位
      TSCR2=0X01;
      TIE_C0I=0;           //场中断
      TIE_C1I=0;           //奇偶场中断
      TIE_C2I=0;           //行中断
      TIE_C3I=0;           //上升沿中断
      TIE_C4I=0;           //下降沿中断
      TIOS=0X00;
      TCTL4_EDG0A=1;       //上升沿沿捕捉  场中断
      TCTL4_EDG0B=0; 
      TCTL4_EDG2A=1;       //上升沿沿触发  行中断
      TCTL4_EDG2B=0;
      TCTL4_EDG3A=1;       //上升沿沿触发  检测上升沿中断 
      TCTL4_EDG3B=0;
      TCTL3_EDG4A=0;       //下降沿触发  检测上升沿中断  
      TCTL3_EDG4B=1;       
      TFLG1=0XFF;          //清标志位 */
  }
/************************************************************************************
Func Name:    ECT_Init                                
FuncDescrip:  ECT外部脉冲中断初始化
**************************************************/
void ECT_Init(void) 
  {
      PACTL=0x40;              //使能脉冲累加器，计数模式，下降沿计数，时钟TCNT的时钟
      PACNT=0x00;              //脉冲累加器使用的是PT7口
                               //要注意的是这里的paen要置1，同时脉冲输入口，要接入到ioc7也就是说pt7接口
                               //只要不使用PT7口作为PWM7 同时使能了TCNT或者PACNT 就可以使PT7接入到IOC7上
  }

/************************************************************************************
Func Name:    PWM_Init                                
FuncDescrip:  PWM初始化                            
************************************************************************************/
void PWM_Init()
 {
      PWME    =0x00;	     //禁止PWM 因为PT7要做脉冲累加器 不能再做PWM口 所以不能再使用PWME = 0xff
      //DDRM=0XFF;
      PWMPRCLK=0x33;         //SA SB 时钟 为总线时钟的4分频  80M/8 = 10M              
      PWMSCLA =0x02;         //时钟SA为2MHz   Clock SA = Clock A / (2 * PWMSCLA)
      PWMSCLB =0x02;         //时钟SB为2MHz   Clock SB = Clock B / (2 * PWMSCLB)
      PWMCAE  =0x00;         //左对齐输出模式           
      PWMPOL  =0xff;	       //通道输出波形开始极性为1   
      PWMCLK  =0xff;         //PWM时钟源                  SA:01  SB:23 SA:45
      PWMCTL  =0xB0;         //67 01 23级联
      PWMPER01=500;          //  2.5M/500=5khz
      PWMPER23=500;          //  2.5M/500=5khz

      //舵机控制
      PWMPER67=50000;        //  100hz
         	
      PWME_PWME0=1;          //电机使能
      PWME_PWME1=1;          //舵机使能
      PWME_PWME2=1;          //舵机使能
      PWME_PWME3=1;          //舵机使能
      PWME_PWME7=1;          //舵机使能 
      PWMDTY01=0;            //right占空比初始化               Duty is 50%,and PWM waveform's frequent is 16                     //占空比初始化               Duty is 50%,and PWM waveform's frequent is 16  
      PWMDTY23=0;            //left占空比初始化                Duty is 50%,and PWM waveform's frequent is 16  
   
      PWMDTY67=Steer_Mid;
      
      
      DDRP_DDRP5=1;          //硬件使能 
      PTP_PTP5=1; 
 }
/***********************************************************************************************
Func Name:       PIT1_init
FuncDescrip:     周期性定时器初始化
************************************************************************************************/
void PIT1_init() 
{
      PITCFLMT_PITE=0;                  //初始化之前先关定时器
      PITMUX_PMUX1=1;                   //通道一连接时基1，通道0连接时基0，通道0用于测速，通道1用于计时
      PITMTLD1=PITMLD1temp;             //设定分频数，具体的分频数由行中断的时间及总线频率确定
      PITLD1=PITLD1temp;                //设定计时器的初始值
      PITCE_PCE1=1;                     //使能通道一
      PITINTE_PINTE1=0;                 //禁止通道一申请中断，只利用通道一的时间所存功能
      PITCFLMT_PITE=1;                  //初始化结束后使能PIT中断
}
/***************************************************************************************************
Func Name       :Dial_Switch_init
FuncDescrip     :对拨码开关连接的PORTAD口进行初始化
拨码开关的引脚接在单片机的AD口上，古必须先对单片机的AD口引脚进行初始化，使其作为输入口
***************************************************************************************************/
void  Dial_Switch_init() 
{
      ATD0DIEN=0xffff;                  //ATD0DIEN(ATD0 digital input enable),把AD口作为普通输入口用时必须使能该寄存器
      DDR0AD0=0x00;                     //1----output        0--input
      DDR1AD0=0x00;                     //1----output        0--input
}