#include <hidef.h>     
#include <MC9S12XS128.h>
#include "derivative.h" 									    
#include<stdlib.h>
#include"MySD.h"
#include"Define.h"
#include"nRF24L01.h"															    
int     quan[ROW_MAX]=
{
	14,14,14,14,14,
	15,15,15,15,15,
	20,20,20,20,20,
	15,15,14,14,14,
	11,11,11,11,11,
	10,10,10,10,10,
	10,10,10,10,10,
	10,10,10,10,10,
	5,5,5,5,5,
	5,5,5,5,5
}; 
unsigned char cmos[ROW_MAX]={
	24,33,42,51,60,
	69,80,90,100,107,
	111,115,119,123,127,
	131,133,135,137,140,
	142,145,151,153,155,
	157,160,163,166,170,
	172,174,176,177,179,
	181,183,185,187,189,
	191,192,193,194,195,
	196,197,198,199,200
};
uchar     numb_cmos;            //采集的行数
uchar     v_point_up[60][5];
uchar     v_point_down[60][5];
uchar     en_speed;
uchar     use_ad_con;
uchar     now_e0,pre_e0;;
uchar     Flag_Up,Flag_Down;
uint      Line_CCD;
uchar     Line_Save;
uint      PITCNT1temp;
uchar     Pre_Line_Save;
int       pt_left,pt_right;
int       pangle_left,pangle_right;
uchar     pt_left_line,pt_right_line;
uchar     will_be_out,will_be_out_left,will_be_out_right;
int       sum_Block;
uchar     point_up[60];
uchar     point_down[60];
int       p_point_up[60],p_point_down[60];
int       test_numbL;
uchar     stop_find_right,stop_find_left;
uchar     Flag_Lose_Line;
uchar     is_find_a_real_start;
uchar     not_find_a_real_start;
uchar     num_wrong_left,num_wrong_right;
int       End_Line;
uchar     num_find;
uchar     num_shizi;
uchar     flag_shizi;
uchar     flag_not_shizi;
uchar     stright_flag;
int       stright;
uchar     flag_find_aftershizi;
uchar     is_have_a_lstl,is_have_a_rstl;
uchar     find_left_startline,find_right_startline;
uchar     pfind_left_startline,pfind_right_startline;
uchar     left_stratline,right_startline;
uchar     pleft_endline,pright_endline;
uchar     find_last_lstl,find_last_rstl;
uchar     pre_left_stl,pre_right_stl;
uchar     pre_left_stl_error,pre_right_stl_error;
uchar     last_stl;
long int       error,SteerAngle0,SteerAngle1,SteerAngle;
long int       p_SteerAngle0;
long int       pre_SteerAngle;
int       now_pwmdty67;
int       xiuzheng_error;
int       mid_error;
int       start_error;
uchar     x;
uchar     is_odd;
int       x_error;
//int       test_x_error[600]; 
uint 	  test_x_error_num;
int       pre_x_error;
int       ee_x_error;
uchar     dian_sha;
uint      left_block0,right_block0;
uchar     find_left,find_right;
uchar     find_last_left,find_last_right;
uchar     left_endline,right_endline;
uchar     num_onlyfind_one_startline;
uchar     test_error;
uchar     valium,mid_valium;
uchar     turn_left,turn_right;
uchar     condition_1,condition_2,condition_3,condition_4,condition_5,condition_6;
uchar     condition_7,condition_8,condition_9,condition_10,condition_11,condition_12;
uchar     condition_13,condition_14,condition_15,condition_16,condition_17,condition_18;
uchar     condition_19,condition_20;
int       x1,x2,x3,y1,y2,y3;
int   	  s_abc;
uint      ab,bc,ac;
uint      temp_aa,temp_bb,temp_cc;
int       radius;
int       temp_test;
uchar     test_left_stl,test_right_stl;
uchar     test_left,test_right,test_a,test_b;
uchar     test_stl_error,test_str_error;
int       test_abs;
uchar     test_c,test_d;
uchar     test_flag_shizi;
int       test_char;
float     test_sum;   
uchar 	  flag_ed,flag_q;
uchar     test_aa_lstl,test_aa_rstl;
uchar     test_bbhh;
uchar     is_startline1,is_startline2,is_startline3,is_startline4,is_startline;
uint      num_chang;
uint      num_shizi_chang;
uchar     shorter_line;
uint      sum_zhidao;
uchar      num_zhidao;
uchar      need_cut_speed;
int        ped_tspeed;
int        CurrentSpeed;
int        OutputSpeed=0;
uint       CSpeedArraySum=0;
uint       CSpeedArraySum1=0;
uint       CSpeedArray[5];
int        PWM;
int StraightSpeed;
int CurveSpeed;
int angle;
uchar send;
uint test1,test;
int     zhidao_speed;
int     shizi_speed;
int     refer_speed;
int     limit_speed;
uchar   is_use_start;
uchar   big_left,little_right;
uchar   sum_little_angle,sum_cut;
unsigned long int       aver_pwm,num_pwm;
uchar   flag_use_special_cutspeed;
uchar   sum_wandao;
//**********无线发送************//
void SCI0Tx255(int ch)
{
    uchar a,b,c,d,e;
    if(ch>=0)
    {
 	    a=ch/10000;
	    b=ch/1000%10;
	    c=ch/100%10;
	    d=ch/10%10;
	    e=ch%10;
	    a+='0';
	    b+='0';
	    c+='0';
	    d+='0';
	    e+='0';
	    Wireless_Send_Byte(a);
	    Wireless_Send_Byte(b);
	    Wireless_Send_Byte(c);
	    Wireless_Send_Byte(d );
	    Wireless_Send_Byte(e);
	    Wireless_Send_Byte(' ');
    }
    else
    {
        ch=-ch;
        a=ch/10000;
	    b=ch/1000%10;
	    c=ch/100%10;
	    d=ch/10%10;
	    e=ch%10;
	    a+='0';
	    b+='0';
	    c+='0';
	    d+='0';
	    e+='0';
	    Wireless_Send_Byte('-');
	    Wireless_Send_Byte(a);
	    Wireless_Send_Byte(b);
	    Wireless_Send_Byte(c);
	    Wireless_Send_Byte(d );
	    Wireless_Send_Byte(e);
	    Wireless_Send_Byte(' ');
    }
}

void SCI0Tx1Fld1(void) 
{
  uint i;
  for(i=0;i<600;i++) 
  {
   //   SCI0Tx255(test_x_error[i]);
      Wireless_Send_Byte(',');
  }
}
typedef struct MyPid
{                      
  int PID_P;
  int PID_I;
  int PID_D;
  int Error;
  int PreError;
  int PrePreError;
  int ErrorSum;
  int CSpeed;    //当前速度
  int TSpeed;    //目标速度
  int OutPut;    //PID输出量
}MYPID;
MYPID MyPID;
/*************************************************/

/***************初始化PID参数**************************/
void PIDInit() 
{
  MyPID.PID_P=KP;
  MyPID.PID_I=KI;
  MyPID.PID_D=KD; 
  MyPID.PreError=0;
  MyPID.PrePreError=0;
  MyPID.ErrorSum=0;  
}

int PIDCalculate() 
{
  int DeltValue,CurrentError;
  CurrentError=MyPID.TSpeed-CurrentSpeed; //获取目标速度和当前速度差
  if (CurrentError>100) { return  500;}
  if(CurrentError<-100) { return -500;}
  if(CurrentError<MyPID.TSpeed/20&& CurrentError>(MyPID.TSpeed/20)*(-1))
      CurrentError=0;
  DeltValue=CurrentError*MyPID.PID_P-MyPID.PreError*MyPID.PID_I+MyPID.PrePreError*MyPID.PID_D;     //求得速度增量
  MyPID.PrePreError=MyPID.PreError;
  MyPID.PreError=CurrentError;
  if(DeltValue>400)
    DeltValue=400;
  return  DeltValue;
}

uint get_current_speed(void) 
{
    uchar i;
    CSpeedArraySum=0;    
    for(i=1;i<5;i++) 
    {
      CSpeedArray[i]=CSpeedArray[i-1];
    } 
    CSpeedArray[0] = PACNT;
    PACNT=0; 
    CSpeedArraySum=(CSpeedArray[0]*7+CSpeedArray[1]*3)/10;
    CSpeedArraySum1 = CSpeedArraySum*5+18;
    if(CSpeedArraySum1>400)
       CSpeedArraySum1 = 400;          
    return CSpeedArraySum1;
}

void speed_ctl(void)
{                   
  CurrentSpeed=get_current_speed();
  OutputSpeed+=PIDCalculate();
  if(OutputSpeed>=SPEED_MAX) 
   OutputSpeed=SPEED_MAX;
  else if(OutputSpeed<=SPEED_MIN) 
   OutputSpeed=SPEED_MIN;
  num_chang++;
  if(num_chang>10000)
  {
  	    num_chang=10000;
  }
  if(num_chang==400)// 这里应该是起跑线等待，如果一场25ms，400 长10s吗？23 级联控制电机
  {
     PWME_PWME3=1;
     sum_zhidao=0;   // 开始直道计数	 
  }
  num_shizi_chang++;// 十字场叠加
  if(num_shizi_chang>10000)
  {
  	  num_shizi_chang=10000;	
  }
  if(test_a>40&&test_b>40)
  {
  	num_shizi_chang+=200;	
  }
  if(flag_shizi)// 遇到十字就把十字场清0
  {
  	  num_shizi_chang=0;	
  }
  if(num_chang>10000)
  {
  	    num_chang=10000;
  }
  if(PT1AD0_PT1AD03==1)  // 这是干嘛？
  {
     PWMDTY23=OutputSpeed;
     if(!is_startline)
     {
        PWME_PWME1=1;
     }
    // PWMDTY23=0;
  }
  else
  {
     PWMDTY23=0;
     PWME_PWME1=0;	
  } 
}
void      pto_get_sumblock()
{
		 int  i;
		 x=0;
		 if(find_left_startline&&find_right_startline)
		 {
		       mid_error=((p_point_up[left_endline-(left_endline-left_stratline)/3*2]+p_point_down[right_endline-(right_endline+right_startline)/3*2]))>>1;
		       error=(p_point_up[left_endline]+p_point_down[right_endline])>>1;
		       start_error=(p_point_up[left_stratline]+p_point_down[right_startline])>>1;
			   if(left_stratline>right_startline)   { last_stl=left_stratline; } 
			   else									{ last_stl=right_startline;}
			   if(pleft_endline>pright_endline)
			   {
			   		for(i=pleft_endline;i>pright_endline;i--)
			   		{
			   			  is_odd=(is_odd+1)%2;
			   		      if(is_odd)
			   		      {
			   		      	 sum_Block+=(int)((((p_point_up[i]+p_point_down[right_endline])/2)-Line_Center+23)*quan[x])/10;
			   		      	 x++;
			   		      }
			   		}
			   		test_c=right_endline;
			   		test_d=last_stl;
			   		for(i=pright_endline;i>=last_stl;i--)
			   		{
			   			  sum_Block=sum_Block+(int)((((p_point_up[i]+p_point_down[i])/2)-Line_Center)*quan[x])/10;
			   			  x++;
			   		}
			   		if(left_stratline>right_startline)
			   		{
			   			  for(i=left_stratline-1;i>=right_startline;i--)
			   			  {
			   			  		is_odd=(is_odd+1)%2;
			   			  		if(is_odd) 
			   			  		{
				   			  		sum_Block+=(int)((((p_point_down[i]+p_point_up[left_stratline])/2)-Line_Center)*quan[x])/10;
				   			  		x++;
			   			  		}	
			   			  } 
			   		} 
			   		else
			   		{
			   			  for(i=right_startline-1;i>=left_stratline;i--)
			   			  {     
			   			        is_odd=(is_odd+1)%2; 
			   			        if(is_odd)
			   			        {
				   			  		sum_Block+=(int)((((p_point_up[i]+p_point_down[right_startline])/2)-Line_Center)*quan[x])/10;
				   			  		x++;
			   			        }
			   			  }
			   		}
			   }
			   else
			   {
			   		for(i=pright_endline;i>pleft_endline;i--)
			   		{
			   			is_odd=(is_odd+1)%2;
			   			if(is_odd)
			   			{
			   			  sum_Block+=(int)((((p_point_down[i]+p_point_up[left_endline])/2)-Line_Center-23)*quan[x])/10;
			   			  x++;
			   			}
			   		}
			   		for(i=pleft_endline;i>=last_stl;i--)
			   		{
			   			  sum_Block=sum_Block+(int)((((p_point_up[i]+p_point_down[i])/2)-Line_Center)*quan[x])/10;
			   			  x++;
			   		}
			   		if(left_stratline>right_startline)
			   		{
			   			  for(i=left_stratline-1;i>=right_startline;i--)
			   			  {
			   			  		is_odd=(is_odd+1)%2;
			   			  		if(is_odd)
			   			  		{
				   			  		sum_Block+=(int)((((p_point_down[i]+p_point_up[left_stratline])/2)-Line_Center)*quan[x])/10;
				   			  		x++;
			   			  		}
			   			  }
			   		} 
			   		else
			   		{
			   			  for(i=right_startline-1;i>=left_stratline;i--)
			   			  {
			   			  		is_odd=(is_odd+1)%2;
			   			  		if(is_odd) 
			   			  		{
				   			  		sum_Block+=(int)((((p_point_up[i]+p_point_down[right_startline])/2)-Line_Center)*quan[x])/10;
				   			  		x++;
			   			  		}
			   			  }
			   		}
			   }
			   	
		 }
		 test_sum=0.3;
}
void  find_edge() 
{	 
    uchar i;
	if(find_left_startline&&find_right_startline)
	{
		big_left=p_point_up[left_stratline];
		little_right=p_point_down[right_startline];
		for(i=0;i<=left_endline;i++)
		{
			 if(big_left<p_point_up[i]&&p_point_up[i]!=0)	 big_left=p_point_up[i];
		}
		for(i=0;i<=right_endline;i++)
		{
			if(little_right>p_point_down[i]&&p_point_down[i]!=0)  little_right=p_point_down[i];
		}
	}
}
void is_will_be_out()
{
      uchar i;
	  int   t_left,t_right;
	  uchar t_left_line,t_right_line;
	  int  angle_left,angle_right;
	  t_left=p_point_up[left_stratline];
	  t_left_line=left_stratline;
	  t_right_line=right_startline;
	  t_right=p_point_down[right_startline];
	  angle_left=Steer_Mid-70*((int)(p_point_up[left_stratline]-PLEFT))/(5+left_stratline/3);
	  angle_right=Steer_Mid-70*((int)(p_point_down[right_startline]-PRIGHT))/(5+right_startline/3);
      if(find_left_startline&&find_right_startline)
      {
      	  if(left_endline>5)
      	  {
      	        if(left_endline>=25)
      	        {
	      	  		for(i=left_stratline;i<25;i++)
	      	  		{
	      	  			 if(angle_left>Steer_Mid-70*((int)(p_point_up[i]-PLEFT))/(5+i/3))
	      	  			 {
	      	  			 	t_left=p_point_up[i];
	      	  			 	t_left_line=i;
	      	  			 	angle_left=Steer_Mid-70*((int)(p_point_up[i]-PLEFT))/(5+i/3);
	      	  			 }
	      	  		}   
      	        }
      	        else
      	        {
      	        	for(i=left_stratline;i<left_endline;i++)
	      	  		{
	      	  			 if(angle_left>Steer_Mid-70*((int)(p_point_up[i]-PLEFT))/(5+i/3))
	      	  			 {
	      	  			 	t_left=p_point_up[i];
	      	  			 	t_left_line=i;
	      	  			 	angle_left=Steer_Mid-70*((int)(p_point_up[i]-PLEFT))/(5+i/3);
	      	  			 }
	      	  		}   
      	        }
      	       angle_left=angle_left-0;
      	  	   if(SteerAngle>angle_left)
      	       {
      	       	   SteerAngle=angle_left;
      	       	   will_be_out_left=1;
      	       	   if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
	               if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
      	       }
      	  }
      	  if(right_endline>5)
      	  {
      	        if(right_endline>=25)
      	        {
	      	  	    for(i=right_startline;i<25;i++)
	      	  		{
	      	  			 if(angle_right<Steer_Mid-70*((int)(p_point_down[i]-PRIGHT))/(5+i/3))
	      	  			 {
	      	  			 	t_right=p_point_down[i];
	      	  			 	t_right_line=i;
	      	  			 	angle_right=Steer_Mid-70*((int)(p_point_down[i]-PRIGHT))/(5+i/3);
	      	  			 }
	      	  		}   
      	        } 
      	        else
      	        {
      	        	for(i=right_startline;i<right_endline;i++)
	      	  		{
	      	  			 if(angle_right<Steer_Mid-70*((int)(p_point_down[i]-PRIGHT))/(5+i/3))
	      	  			 {
	      	  			 	t_right=p_point_down[i];
	      	  			 	t_right_line=i;
	      	  			 	angle_right=Steer_Mid-70*((int)(p_point_down[i]-PRIGHT))/(5+i/3);
	      	  			 }
	      	  		}  	
      	        }
      	       angle_right=angle_right+0;
      	       //angle_right=angle_right-100;
      	  	   if(SteerAngle<angle_right)
               {
      	           SteerAngle=angle_right;
      	           will_be_out_right=1;
      	           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
	               if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
      	       }	
      	  }
      	  pt_left=t_left;
      	  pt_right=t_right;
      	  pangle_left=angle_left;
      	  pangle_right=angle_right;
      	  pt_left_line=t_left_line;
      	  pt_right_line=t_right_line;
      }
}
void pnew_duoji_control() 
{
	  uchar i;
	  xiuzheng_error=0; 
	  {
	  		condition_1=0;condition_2=0;condition_3=0;condition_4=0;condition_5=0;condition_6=0;condition_7=0;condition_8=0;condition_9=0;condition_10=0;condition_11=0;condition_12=0;
	  		condition_13=0;condition_14=0;condition_15=0;condition_16=0;condition_17=0;condition_18=0;condition_19=0;condition_20=0;
	  }
	  error=(p_point_up[left_endline]+p_point_down[right_endline])>>1;
  	  test_error=(error>>1)+(mid_error>>1);
  	  test_left=point_up[left_endline];
  	  test_right=point_down[right_endline];
  	  test_a=left_endline;
  	  test_b=right_endline;
  	  test_left_stl=left_stratline;
  	  test_right_stl=right_startline;
  	  test_stl_error=point_up[left_stratline];
  	  test_str_error=point_down[right_startline];
  	  test_flag_shizi=flag_shizi;
  	  valium=(left_endline+right_endline)>>1;
  	  mid_valium=valium>>1;
  	  pright_endline=right_endline;
  	  pleft_endline=left_endline;
	  pto_get_sumblock();
	  pre_x_error=x_error;
	  x_error=sum_Block/x;
	  shorter_line=left_endline<right_endline?left_endline:right_endline;
	  if(!find_left_startline&&!find_right_startline)
	  {
	  	  condition_1=1;
	  	  return;
	  }
	  if(find_left_startline&&find_right_startline) 
	  {
	  	  xiuzheng_error=(((p_point_up[left_stratline]+p_point_down[right_startline])>>1)-Line_Center);
	  	  if(valium<20||right_endline<4||left_endline<4)
	  	  {
	  	  	   xiuzheng_error=1;	
	  	  }
	  }
	  
	  if(flag_shizi)
  	  {  
  	      if(flag_find_aftershizi)
  	      {
	           error=(p_point_up[left_endline]+p_point_down[right_endline])>>1;
	           if(valium<5)
  	           {
  	  	    	  p_SteerAngle0=error<Line_Center?Steer_Left:Steer_Right;
  	  	    	  condition_2=1;
  	  		      return;
  	           }
	      	   SteerAngle0=110*(error-Line_Center)/(((right_endline+left_endline)>>1)-3);
	           SteerAngle=Steer_Mid-SteerAngle0;
	           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
	           if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
	           p_SteerAngle0=SteerAngle;
	           condition_3=1;
	           return;
  	      }
  	      else
  	      {
               p_SteerAngle0=Steer_Mid;
               condition_4=1;
               return;
  	      }
  	  }
  	  if(cabs_uchar(right_endline-left_endline)>20&&(right_endline>40||left_endline>40)&&num_shizi_chang<200)
  	  {
  	  	   if(right_endline>left_endline)
  	  	   {
  	  	           if(aver_pwm/num_pwm<Steer_Mid)
  	  	           {
	  	  	   		   error=p_point_down[right_endline]-30;
			      	   SteerAngle0=140*(error-Line_Center)/(right_endline-3);
			           SteerAngle=Steer_Mid-SteerAngle0;
			           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
			           if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
			           p_SteerAngle0=SteerAngle;
			           return;
  	  	           }
  	  	           else
  	  	           {
  	  	           	   SteerAngle=Steer_Left-80;
  	  	           	   is_will_be_out();
  	  	           	   p_SteerAngle0=SteerAngle;
  	  	           	   return;
  	  	           }
  	  	   }
  	  	   if(right_endline<left_endline)
  	  	   {
  	  	           if(aver_pwm/num_pwm>Steer_Mid)
  	  	           { 
	  	  	     	   error=p_point_up[left_endline]+30;
			      	   SteerAngle0=140*(error-Line_Center)/(left_endline-3);
			           SteerAngle=Steer_Mid-SteerAngle0;
			           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
			           if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
			           p_SteerAngle0=SteerAngle;
			           return;
  	  	           }
  	  	           else
  	  	           {
  	  	           
  	  	           	   SteerAngle=Steer_Right+130;
  	  	           	   is_will_be_out();
  	  	           	   p_SteerAngle0=SteerAngle;
  	  	           	   return;	
  	  	           }
  	  	   }
  	  }
  	  if(cabs_uchar(right_endline-left_endline)>7&&num_shizi_chang<200)
  	  {
  	  		if(right_endline>left_endline&&left_endline<20&&point_up[left_endline]<80&&point_down[right_endline]>160)
  	  		{
  	  		    SteerAngle=Steer_Left-120;
  	  		    is_will_be_out();
  	  		    p_SteerAngle0=SteerAngle;
  	  		    condition_12=1;
  	  	   		return;		
  	  		}
  	  		if(right_endline<left_endline&&right_endline<20&&point_up[left_endline]<85&&point_down[right_endline]<180)
  	  		{
  	  		    SteerAngle=Steer_Right+120;
  	  		    condition_13=1;
  	  		    is_will_be_out();
  	  		    p_SteerAngle0=SteerAngle;
  	  	   		return;		
  	  		}
  	  }
  	  if(cabs_uchar(right_endline-left_endline)<3&&right_endline<22&&cabs_uchar(right_startline-left_stratline)>6&&num_shizi_chang<200)
  	  {
  	  		if(right_startline>left_stratline)
  	  		{
  	  			p_SteerAngle0=Steer_Right;
  	  			return;	
  	  		}
  	  		else
  	  		{
  	  			p_SteerAngle0=Steer_Left;
  	  			return;
  	  		}
  	  }
  	  
  	 /*if(cabs_uchar(right_endline-left_endline)>40)
  	  {
  	  			if(right_endline>left_endline&&point_down[right_endline]<Line_Center-30)
  	  			{
  	  				   error=p_point_down[right_endline]-50;;
			      	   SteerAngle0=110*(error-Line_Center)/(right_endline-4);
			           SteerAngle=Steer_Mid-SteerAngle0;
			           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
			           if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
			           p_SteerAngle0=SteerAngle;
			           return;
  	  			} 
  	  			if(right_endline<left_endline&&point_up[left_endline]>Line_Center+30)
  	  			{
  	  				   error=p_point_up[left_endline]+50;;
			      	   SteerAngle0=110*(error-Line_Center)/(left_endline-4);
			           SteerAngle=Steer_Mid-SteerAngle0;
			           if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
			           if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
			           p_SteerAngle0=SteerAngle;
			           return;
  	  			}
  	  }*/

  	  if(turn_left||turn_right)
  	  {
  	  		if(turn_left)  { p_SteerAngle0=Steer_Left; condition_5=1;return;}
  	  		if(turn_right) { p_SteerAngle0=Steer_Right;condition_6=1;return;}
  	  }
  	  if(valium<7)
  	  {
  	        if(left_endline>right_endline+5)
  	        {
  	        	p_SteerAngle0=Steer_Right;
  	        	return;	
  	        }
  	        if(right_endline>left_endline+5) 
  	        {
  	        	p_SteerAngle0=Steer_Left;
  	        	return;		
  	        }
  	  		p_SteerAngle0=error<Line_Center?Steer_Left:Steer_Right;
  	  		condition_7=1;
  	  		return;
  	  }
  	  if(cabs_uchar(right_endline-left_endline)>10)//test
  	  {
  	  	   if(right_endline>left_endline&&point_down[right_endline]<Line_Center&&right_endline<15) 
  	  	   {
  	  	   		p_SteerAngle0=Steer_Left;
  	  	   		condition_8=1; 
  	  	   		return;
  	  	   }
  	  	   if(right_endline<left_endline&&point_up[left_endline]>Line_Center&&left_endline<15) 
  	  	   {
  	  	   	   p_SteerAngle0=Steer_Right;
  	  	   	   condition_9=1;
  	  	   	   return;	
  	  	   }
  	  	   if(right_endline>left_endline+15&&point_down[right_endline]<Line_Center-20&&right_endline<22)
  	  	   {
  	  	   		p_SteerAngle0=Steer_Left;
  	  	   		condition_10=1; 
  	  	   		return;	
  	  	   }
  	  	   if(left_endline>right_endline+15&&point_up[left_endline]>Line_Center+20&&left_endline<22)
  	  	   {
  	  	   		p_SteerAngle0=Steer_Right;
  	  	   		condition_11=1; 
  	  	   		return;
  	  	   }
  	  }
  	  if(cabs_uchar(right_endline-left_endline)>7)
  	  {
  	  		if(right_endline>left_endline&&left_endline<13&&point_up[left_endline]<90&&point_down[right_endline]>168&&point_down[right_endline]+point_down[right_startline]-point_down[(right_endline+right_startline)/2]*2>20)
  	  		{
  	  		    SteerAngle=Steer_Left-120;
  	  		    p_SteerAngle0=SteerAngle;
  	  		    condition_12=1;
  	  	   		return;		
  	  		}
  	  		if(right_endline<left_endline&&right_endline<13&&point_up[left_endline]<100&&point_down[right_endline]>170&&point_up[(left_endline+left_stratline)/2]*2-point_up[left_endline]-point_up[left_stratline]>35)
  	  		{
  	  		    SteerAngle=Steer_Right+120;
  	  		    condition_13=1;
  	  		    p_SteerAngle0=SteerAngle;
  	  	   		return;		
  	  		}
  	  }
  	  if(shorter_line<16)
	  {   
	     use_ad_con=1;
	  	 duoji_control();
	 	 condition_14=1;
	  	 return;		    
	 }
	  ee_x_error=x_error-pre_x_error;
  	 if(left_endline<46||right_endline<46)
  	  {
  	    if(x_error<0)
  	    {
  	  		SteerAngle1=5*x_error;
  	  	}
  	  	else
  	  	{
  	  		SteerAngle1=5*x_error;
  	  	}
  	  }
  	  else
  	  {
  	      if(x_error<0)
  	      {
  	      	 SteerAngle1=3*x_error;
  	      }
  	      else
  	      {
  	      	 SteerAngle1=4*x_error;
  	      }
  	  }
      SteerAngle0=SteerAngle1+xiuzheng_error;
      SteerAngle=Steer_Mid-SteerAngle0;
     /*if(valium>40)
      {
      	  SteerAngle=(SteerAngle-now_pwmdty67)/2+SteerAngle;
      }
      else if(valium>30)
      {
      	  SteerAngle=(SteerAngle-now_pwmdty67)*1/2+SteerAngle;
      } 
      else if(valium>20)  
      {
      	  SteerAngle=(SteerAngle-now_pwmdty67)*2/2+SteerAngle;
      } 
      else 
      {
      	  SteerAngle=(SteerAngle-now_pwmdty67)*2/2+SteerAngle;
      }*/
      if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
      if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;									  
      is_will_be_out();
      now_pwmdty67=PWMDTY67;
      p_SteerAngle0=SteerAngle;
      condition_15=1;
      return;  
}
void      duoji_control() 
{
  	  error=(p_point_up[left_endline]+p_point_down[right_endline])>>1;
      mid_error=(p_point_up[left_endline>>1]+p_point_down[right_endline>>1])>>1;
  	  test_error=(error>>1)+(mid_error>>1);
  	  test_left=p_point_up[left_endline];
  	  test_right=p_point_down[right_endline];
  	  test_a=left_endline;
  	  test_b=right_endline;
  	  test_left_stl=left_stratline;
  	  test_right_stl=right_startline;
  	  test_stl_error=p_point_up[left_stratline];
  	  test_str_error=p_point_down[right_startline];
  	  test_flag_shizi=flag_shizi;
  	  valium=(left_endline+right_endline)>>1;
  	  mid_valium=valium>>1;
  	  if(shorter_line<4)
  	  {
  	  		p_SteerAngle0=error<Line_Center?Steer_Left:Steer_Right;
  	  		condition_16=1;
  	  		return;
  	  }
  	  if(cabs_uchar(left_endline-right_endline)>17) 
  	  {
  	  		if(right_endline>left_endline&&left_endline<2)
  	  		{
  	  			 SteerAngle=Steer_Left;p_SteerAngle0=SteerAngle;condition_17=1;return;
  	  		}
  	  		if(right_endline<left_endline&&right_endline<2) 
  	  		{
  	  			 SteerAngle=Steer_Right;p_SteerAngle0=SteerAngle;condition_18=1;return;
  	  		}
  	  }
  	  SteerAngle0=30*(error-Line_Center)/(shorter_line-3);
      SteerAngle=Steer_Mid-SteerAngle0;
      if(SteerAngle<Steer_Right)    SteerAngle=Steer_Right;
      if(SteerAngle>Steer_Left)     SteerAngle=Steer_Left;
      now_pwmdty67=PWMDTY67;
      p_SteerAngle0=SteerAngle;
      condition_19=1;
      return;
}
void  enstart_duoji()
{
	PWME_PWME7=0;
	PWMDTY67=p_SteerAngle0;
	if(PT1AD0_PT1AD03==1)
	{
       PWME_PWME7=1;
	} 
	else
	{
		_asm(nop);
	}
    if(num_shizi_chang<1)
	{
	     aver_pwm=0;
		 num_pwm=0;	
	}
	if(num_shizi_chang<200)
	{
		 aver_pwm+=p_SteerAngle0;
		 num_pwm++;
	} 

}
void  clear_this_field_flag() 
{
	 is_odd=0;
	 flag_ed=0;
	 num_find=0;
     last_stl=0;
     last_stl=0;
     test_bbhh=0;
     test_aa_rstl=0;
     test_aa_lstl=0;
     flag_shizi=0;
     flag_not_shizi=0;
     End_Line=0;
     use_ad_con=0;
     num_shizi=0;
     will_be_out=0;
     will_be_out_left=0;
     will_be_out_right=0;
     find_last_left=0;
     find_last_right=0;
     Pre_Line_Save=0;
     is_have_a_lstl=0;
     is_have_a_rstl=0;
     sum_Block=0;
     turn_left=0;
     turn_right=0;
     left_endline=0;
     right_endline=0;
     stop_find_right=0;
     stop_find_left=0;
     num_wrong_left=0;
     num_wrong_right=0;
     Flag_Lose_Line=0;    
     left_stratline=0;
     right_startline=0;
     find_left_startline=0;
     find_right_startline=0;
     flag_find_aftershizi=0;
     is_find_a_real_start=0;
     not_find_a_real_start=0;
     num_onlyfind_one_startline=0;
}
void   select_speed()                       //选择速度
{
	  if(PT1AD0_PT1AD04==1&&PT1AD0_PT1AD05==1)
	  {
	  	     zhidao_speed=270;//270	  300
             shizi_speed=170;
             refer_speed=200;//200
             limit_speed=160;
	  }
	  else if(PT1AD0_PT1AD04==1&&PT1AD0_PT1AD05==0)
	  {
	  		 zhidao_speed=240;
             shizi_speed=160;
             refer_speed=190;
             limit_speed=150;
	  } 
	  else if(PT1AD0_PT1AD04==0&&PT1AD0_PT1AD05==1)
	  {
	  		 zhidao_speed=220;
             shizi_speed=150;
             refer_speed=180;     //参考速度
             limit_speed=140;
	  }
	  else if(PT1AD0_PT1AD04==0&&PT1AD0_PT1AD05==0)
	  {
	  	       zhidao_speed=200;
             shizi_speed=150;
             refer_speed=170;
             limit_speed=140;	
	  }
	  if(PT1AD0_PT1AD02==1)
	  {
	      is_use_start=1;
	  } 
	  else
	  {
	  	  is_use_start=0;
	  }
	  if(PT1AD0_PT1AD06==1)
	  {
	  	  flag_use_special_cutspeed=1;	
	  }
	  else
	  {
	  	  flag_use_special_cutspeed=0;	
	  }
}
void      set_speed() 
{
  PWM=PWMDTY67;
  angle=PWM-Steer_Mid;
  angle=angle/10;
  find_edge();
  if(num_shizi_chang<210&&(test_a<=40||test_b<=40)&&!flag_shizi)
  {
  	 MyPID.TSpeed=shizi_speed;
  	 return;	
  }
  if(find_left_startline&&find_right_startline
  &&test_b>47&&test_a>47&&angle>-20&&angle<20
  &&point_up[left_stratline]<Line_Center-30
  &&point_down[right_startline]>Line_Center+30
  &&little_right>Line_Center+15
  &&big_left<Line_Center-15)										   
  {
        num_zhidao++;
        need_cut_speed=0;
        sum_cut=0;
        if(num_zhidao>100)
        {
            num_zhidao=100;
        }
        
  }
  else
  {
       if(num_zhidao>1)
       {
       		num_zhidao=1;
       }
       else
       {
           num_zhidao=0;
       }
  }
  if(num_zhidao>=1)
  {
       if(sum_wandao>3)
  	   {
  	 	  sum_zhidao=0;
  	   }
  	   else
  	   {
  	   	  sum_zhidao++;
  	   }
  	   sum_wandao=0;
  	   if(CurrentSpeed>zhidao_speed-30)
  	   {
  	   	   MyPID.TSpeed=zhidao_speed;	
  	   }
  	   else
  	   {
  	    	MyPID.TSpeed=(zhidao_speed-CurrentSpeed)*30+zhidao_speed;	
  	   }
  	   if(MyPID.TSpeed>max_speed)
  	   {
  	   	  MyPID.TSpeed=max_speed;	
  	   }
  	   if(MyPID.TSpeed<zhidao_speed)
  	   {
  	   	  MyPID.TSpeed=zhidao_speed;
  	   }
	   if(sum_zhidao>20000)
	   {
	     	sum_zhidao=20000;	
	   }
  	   return;
  }
  if(CurveSpeed<120)
  {
    	sum_zhidao=0;	
  }
  if(sum_zhidao)
  {	  
  	 sum_wandao++;
  	 if(sum_wandao>100)
  	 {
  	 	  sum_wandao=100;
  	 }
     if(flag_use_special_cutspeed==0) 
	 {
	      StraightSpeed=190;   
	      CurveSpeed   =120;
	      test=1700;
	      MyPID.TSpeed=refer_speed-angle*angle*(StraightSpeed-CurveSpeed)/test;
		   if(MyPID.TSpeed<limit_speed)
			{
				MyPID.TSpeed=limit_speed;
			}
		   if(((left_endline<15&&right_endline<30)||(right_endline<15&&left_endline<30))&&angle<30&&angle>-30)
		   {
		  	    MyPID.TSpeed=2*MyPID.TSpeed/3;
		  	    if(MyPID.TSpeed<140)
				{
					MyPID.TSpeed=140;
				}
		   }
		   if(CurrentSpeed>MyPID.TSpeed+0)
		   {
		   		  MyPID.TSpeed=0;
		   }
		   else
		   {
		         if(sum_zhidao<20)
		         {
		   		     sum_zhidao=0;
		         }
		         else
		         {
		         	sum_zhidao=sum_zhidao-5;
		         	MyPID.TSpeed=MyPID.TSpeed-80-sum_zhidao/5;
		         	if(MyPID.TSpeed<10)
		         	{
		         		MyPID.TSpeed=10;
		         	}
		         }
		   }
	     
	      if(MyPID.TSpeed>zhidao_speed-140)
	      {
	      	 MyPID.TSpeed=zhidao_speed-140;
	      }
	      if(MyPID.TSpeed<min_speed)
	      {
	      	 MyPID.TSpeed=min_speed;	
	      }
	  	  return;
	  }
	  else//特殊刹车
	  {
	      sum_wandao++;
	  	  if(sum_wandao>100)
	  	  {
	  	 	  sum_wandao=100;
	  	  }
	      StraightSpeed=190;   
	      CurveSpeed   =120;
	      test=1700;
	      MyPID.TSpeed=refer_speed-angle*angle*(StraightSpeed-CurveSpeed)/test;
		   if(MyPID.TSpeed<limit_speed)
			{
				MyPID.TSpeed=limit_speed;
			}
		   if(((left_endline<15&&right_endline<30)||(right_endline<15&&left_endline<30))&&angle<30&&angle>-30)
		   {
		  	    MyPID.TSpeed=2*MyPID.TSpeed/3;
		  	    if(MyPID.TSpeed<140)
				{
					MyPID.TSpeed=140;
				}
		   }
		   if(CurrentSpeed>MyPID.TSpeed-50)
		   {
		          
		   		  MyPID.TSpeed=0;
		   }
		   else
		   {
		         if(sum_zhidao<10)
		         {
		            if(sum_zhidao>5)
		            {
		            	sum_zhidao=4;
		            	MyPID.TSpeed=15;
		            }
		            else if(sum_zhidao==4)
		            {
		            	sum_zhidao=3;
		            	MyPID.TSpeed=15;	
		            }
		            else if(sum_zhidao==3)
		            {
		            	sum_zhidao=2;
		            	MyPID.TSpeed=15;
		            }
		            else if(sum_zhidao==2)
		            {
		            	sum_zhidao=1;
		            	MyPID.TSpeed=15;	
		            } 
		            else
		            { 
		   		        sum_zhidao=0;
		   		    }
		         }
		         else
		         {
		         	sum_zhidao=sum_zhidao-3;
		         	MyPID.TSpeed=15;
		         }
		   }
	     
	      if(MyPID.TSpeed>zhidao_speed-140)
	      {
	      	 MyPID.TSpeed=zhidao_speed-140;
	      }
	      if(MyPID.TSpeed<min_speed)
	      {
	      	 MyPID.TSpeed=min_speed;	
	      }
	  	  return;
	  }
  }
  sum_wandao++;
  if(sum_wandao>100)
  {
 	  sum_wandao=100;
  }
  if(sum_wandao>3)
  {
 	  sum_zhidao=0;
  }
  StraightSpeed=190;   
  CurveSpeed   =120;
  test=1700;
  if(sum_little_angle>10)
  {
 		if(left_endline<28||right_endline<28) 
  		{
  			MyPID.TSpeed=0;
  			sum_little_angle=0;
  			
  			return;   
  		}
  }
 
   if(sum_little_angle<=10)
   {
	  if(angle<15&&angle>-15)
	  {
	  		sum_little_angle++;
	  }	
	  else
	  {	   
	  	   sum_little_angle=0;	
	  } 
   } 
   MyPID.TSpeed=refer_speed-angle*angle*(StraightSpeed-CurveSpeed)/test;
   if(MyPID.TSpeed<limit_speed)
	{
		MyPID.TSpeed=limit_speed;
	}
   if(((left_endline<15&&right_endline<30)||(right_endline<15&&left_endline<30))&&angle<30&&angle>-30)
   {
  	    MyPID.TSpeed=2*MyPID.TSpeed/3;
  	    if(MyPID.TSpeed<80)
		{
			MyPID.TSpeed=80;
		}
   }
   return;
}
uchar  cabs_uchar(char b) 
{
    if(b>0)  return b;
    else     return -b;
}
uint  cabs_uint(int b) 
{
    if(b>0)  return b;
    else     return -b;
}
int  cabs_char(char b) 
{
	if(b>0) return b;
	else    return -b;
}
void init_all() 
{
  PLL_Init();
  ECT0_init();
  ECT_Init();
  PWM_Init();
  PIT_Init();
  PIT1_init();
  Dial_Switch_init();
  PIDInit();
  DDRB=0xff;//B口接发光二极管，初始化为输出
  DDR1AD0=0x00;
  dian_sha=0;
}
void Init_sd(void) 
{
   SPIInit();
   SD_Reset();
   SD_Init();
}
void is_shizi() 
{
    if(flag_not_shizi)
    {
     	num_shizi=0;
     	flag_shizi=0;	
    	return;
    }
    if(v_point_up[Pre_Line_Save][0]+v_point_up[Pre_Line_Save][1]+v_point_up[Pre_Line_Save][2]+v_point_up[Pre_Line_Save][3]+v_point_up[Pre_Line_Save][4]+v_point_down[Pre_Line_Save][0]+v_point_down[Pre_Line_Save][1]+v_point_down[Pre_Line_Save][2]+v_point_down[Pre_Line_Save][3]+v_point_down[Pre_Line_Save][4]==0)
    {
    	flag_not_shizi=1;
    	num_shizi=0;
     	flag_shizi=0;
    	return;
    }
	if((find_right_startline&&point_down[right_endline]<Line_Center+10)||(find_left_startline&&point_up[left_endline]>Line_Center-10))
    {
		_asm(nop);
	} 
	else
	{
		if(v_point_up[Pre_Line_Save][0]<54&&v_point_down[Pre_Line_Save][0]>184&&v_point_up[Pre_Line_Save][1]==0&&v_point_up[Pre_Line_Save][2]==0&&v_point_up[Pre_Line_Save][3]==0&&v_point_up[Pre_Line_Save][4]==0&&v_point_down[Pre_Line_Save][1]==0&&v_point_down[Pre_Line_Save][2]==0&&v_point_down[Pre_Line_Save][3]==0&&v_point_down[Pre_Line_Save][4]==0)
		{
		      num_shizi++;
		} 
		else
	    {
	          num_shizi=0;
		}
	}
	if(num_shizi>1) 
	{
		 if(find_right_startline&&!find_left_startline)
		 {
		 	if((point_down[right_startline]-point_down[right_endline])/(right_endline-right_startline)>4)
		 	{
		 		num_shizi=0;
		 		flag_shizi=0;
		 		return;
		 	}
		 	if(right_endline>10)
		 	{
		 		num_shizi=0;
		 		flag_shizi=0;
		 		return;
		 	}
		 }
		 
		 if(!find_right_startline&&find_left_startline)
		 {
		 		if((point_up[right_endline]-point_up[right_startline])/(right_endline-right_startline)>4)
		 		{
		 			num_shizi=0;
			 		flag_shizi=0;
			 		return;	
		 		}
		 	   if(left_endline>10)
			 	{
			 		num_shizi=0;
			 		flag_shizi=0;
			 		return;
			 	}

		 }
		 if(not_find_a_real_start)
		 {
		 		if(num_shizi>=4)
		 		{
		 		     flag_shizi=1;		
		 		}
		 } 
		 else
		 {
		      flag_shizi=1;
		 }
	}
}
void is_the_startline() 
{
	  uchar i,j;
	  for(i=0;i<5;i++)
	  {
	  		if(point_up[Pre_Line_Save-1]==v_point_up[Pre_Line_Save-1][i])
	  		break;
	  }
	  for(j=0;j<5;j++)
	  {
	  	  if(point_down[Pre_Line_Save-1]==v_point_down[Pre_Line_Save-1][j])
	  	  break;
	  }
	  /*if(j<=2&&point_down[Pre_Line_Save-1]-v_point_down[Pre_Line_Save-1][j+1]>20&&point_down[Pre_Line_Save-1]-v_point_down[Pre_Line_Save-1][j+1]<40&&v_point_down[Pre_Line_Save-1][j+1]>LineMin&&v_point_down[Pre_Line_Save-1][j+1]<LineMax)
	  {
	  	  is_startline1=1;
	  } 
	  else
	  {
	  	  is_startline1=0;
	  }
	  if(is_startline1)
	  {
	  		if(j<=2&&v_point_down[Pre_Line_Save-1][j+1]-v_point_down[Pre_Line_Save-1][j+2]>20&&v_point_down[Pre_Line_Save-1][j+1]-v_point_down[Pre_Line_Save-1][j+2]<45&&v_point_down[Pre_Line_Save-1][j+2]>LineMin&&v_point_down[Pre_Line_Save-1][j+2]<LineMax)
	  		{
	  			 is_startline2=1;
	  		}
	  		else
	  		{
	  			 is_startline2=0;
	  		}
	  }
	  if(is_startline1&&is_startline2)
	  {
	  		if(i>=2&&v_point_up[Pre_Line_Save-1][i-1]-point_up[Pre_Line_Save-1]>8&&v_point_up[Pre_Line_Save-1][i-1]-point_up[Pre_Line_Save-1]<30&&v_point_up[Pre_Line_Save-1][i-1]>LineMin&&v_point_up[Pre_Line_Save-1][i-1]<LineMax)
	  		{
	  			is_startline3=1;
	  		} 
	  		else
	  		{
	  			 is_startline3=0;
	  		}
	  		
	  }
	  if(is_startline1&&is_startline2&&is_startline3)
	  {
	  		  if(i>=2&&v_point_up[Pre_Line_Save-1][i-2]-v_point_up[Pre_Line_Save-1][i-1]>10&&v_point_up[Pre_Line_Save-1][i-2]-v_point_up[Pre_Line_Save-1][i-1]<35&&v_point_up[Pre_Line_Save-1][i-2]>LineMin&&v_point_up[Pre_Line_Save-1][i-2]<LineMax)
	  		  {
	  		  	 is_startline4=1;	
	  		  }
	  		  
	  }
	  if(is_startline1&&is_startline2&&is_startline3&&is_startline4)
	  {
	  	is_startline=1;	
	  }*/
	  if(v_point_down[Pre_Line_Save-1][j+1]>point_up[Pre_Line_Save-1]+7&&point_up[Pre_Line_Save-1]!=0&&v_point_up[Pre_Line_Save-1][i-1]<point_down[Pre_Line_Save-1]-7&&point_down[Pre_Line_Save-1]!=0&&v_point_down[Pre_Line_Save-1][j+1]!=0&&v_point_up[Pre_Line_Save-1][i-1]!=0)
	  {
	  		if(v_point_down[Pre_Line_Save-1][j+1]>v_point_up[Pre_Line_Save-1][i-1]+10||v_point_down[Pre_Line_Save-1][j+1]<v_point_up[Pre_Line_Save-1][i-1]-10)
	  		{
	  	          is_startline1=1;	
	  		}
	  }
}
to_find_both_startline_new()        //找到两边开始的线
{
	 uchar i,j;
	 uchar flag_find_first;
	 left_block0=0;
	 flag_find_first=0;
  	 find_left=0;
	 find_right=0;
	 pfind_left_startline=0;
	 pfind_right_startline=0;
	 right_block0=500;
	 for(i=0;i<5;i++)
	 {
	 	 if((v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][i])>85)
	 	 {
	 	 		if(!flag_find_first)
	 	 		{
		 	 		if(find_last_rstl) 
					{
						if(LineMax>v_point_down[Pre_Line_Save][i]&&v_point_down[Pre_Line_Save][i]>(Line_Center-50)) 
						{
							 right_block0=v_point_down[Pre_Line_Save][i];
							 find_right_startline=1;
							 pfind_right_startline=1;
						}
					}
					else
					{
						if(LineMax>v_point_down[Pre_Line_Save][i]&&v_point_down[Pre_Line_Save][i]>Line_Center) 
						{
							 right_block0=v_point_down[Pre_Line_Save][i];
							 find_right_startline=1;
							 pfind_right_startline=1;
						}	
					}
				    if(find_last_lstl)
					  {
						  if(LineMin<v_point_up[Pre_Line_Save][i]&&v_point_up[Pre_Line_Save][i]<(Line_Center+50))
						  {
						  		left_block0=v_point_up[Pre_Line_Save][i];
						  		find_left_startline=1;
						  		pfind_left_startline=1;			
						  }
					  } 
					  else
					  {
					  	  if(LineMin<v_point_up[Pre_Line_Save][i]&&v_point_up[Pre_Line_Save][i]<Line_Center)
						  {
						  		left_block0=v_point_up[Pre_Line_Save][i];
						  		find_left_startline=1;
						  		pfind_left_startline=1;			
						  }
					  }
					  if(find_left_startline&&find_right_startline)
					  {
					  	   flag_find_first=1;
					  }
	 	 		}
	 	 		else //根据实际情况，若有需要，则再补上，当前假定上面找到的第一个肯定是正确的
	 	 		{ 
	 	 	
	 	 	
	 	 		}
	 	 }
	 }
	 if(!flag_find_first)//滤除起始线
	 {	
	    uchar  temp_width;
	    uchar  temp_left_block0,temp_right_block0;
	    uchar  flag_find_firstt;
	    temp_width=0;
	    flag_find_firstt=0;														 
	 	for(i=0;i<5;i++)
	 	{
	 	    for(j=i;j<5;j++)
	 	    {
	 	        if(v_point_down[Pre_Line_Save][i]<LineMax&&v_point_down[Pre_Line_Save][i]>LineMin&&v_point_up[Pre_Line_Save][j]<LineMax&&v_point_up[Pre_Line_Save][j]>LineMin&&v_point_down[Pre_Line_Save][i]>v_point_up[Pre_Line_Save][j]+30)
	 	        {
	 	        	if(!flag_find_firstt)
	 	        	{
	 	        		  flag_find_firstt=1;
	 	        		  temp_left_block0=v_point_up[Pre_Line_Save][j];
	 	        		  temp_right_block0=v_point_down[Pre_Line_Save][i];
	 	        		  temp_width=v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][j];
	 	        	}
	 	        	else
	 	        	{
	 	        		  if(cabs_uchar(temp_width-Temp_Width)>cabs_uchar(v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][j]-Temp_Width)) 
	 	        		  {
	 	        		  	  temp_left_block0=v_point_up[Pre_Line_Save][j];
		 	        		  temp_right_block0=v_point_down[Pre_Line_Save][i];
		 	        		  temp_width=v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][j];	
	 	        		  }
	 	        	}
	 	        }
	 	    }
	 	}
	 	if(temp_width<150&&temp_width>80)
	 	{
	 		 find_left_startline=1;
	 		 find_right_startline=1;
	 		 left_block0=temp_left_block0;
	 		 right_block0=temp_right_block0;
	 		 flag_find_first=1;
	 	}
	 }
	 if(flag_find_first) 
	 {
	 	_asm(nop);
	 }
	 else
	 {
	 	 to_find_both_startline();
	 }
	if(pfind_left_startline) 
	{
	      if(!is_have_a_lstl){is_have_a_lstl=1;left_stratline=Pre_Line_Save;}
          find_left=1;          
	}
	if(pfind_right_startline)
	{
		  if(!is_have_a_rstl){is_have_a_rstl=1;right_startline=Pre_Line_Save;}
          find_right=1;	
     }
}
void to_find_both_line_new()    //找两边线
{
	 uchar ii;
	 uchar flag_find_first;
	 uint last_error,t_error;
	 flag_find_first=0;
	 left_block0=0;
  	 find_left=0;
	 find_right=0;
	 right_block0=0;
	 for(ii=0;ii<5;ii++)
	 {
	 	  if((v_point_down[Pre_Line_Save][ii]-v_point_up[Pre_Line_Save][ii])>30&&v_point_down[Pre_Line_Save][ii]>v_point_up[Pre_Line_Save][ii])
	 	  { 	   
	 	  		 if(!flag_find_first)
	 	  		 {   
		 	  		 if((cabs_uchar(v_point_up[Pre_Line_Save][ii]-point_up[Pre_Line_Save-1])<10||point_up[Pre_Line_Save-1]==0)&&v_point_up[Pre_Line_Save][ii]!=0&&v_point_up[Pre_Line_Save][ii]>LineMin) 
			   		 {
					   		if((cabs_uchar(v_point_down[Pre_Line_Save][ii]-point_down[Pre_Line_Save-1])<10||point_down[Pre_Line_Save-1]==0)&&v_point_down[Pre_Line_Save][ii]!=0&&v_point_down[Pre_Line_Save][ii]<(LineMax)) 
					   		{
					   			 find_left=1;
	                             find_right=1;
					   			 flag_find_first=1;
					   			 left_block0=v_point_up[Pre_Line_Save][ii];
					   			 right_block0=v_point_down[Pre_Line_Save][ii];
					   		}
			   		 }
		   		 }
		   		 else 
		   		 {
		   		 	last_error=cabs_uchar(left_block0-point_up[Pre_Line_Save-1])+cabs_uchar(right_block0-point_down[Pre_Line_Save-1]);
		   		 	t_error=cabs_uchar(v_point_up[Pre_Line_Save][ii]-point_up[Pre_Line_Save-1])+cabs_uchar(v_point_down[Pre_Line_Save][ii]-point_down[Pre_Line_Save-1]);
		   		 	if(last_error>t_error&&v_point_down[Pre_Line_Save][ii]>LineMin&&v_point_down[Pre_Line_Save][ii]<LineMax&&v_point_up[Pre_Line_Save][ii]>LineMin&&v_point_up[Pre_Line_Save][ii]<LineMax)
		   		 	{
		   		 		left_block0=v_point_up[Pre_Line_Save][ii];
					    right_block0=v_point_down[Pre_Line_Save][ii];	
		   		 	}
		   		 }
	 	  } 
	 	  
	 }
	 if(flag_find_first) 
	 {
	 	return;
	 } 
	 else
	 { 
	 	get_path_left(); 
	 	get_path_right();
	 	 if(find_left&&find_right)
	 	 {
	 	 	if(left_block0>right_block0-20)//宽度限制 
	 	 	{
	 	 		 find_left=0;
	 	 		 find_right=0;
	 	 	}
	 	 }
	 	 return;
	 }
}
to_find_both_startline() 
{
	uchar i;
	left_block0=0;
	find_left=0;
	find_right=0;
	right_block0=500;
	right_block0=v_point_down[Pre_Line_Save][0];
	if(LineMax>right_block0&&right_block0>(Line_Center-30)&&right_block0!=0) find_right_startline=1;else find_right_startline=0;
	for(i=0;i<4;i++)
	{
		if(find_last_rstl) 
		{
			if(LineMax>v_point_down[Pre_Line_Save][i+1]&&v_point_down[Pre_Line_Save][i+1]>(Line_Center-30)&&cabs_uchar(right_block0-pre_right_stl_error)>cabs_uchar(v_point_down[Pre_Line_Save][i+1]-pre_right_stl_error)&&v_point_down[Pre_Line_Save][i+1]!=0) 
			{
				 right_block0=v_point_down[Pre_Line_Save][i+1];
				 find_right_startline=1;
				 pfind_right_startline=1;
			}
		}
		else
		{
			if(LineMax>v_point_down[Pre_Line_Save][i+1]&&v_point_down[Pre_Line_Save][i+1]>(Line_Center-30)&&right_block0<v_point_down[Pre_Line_Save][i+1]&&v_point_down[Pre_Line_Save][i+1]>Line_Center&&v_point_down[Pre_Line_Save][i+1]!=0) 
			{
				 right_block0=v_point_down[Pre_Line_Save][i+1];
				 find_right_startline=1;
				 pfind_right_startline=1;
			}	
		}
	}
	left_block0=v_point_up[Pre_Line_Save][0];
	if(LineMin<left_block0&&left_block0<(Line_Center+30)&&left_block0!=0) {find_left_startline=1;} else    find_left_startline=0;
	for(i=0;i<4;i++) 
	{	  
		  if(find_last_lstl)
		  {
			  if(LineMin<v_point_up[Pre_Line_Save][i+1]&&v_point_up[Pre_Line_Save][i+1]<(Line_Center+30)&&cabs_uchar(left_block0-pre_left_stl_error)>cabs_uchar(v_point_up[Pre_Line_Save][i+1]-pre_left_stl_error)&&v_point_up[Pre_Line_Save][i+1]!=0)
			  {
			  		left_block0=v_point_up[Pre_Line_Save][i+1];
			  		find_left_startline=1;
                    pfind_left_startline=1;			  
              }
		  } 
		  else
		  {
		  	  if(LineMin<v_point_up[Pre_Line_Save][i+1]&&v_point_up[Pre_Line_Save][i+1]<(Line_Center+30)&&left_block0<v_point_up[Pre_Line_Save][i+1]&&v_point_up[Pre_Line_Save][i+1]<Line_Center&&v_point_up[Pre_Line_Save][i+1]!=0)
			  {
			  		left_block0=v_point_up[Pre_Line_Save][i+1];
			  		find_left_startline=1;
			  		pfind_left_startline=1;				
			  }
		  }
	}
    if(find_left_startline) 
	{
	      if(!is_have_a_lstl){is_have_a_lstl=1;left_stratline=Pre_Line_Save;}
          find_left=1;          
	}
	if(find_right_startline)
	{
		  if(!is_have_a_rstl){is_have_a_rstl=1;right_startline=Pre_Line_Save;}
          find_right=1;	
     }
}
to_find_left_startline()
{
	uchar i;
	find_left=0;
	left_block0=v_point_up[Pre_Line_Save][0];
	if(LineMin<left_block0&&left_block0<(Line_Center+30)&&left_block0!=0) {find_left_startline=1;}
	for(i=0;i<4;i++) 
	{										
		  if(LineMin<v_point_up[Pre_Line_Save][i+1]&&v_point_up[Pre_Line_Save][i+1]<(Line_Center+30)&&left_block0<v_point_up[Pre_Line_Save][i+1]&&v_point_up[Pre_Line_Save][i+1]!=0)
		  {
		  		left_block0=v_point_up[Pre_Line_Save][i+1];
		  		find_left_startline=1;
		  }
	}
	if(find_left_startline) 
	{
		  left_stratline=Pre_Line_Save;
		  find_left=1;
	}
}
to_find_right_startline()
{
	uchar i;
	find_right=0;
	right_block0=v_point_down[Pre_Line_Save][0];
	if(LineMax>right_block0&&right_block0>(Line_Center-30)&&right_block0!=0) find_right_startline=1;
	for(i=0;i<4;i++)
	{
		if(LineMax>v_point_down[Pre_Line_Save][i+1]&&v_point_down[Pre_Line_Save][i+1]>(Line_Center-30)&&right_block0>v_point_down[Pre_Line_Save][i+1]&&v_point_down[Pre_Line_Save][i+1]!=0) 
		{
			 right_block0=v_point_down[Pre_Line_Save][i+1];
			 find_right_startline=1;
		}
	}
	if(find_right_startline)
	{
		  right_startline=Pre_Line_Save;
		  find_right=1; 
	}	
}
void get_path_left() 
{
   unsigned char i;
   left_block0=0;
   find_left=0;
   left_block0=v_point_up[Pre_Line_Save][4];
   for(i=4;i>0;i--) 
   {
   		if(cabs_uchar(v_point_up[Pre_Line_Save][i-1]-point_up[Pre_Line_Save-1])<10&&v_point_up[Pre_Line_Save][i-1]>LineMin)
   		{
   			left_block0=v_point_up[Pre_Line_Save][i-1];
   		}
   }
	if(cabs_uchar(left_block0-point_up[Pre_Line_Save-1])<=10&&left_block0!=0&&left_block0>LineMin)
	{
		 find_left=1;
	}
	return ;
}
void to_start_or_stop()
{
	 /*	now_e0=PORTE_PE0;
    	if(now_e0==1&&pre_e0==0) 
		{
			  en_speed=(en_speed+1)%3;
		}
		if(1==en_speed)
		{
			TIE_C0I=1;
			PWME_PWME7=1;
		} 
		else if(0==en_speed)
		{
			TIE_C0I=0;
			PWME_PWME7=0;
		} 
		else
		{
			TIE_C0I=1;
		}
	    pre_e0=now_e0;*/
	    en_speed=1;
	    TIE_C0I=1;
	    //PWME_PWME7=1;
}
void get_path_right()
{
   uchar i;
   find_right=0;
   right_block0=v_point_down[Pre_Line_Save][0];
   for(i=0;i<4;i++) 
   {
   		if(cabs_uchar(v_point_down[Pre_Line_Save][i+1]-point_down[Pre_Line_Save-1])<10&&v_point_down[Pre_Line_Save][i+1]!=0&&v_point_down[Pre_Line_Save][i+1]<(LineMax))
   		{
   			 right_block0=v_point_down[Pre_Line_Save][i+1];
   		}
   }
	if(cabs_uchar(right_block0-point_down[Pre_Line_Save-1])<=10&&right_block0!=0&&right_block0<LineMax)
	{
		find_right=1;
	}
	return;
}
void to_save_thisfield_state() 
{
	 find_last_lstl=find_left_startline;
	 find_last_rstl=find_right_startline;
     pre_left_stl=left_stratline;
	 pre_right_stl=right_startline;
	 pre_left_stl_error=point_up[left_stratline];
	 pre_right_stl_error=point_down[right_startline];
	
}
void getline_when_shizi() // 十字时找边沿？
{
    uchar i;
    find_left=0;          
    find_right=0;
    for(i=0;i<5;i++) 
    {
    	if((v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][i])>30&&v_point_down[Pre_Line_Save][i]-v_point_up[Pre_Line_Save][i]<100&&v_point_down[Pre_Line_Save][i]!=0&&v_point_up[Pre_Line_Save][i]!=0&&v_point_down[Pre_Line_Save][i]<LineMax&&v_point_up[Pre_Line_Save][i]>LineMin) 
    	{
			 right_block0=v_point_down[Pre_Line_Save][i];
			 left_block0=v_point_up[Pre_Line_Save][i];
			 find_left=1;
             find_right=1;
			 break;
    	}
    }
}
void to_save_this_hang_state() 
{
	 find_last_left=find_left;
	 find_last_right=find_right;
}
void bvbo_when_no_real_start()
{
   find_left=0;
   find_right=0;
   if(Pre_Line_Save<20&&Pre_Line_Save>1) {	is_shizi(); }
   if(flag_shizi) 
   {
   		 if(!flag_find_aftershizi)
   		 {
   		 	getline_when_shizi();
   		 }
   		 if(flag_find_aftershizi)
   		 {						    
   		 	  get_path_left();
   		 	  get_path_right();
   		 	  if(find_left)
   		      {
                point_up[Pre_Line_Save]=left_block0;
                p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
                left_endline=Pre_Line_Save;
   		      }
   		      if(find_right)
   		      {
           		point_down[Pre_Line_Save]=right_block0;
                p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
                right_endline=Pre_Line_Save;
   		      }
   		 }
   		 if(find_left&&find_right&&!flag_find_aftershizi)
   		 {
           		point_down[Pre_Line_Save]=right_block0;
                point_up[Pre_Line_Save]=left_block0;
                p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
                p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
                left_endline=Pre_Line_Save;
                right_endline=Pre_Line_Save;
		        flag_find_aftershizi=1;
   		 }
   }
   else 
   {
       if(!Flag_Lose_Line)
       {
           if(!find_left_startline&&!find_right_startline)
           {
           		to_find_both_startline();
           		if(find_left_startline&&find_right_startline) 
           		{
           			_asm(nop);
           		} 
           		else if(!find_left_startline&&!find_right_startline)//两条起始线都未找到，此时可能会是一种特殊情况
           	    {
           			if (v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center) 
           			{
           				  find_left_startline=1;
           				  left_stratline=Pre_Line_Save;
           				  find_left=1;
           				  left_block0=v_point_up[Pre_Line_Save][0];
           			}
           			else if(v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center)
           			{
           				 find_right_startline=1;
           				 right_startline=Pre_Line_Save;
           				 find_right=1;
           				 right_block0=v_point_down[Pre_Line_Save][0];
           			}
           	   }
           	   if(find_left_startline) 
           	   {
           	   		if((0==find_last_lstl&&left_block0>Line_Center)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
           	   		{
           	   			  find_left_startline=0;
           	   			  find_left=0;
           	   		}
           	   }
           	   if(find_right_startline)
           	   {
           	   		if((0==find_last_rstl&&right_block0<Line_Center)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
           	   		{
           	   			 find_right_startline=0;
           	   			 find_right=0;
           	   		}
           	   }
           	   if(find_left_startline&&find_right_startline) 
           	   {
               	   if(right_block0<left_block0||right_block0<left_block0+40)
               	  {
           				 find_left_startline=0;
           				 find_right_startline=0;
           				 find_right=0;
           				 find_left=0; 
               	  }
           	   }
           }
           else if(find_left_startline&&!find_right_startline) 
           {
           		 if(!stop_find_right){	to_find_right_startline();}
           		 if(!stop_find_left) {      get_path_left();      }
           		 if(find_right_startline)
           		 {
           		 		if(right_block0<point_up[left_stratline]+30) 
           		 		{
           		 			find_right_startline=0;
           		 			find_right=0;	
           		 		}
           		 }
           		 if(find_right_startline)
           	    {
           	   		if((0==find_last_rstl&&right_block0<Line_Center+20)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
           	   		{
           	   			 find_right_startline=0;
           	   			 find_right=0;
           	   		}
           	    }
           } 
           else if(!find_left_startline&&find_right_startline) 
           {
           		 if(!stop_find_left) { to_find_left_startline();}
           		 if(!stop_find_right){	  get_path_right();     }
           		 if(find_left_startline) 
           		 {
           		 		if(left_block0>point_down[right_startline]-30)
           		 		{
           		 			find_left_startline=0;
           		 			find_left=0;	
           		 		}
           		 }
           		 if(find_left_startline) 
           	     {
           	   		if((0==find_last_lstl&&left_block0>Line_Center-20)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
           	   		{
           	   			  find_left_startline=0;
           	   			  find_left=0;
           	   		}
           	    }
           }
           else
           {
           		 if(!stop_find_right){	get_path_right(); }
           		 if(!stop_find_left) {  get_path_left();  }
           }
       }
       if(find_right&&find_left) 
       {
            point_down[Pre_Line_Save]=right_block0;
            point_up[Pre_Line_Save]=left_block0;
            p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
            p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
            left_endline=Pre_Line_Save;
            right_endline=Pre_Line_Save;
            num_wrong_left=0;
            num_wrong_right=0;
       }
       else if(!find_right&&find_left) 
       {
       		point_up[Pre_Line_Save]=left_block0;
            p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
       		left_endline=Pre_Line_Save;
       		if(find_right_startline){point_down[Pre_Line_Save]=point_down[Pre_Line_Save-1];p_point_down[Pre_Line_Save]=(char)(point_down[Pre_Line_Save-1]-Line_Center)*jibian+Line_Center;}
       		num_wrong_left=0;
       		if(find_right_startline||Pre_Line_Save>10) { num_wrong_right++;}
       		if(num_wrong_right>3){ stop_find_right=1;}
       }
       else if(find_right&&!find_left) 
       {
       		 point_down[Pre_Line_Save]=right_block0;
             p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
       		 right_endline=Pre_Line_Save;
       		 if(find_left_startline){point_up[Pre_Line_Save]=point_up[Pre_Line_Save-1];p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-point_up[Pre_Line_Save-1])*jibian;}
       		 num_wrong_right=0;
       		 if(find_left_startline||Pre_Line_Save>10){	num_wrong_left++; }
       		 if(num_wrong_left>3){ stop_find_left=1; }
       }
       else 
       {
       		if(find_right_startline){point_down[Pre_Line_Save]=point_down[Pre_Line_Save-1];p_point_down[Pre_Line_Save]=(char)(point_down[Pre_Line_Save-1]-Line_Center)*jibian+Line_Center;}
       		if(find_left_startline){point_up[Pre_Line_Save]=point_up[Pre_Line_Save-1];p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-point_up[Pre_Line_Save-1])*jibian;}
       		if(find_right_startline||Pre_Line_Save>10) { num_wrong_right++;}
       		if(num_wrong_right>3){ stop_find_right=1;}
       		if(find_left_startline||Pre_Line_Save>10){	num_wrong_left++; }
       		if(num_wrong_left>3){ stop_find_left=1; }
       		if(stop_find_left&&stop_find_right){  Flag_Lose_Line=1;	 }
       }
   }

}
void lvbo() 
{
   
   if(!Flag_Lose_Line)
   {
       if(!find_left_startline&&!find_right_startline)
       {
       		to_find_both_startline_new();
       		if(pfind_left_startline&&pfind_right_startline) 
       		{
       			_asm(nop);
       		} 
       		else if(!pfind_left_startline&&!pfind_right_startline)//两条起始线都未找到，此时可能会是一种特殊情况 待改进  
       	    {
       	       if(PWMDTY67<Steer_Mid)
       	       {
	       			if (v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20) 
	       			{
	       				  find_left_startline=1;
	       				  left_stratline=Pre_Line_Save;
	       				  find_left=1;
	       				  left_block0=v_point_up[Pre_Line_Save][0];
	       			}
	       			else if(v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20)
	       			{
	       				 find_right_startline=1;
	       				 right_startline=Pre_Line_Save;
	       				 find_right=1;
	       				 right_block0=v_point_down[Pre_Line_Save][0];
	       			}
       	       }
       	       else
       	       {
       	       		if (v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20) 
	       			{
	       				 find_right_startline=1;
	       				 right_startline=Pre_Line_Save;
	       				 find_right=1;
	       				 right_block0=v_point_down[Pre_Line_Save][0];
	       			}
	       			else if(v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20)
	       			{
	       				  find_left_startline=1;
	       				  left_stratline=Pre_Line_Save;
	       				  find_left=1;
	       				  left_block0=v_point_up[Pre_Line_Save][0];
	       			}	
       	       }
       	   }
       	   if(pfind_left_startline) 
       	   {
       	   		if((0==find_last_lstl&&left_block0>Line_Center)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
       	   		{
       	   			  find_left_startline=0;
       	   			  find_left=0;
       	   		}
       	   }
       	   if(pfind_right_startline)
       	   {
       	   		if((0==find_last_rstl&&right_block0<Line_Center)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
       	   		{
       	   			 find_right_startline=0;
       	   			 find_right=0;
       	   		}
       	   }
       	   if(pfind_left_startline&&pfind_right_startline) 
       	   {
           	   if(right_block0<left_block0+40)//宽度限制
           	  {
       				 find_left_startline=0;
       				 find_right_startline=0;
       				 find_right=0;
       				 find_left=0; 
           	  }
       	   }
       }
       else if(find_left_startline&&!find_right_startline) 
       {
       		 if(!is_find_a_real_start)
       		 {
       		 	to_find_both_startline_new();
       		 	if(pfind_left_startline&&pfind_right_startline) 
	       		{
	       			_asm(nop);
	       		} 
	       		else if(!pfind_left_startline&&!pfind_right_startline)//两条起始线都未找到，此时可能会是一种特殊情况
		       {
	       	       if(PWMDTY67<Steer_Mid)
	       	       {
		       			if (v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20) 
		       			{
		       				  find_left_startline=1;
		       				  left_stratline=Pre_Line_Save;
		       				  find_left=1;
		       				  left_block0=v_point_up[Pre_Line_Save][0];
		       			}
		       			else if(v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20)
		       			{
		       				 find_right_startline=1;
		       				 right_startline=Pre_Line_Save;
		       				 find_right=1;
		       				 right_block0=v_point_down[Pre_Line_Save][0];
		       			}
	       	       }
	       	       else
	       	       {
	       	       		if (v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20) 
		       			{
		       				 find_right_startline=1;
		       				 right_startline=Pre_Line_Save;
		       				 find_right=1;
		       				 right_block0=v_point_down[Pre_Line_Save][0];
		       			}
		       			else if(v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20)
		       			{
		       				  find_left_startline=1;
		       				  left_stratline=Pre_Line_Save;
		       				  find_left=1;
		       				  left_block0=v_point_up[Pre_Line_Save][0];
		       			}	
	       	       }
	       	   }
	       	   if(pfind_left_startline) 
	       	   {
	       	   		if((0==find_last_lstl&&left_block0>Line_Center)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
	       	   		{
	       	   			  find_left_startline=0;
	       	   			  find_left=0;
	       	   		}
	       	   }
	       	   if(pfind_right_startline)
	       	   {
	       	   		if((0==find_last_rstl&&right_block0<Line_Center)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
	       	   		{
	       	   			 find_right_startline=0;
	       	   			 find_right=0;
	       	   		}
	       	   }
	       	   if(pfind_left_startline&&pfind_right_startline) 
	       	   {
	           	  if(right_block0<left_block0+40)//宽度限制
	           	  {
	       				 find_left_startline=0;
	       				 find_right_startline=0;
	       				 find_right=0;
	       				 find_left=0; 
	           	  }
	       	   }
       		 } 
       		 else
       		 {
	       		 if(!stop_find_right){	to_find_right_startline();}
	       		 if(!stop_find_left) {      get_path_left();      }
	       		 if(find_right_startline)
	       		 {
	       		 		if(right_block0<point_up[left_stratline]+30)//宽度限制
	       		 		{
	       		 			find_right_startline=0;
	       		 			find_right=0;	
	       		 		}
	       		 }
	       		if(find_right_startline)
	       	    {
	       	   		if((0==find_last_rstl&&right_block0<Line_Center+20)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
	       	   		{
	       	   			 find_right_startline=0;
	       	   			 find_right=0;
	       	   		}
	       	    }
       		 }
       		 
       } 
       else if(!find_left_startline&&find_right_startline) 
       {
       		 if(!is_find_a_real_start)
       		 {
       		 	  to_find_both_startline_new();
       		 	  if(pfind_left_startline&&pfind_right_startline) 
		       		{
		       			_asm(nop);
		       		} 
		       		else if(!pfind_left_startline&&!pfind_right_startline)//两条起始线都未找到，此时可能会是一种特殊情况
			       	{
		       	       if(PWMDTY67<Steer_Mid)
		       	       {
			       			if (v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20) 
			       			{
			       				  find_left_startline=1;
			       				  left_stratline=Pre_Line_Save;
			       				  find_left=1;
			       				  left_block0=v_point_up[Pre_Line_Save][0];
			       			}
			       			else if(v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20)
			       			{
			       				 find_right_startline=1;
			       				 right_startline=Pre_Line_Save;
			       				 find_right=1;
			       				 right_block0=v_point_down[Pre_Line_Save][0];
			       			}
		       	       }
		       	       else
		       	       {
		       	       		if (v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20) 
			       			{
			       				 find_right_startline=1;
			       				 right_startline=Pre_Line_Save;
			       				 find_right=1;
			       				 right_block0=v_point_down[Pre_Line_Save][0];
			       			}
			       			else if(v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20)
			       			{
			       				  find_left_startline=1;
			       				  left_stratline=Pre_Line_Save;
			       				  find_left=1;
			       				  left_block0=v_point_up[Pre_Line_Save][0];
			       			}	
		       	       }
		       	   }
		       	   if(pfind_left_startline) 
		       	   {
		       	   		if(left_block0>point_down[right_startline]-30)//宽度限制
		       	   		{
		       	   			  find_left_startline=0;
		       	   			  find_left=0;
		       	   		}
		       	   }
		       	   if(pfind_right_startline)
		       	   {
		       	   		if((0==find_last_rstl&&right_block0<Line_Center)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40))//上一场的基准 
		       	   		{
		       	   			 find_right_startline=0;
		       	   			 find_right=0;
		       	   		}
		       	   }
		       	   if(pfind_left_startline&&pfind_right_startline)//宽度限制 
		       	   {
		           	  if(right_block0<left_block0+40)
		           	  {
		       				 find_left_startline=0;
		       				 find_right_startline=0;
		       				 find_right=0;
		       				 find_left=0; 
		           	  }
		       	   }
       		 } 
       		 else
       		 {
	       		 if(!stop_find_left) { to_find_left_startline();}
	       		 if(!stop_find_right){	  get_path_right();     }
	       		 if(find_left_startline) 
	       		 {
	       		 		if(left_block0>point_down[right_startline]-30)
	       		 		{
	       		 			find_left_startline=0;
	       		 			find_left=0;	
	       		 		}
	       		 }
	       		 if(find_left_startline) 
	       	     {
	       	   		if((0==find_last_lstl&&left_block0>Line_Center-20)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
	       	   		{
	       	   			  find_left_startline=0;
	       	   			  find_left=0;
	       	   		}
	       	    }
       		 }
       		 
       }
       else //两个跳变沿的起始线都找到了
       {
       		if(!is_find_a_real_start)
       		{
       		 	  to_find_both_startline_new();
       		 	  if(pfind_left_startline&&pfind_right_startline) 
		       		{
		       			_asm(nop);
		       		} 
		       		else if(!pfind_left_startline&&!pfind_right_startline)//两条起始线都未找到，此时可能会是一种特殊情况
		       	    {
		       	       if(PWMDTY67<Steer_Mid)
		       	       {
			       			if (v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20) 
			       			{
			       				  find_left_startline=1;
			       				  left_stratline=Pre_Line_Save;
			       				  find_left=1;
			       				  left_block0=v_point_up[Pre_Line_Save][0];
			       			}
			       			else if(v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20)
			       			{
			       				 find_right_startline=1;
			       				 right_startline=Pre_Line_Save;
			       				 find_right=1;
			       				 right_block0=v_point_down[Pre_Line_Save][0];
			       			}
		       	       }
		       	       else
		       	       {
		       	       		if (v_point_up[Pre_Line_Save][0]<(LineMin+2)&&v_point_down[Pre_Line_Save][0]<Line_Center&&v_point_down[Pre_Line_Save][0]>LineMin+20) 
			       			{
			       				 find_right_startline=1;
			       				 right_startline=Pre_Line_Save;
			       				 find_right=1;
			       				 right_block0=v_point_down[Pre_Line_Save][0];
			       			}
			       			else if(v_point_down[Pre_Line_Save][0]>(LineMax-2)&&v_point_up[Pre_Line_Save][0]>Line_Center&&v_point_up[Pre_Line_Save][0]<LineMax-20)
			       			{
			       				  find_left_startline=1;
			       				  left_stratline=Pre_Line_Save;
			       				  find_left=1;
			       				  left_block0=v_point_up[Pre_Line_Save][0];
			       			}	
		       	       }
		       	   }
		       	   if(pfind_left_startline) 
		       	   {
		       	   		if((0==find_last_lstl&&left_block0>Line_Center)||(1==find_last_lstl&&cabs_uchar(left_block0-pre_left_stl_error)>40))
		       	   		{
		       	   			  find_left_startline=0;
		       	   			  find_left=0;
		       	   		}
		       	   }
		       	   if(pfind_right_startline)
		       	   {
		       	   		if((0==find_last_rstl&&right_block0<Line_Center)||(1==find_last_rstl&&cabs_uchar(right_block0-pre_right_stl_error)>40)) 
		       	   		{
		       	   			 find_right_startline=0;
		       	   			 find_right=0;
		       	   		}
		       	   }
		       	   if(pfind_left_startline&&pfind_right_startline) 
		       	   {
		           	  if(right_block0<left_block0+40)
		           	  {
		       				 find_left_startline=0;
		       				 find_right_startline=0;
		       				 find_right=0;
		       				 find_left=0; 
		           	  }
		       	   }
       		}
       		else
       		{
	       		if(!stop_find_right&&!stop_find_left){	to_find_both_line_new(); }
	       		else
	       		{
	       			if(!stop_find_right)
	       			{
	       				get_path_right();
	       			}
	       		    if(!stop_find_left)
	       			{
	       				get_path_left();
	       			}	
	       		 }
       		}
       }
   }
   if(find_right&&find_left) 
   {
        point_down[Pre_Line_Save]=right_block0;
        point_up[Pre_Line_Save]=left_block0;
        p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
        p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
        left_endline=Pre_Line_Save;
        right_endline=Pre_Line_Save;
        num_wrong_left=0;
        num_wrong_right=0;
   }
   else if(!find_right&&find_left) 
   {
   		point_up[Pre_Line_Save]=left_block0;
   		p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
   		left_endline=Pre_Line_Save;
   		if(find_right_startline){point_down[Pre_Line_Save]=point_down[Pre_Line_Save-1];p_point_down[Pre_Line_Save]=(char)(point_down[Pre_Line_Save-1]-Line_Center)*jibian+Line_Center;}
   		num_wrong_left=0;
   		if(find_right_startline||Pre_Line_Save>10) { num_wrong_right++;}
   		if(num_wrong_right>3){ stop_find_right=1;}
   }
   else if(find_right&&!find_left) 
   {
   		 point_down[Pre_Line_Save]=right_block0;
   		 p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
   		 right_endline=Pre_Line_Save;
   		 if(find_left_startline){point_up[Pre_Line_Save]=point_up[Pre_Line_Save-1];p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-point_up[Pre_Line_Save-1])*jibian;}
   		 num_wrong_right=0;
   		 if(find_left_startline||Pre_Line_Save>10){	num_wrong_left++; }
   		 if(num_wrong_left>3){ stop_find_left=1; }
   }
   else 
   {
   		if(find_right_startline){point_down[Pre_Line_Save]=point_down[Pre_Line_Save-1];p_point_down[Pre_Line_Save]=(char)(point_down[Pre_Line_Save-1]-Line_Center)*jibian+Line_Center;}
   		if(find_left_startline){point_up[Pre_Line_Save]=point_up[Pre_Line_Save-1];p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-point_up[Pre_Line_Save-1])*jibian;}
   		if(find_right_startline||Pre_Line_Save>10) { num_wrong_right++;}
   		if(num_wrong_right>3){ stop_find_right=1;}
   		if(find_left_startline||Pre_Line_Save>10){	num_wrong_left++; }
   		if(num_wrong_left>3){ stop_find_left=1; }
   		if(stop_find_left&&stop_find_right){  Flag_Lose_Line=1;	 }
   }
   to_save_this_hang_state(); 
   if(!is_find_a_real_start)
   {
   		if(find_last_left&&find_left&&cabs_uchar(left_block0-point_up[Pre_Line_Save-1])<10) 
   		{
   			 is_find_a_real_start=1;
   			 test_aa_lstl= Pre_Line_Save-1;
   			 left_stratline=Pre_Line_Save-1;
   		}
   		if(find_last_right&&find_right&&cabs_uchar(right_block0-point_down[Pre_Line_Save-1])<10) 
   		{
   			 is_find_a_real_start=1;
   			 right_startline=Pre_Line_Save-1;
   			 test_aa_rstl=Pre_Line_Save-1;
   		}
   		if(Pre_Line_Save>5) 
   		{
   		    clear_this_field_flag();
   		    test_bbhh=Pre_Line_Save;
   			not_find_a_real_start=1;
   		}
   }
   return;
}
void car_control_after_a_field()
{
  	  Pre_Line_Save=0;
	  Line_Save=0;
	  if(is_startline2==0)
	  {
		  if(is_startline1&&(left_endline>45||right_endline>45))
		  {
		     is_startline2=1;
		  }
	  }
	  else
	  {
	  	   is_startline2++;
	  	   if(is_startline2>5)
	  	   {
	  	   		is_startline=1;
	  	   }
	  }
	  if(is_startline) 
	  {		 
	       is_startline3++;
           //if(is_startline3<20)
           if(0)
           {
           		 PWME_PWME3=0;
           		 PWMDTY01=500;
           }
           else
           {    
                is_startline3=21;
             	PWME_PWME3=0;
             	PWME_PWME1=0;	
           }
      }
	  if(find_left_startline&&!find_right_startline)
	  {
	 		turn_right=1;
	  }
	  if(!find_left_startline&&find_right_startline)
	  {
	 		turn_left=1;
	   }
	  to_save_thisfield_state(); 
	  pnew_duoji_control();
      enstart_duoji();
	  if(will_be_out_right||will_be_out_left)
	  {
	  		if(will_be_out_right)
	  		{
            }
	  		else
	  		{
	  		}
	  }
	  pre_SteerAngle=PWMDTY67;
	  if(pre_SteerAngle<Steer_Mid+100&&pre_SteerAngle>Steer_Mid-100)
       stright++;
       else stright=0;
       if(stright>20) 
       {
          stright_flag=1;
       } 
	  set_speed();
}
void how_to_do_when_shizi()
{
	 if(!flag_find_aftershizi)
	 {
	 	getline_when_shizi();
	 }
	 if(flag_find_aftershizi)
	 {
	 	  get_path_left();
	 	  get_path_right();
	 	  if(find_left)
	     {
            point_up[Pre_Line_Save]=left_block0;
            p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
            left_endline=Pre_Line_Save;
	      }
	      if(find_right)
	      {
       		point_down[Pre_Line_Save]=right_block0;
            p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
            right_endline=Pre_Line_Save;
	      }
	 }
	 if(find_left&&find_right&&!flag_find_aftershizi)
	 {
       		point_down[Pre_Line_Save]=right_block0;
            point_up[Pre_Line_Save]=left_block0;
            p_point_up[Pre_Line_Save]=Line_Center-(char)(Line_Center-left_block0)*jibian;
            p_point_down[Pre_Line_Save]=(char)(right_block0-Line_Center)*jibian+Line_Center;
            left_endline=Pre_Line_Save;
            right_endline=Pre_Line_Save;
            num_find++;
            if(num_find>3) 
            {
	            flag_find_aftershizi=1;
	            find_left_startline=1;
	            find_right_startline=1;
            }
	 }
	 else
	 {
	 	  num_find=0;
	 }
}
void main()
{
    DisableInterrupts;
    init_all();//初始化所有模块
    PWMDTY67=Steer_Mid;
    Pre_Line_Save=0;
    num_chang=0;       //场数
    PWME_PWME3=0;
    Line_Save=0;
    select_speed();
    pre_e0=1;
    send=0;
    TIE_C0I=1;//场中断
    PORTB=0xff;
    EnableInterrupts;
    while(1) 
    {	  
          /*  if(0)
            {
                nRF24L01Init();
                IRQCR_IRQEN=1;
               while(1)
               { 
                
                 while(PT1AD0_PT1AD03==1);
                 SCI0Tx1Fld1();
                 while(PT1AD0_PT1AD03==0);
               
               }
            }  */
           
          while(Pre_Line_Save==Line_Save)//当这两个变量不相等时，说明已经采集了一行的跳边沿，此时进行一行的处理
          {
          	  //to_start_or_stop(); //按键控制函数
          }
          {
          	   uchar     i;
               find_left=0;
               find_right=0;
               if(Pre_Line_Save>3&&Pre_Line_Save<20&&is_use_start&&num_chang>1000)
               {
                is_the_startline();
               }
               if(Pre_Line_Save<35&&Pre_Line_Save>7) 
               {	
                 is_shizi(); 
                 if(flag_shizi&&flag_ed==0)
                  flag_ed=Pre_Line_Save+1;
               }
               if(flag_shizi) 
               {
               	  how_to_do_when_shizi();	 
               }
               else 
               {
               		if(not_find_a_real_start)
               		{
               		bvbo_when_no_real_start();
               		} 
               		else{lvbo();}
               }
          }
          if(stop_find_left&&stop_find_right)
          { 
             Flag_Lose_Line=1;
          }
          Pre_Line_Save++;//正常情况下此时的两个变量的值相等
          if((ROW_MAX)==Pre_Line_Save)//共采集了50行，故最后一行的行序号为49，由于前面对行序号进行了加一操作，故此时应判断是否等于50 
          {
              if(find_right_startline&&find_left_startline)//对弯道的特殊情况进行处理
              { 
                uchar  aa,bb;
                aa=left_endline-left_stratline;
                bb=right_endline-right_startline;
              	if(((right_startline>2&&bb<6)||(right_startline>0&&bb<2&&aa>bb+10)))
              	{
              	   find_right_startline=0;
              	}
              	if((left_stratline>2&&aa<6)||(right_startline>0&&aa<2&&bb>aa+10))
              	{
              	   find_left_startline=0;
              	}
              }
          	  car_control_after_a_field();
          }
    }
}
#pragma CODE_SEG NON_BANKED 
void  interrupt 8 field_interrupt(void)        //场中断函数  上升沿出触发
{
  TFLG1_C0F = 1;//清除场标志
  numb_cmos=0;
  TIE_C2I=1;    //场中断中打开行中断
  {
     v_point_up[0][0]=0;//场中断的开始清零保存第一行跳变沿的数组
     v_point_down[0][0]=0;//由于场中断后有可能很快进入行中断，故在场
     v_point_up[0][1]=0;//中断中加入这么多的代码有不妥之处，有待于进一步改进
     v_point_down[0][1]=0;//
     v_point_up[0][2]=0;  //
     v_point_down[0][2]=0;//
     v_point_up[0][3]=0;  //
     v_point_down[0][3]=0;//
     v_point_up[0][4]=0;  //
     v_point_down[0][4]=0;//
     point_up[0]=0;		  //
     point_down[0]=0;	  //
  }
}
#pragma CODE_SEG DEFAULT
#pragma CODE_SEG NON_BANKED
void interrupt 9 odd_even_interrupt(void) 
{
  TFLG1_C1F=1;  //清奇偶场中断标志
}
#pragma CODE_SEG DEFAULT
#pragma CODE_SEG NON_BANKED 
void  interrupt 10 row_interrupt(void)        //行中断函数  上升沿出触发
{
  uchar     i;
  TFLG1_C2F = 1;//清除行标志
  PITCFLMT_PFLMT1=1;//初始化计数器值
  PITFLT_PFLT1=1;//初始化计数器值
  TIE &= ~(TIE_C3I_MASK|TIE_C4I_MASK);//行中断的开始先关闭上升沿和下降沿中断
  Flag_Up=0;
  Flag_Down=0;
  Line_CCD++;
  if(Line_CCD==cmos[numb_cmos]) 
  {
      numb_cmos++;
      if(First_row==Line_CCD) 
      {
         //一场的开始，清除关于判断赛道特性的标志
         Line_Save=0;
         clear_this_field_flag();
      }
      else
      {
         Line_Save++; 
      }
      TFLG1=(1<<3)|(1<<4);//清除三号和四号中断
      TIE |= (TIE_C3I_MASK|TIE_C4I_MASK);//若该行为需要采集的行，则打开上升沿和下降沿中断
      for(i=0;i<5;i++) 
      {
          if(ROW_MAX-1!=Line_Save)        // ROW_MAX=50
          {
             v_point_up[Line_Save+1][i]=0;   //Line_Save此变量为采集的行数0 1 2 . . . . . 49
             v_point_down[Line_Save+1][i]=0;  //uchar     v_point_up[60][5]
          }
      }
      point_up[Line_Save+1]=0;
      point_down[Line_Save+1]=0;
      p_point_up[Pre_Line_Save+1]=0;
      p_point_down[Pre_Line_Save+1]=0;

   }
      if(Last_row==Line_CCD)
      {
      	     Line_Save=ROW_MAX;
             TIE &= ~(TIE_C2I_MASK);//一场中采集到足够的行之后关闭行中断
             Line_CCD=0;
      }
}
#pragma CODE_SEG DEFAULT
#pragma CODE_SEG NON_BANKED 
void  interrupt 11 rising_interrupt(void)        //上升沿中断函数  
{
  TFLG1_C3F = 1;//清除上升沿中断标志
  PITCNT1temp=PITCNT1;//记录该时刻计数器的值
  if(Flag_Down-Flag_Up==1&&Flag_Up<=4) 
  {
      v_point_up[Line_Save][Flag_Up]=PITCNT1temp;
      Flag_Up++;
      /*if((v_point_down[Line_Save][Flag_Down-1]-v_point_up[Line_Save][Flag_Up-1])<7)
      {
      		v_point_down[Line_Save][Flag_Down-1]=0;
      		v_point_up[Line_Save][Flag_Up]=0;
      		Flag_Down--;
      		Flag_Up--;	
      }*/
  }
}
#pragma CODE_SEG DEFAULT
#pragma CODE_SEG NON_BANKED 
void  interrupt 12 falling_interrupt(void)        //下降沿中断函数  
{
  TFLG1_C4F = 1;//清除下降沿中断标志
  PITCNT1temp=PITCNT1;//记录该时刻计数器的值
  if(Flag_Down-Flag_Up==0&&Flag_Down<=4) 
  {
      v_point_down[Line_Save][Flag_Down]=PITCNT1temp;
      Flag_Down++;
      /*if(Flag_Down!=1&&(v_point_up[Line_Save][Flag_Up-1]-v_point_down[Line_Save][Flag_Down-1])<7)
      {
      	 v_point_up[Line_Save][Flag_Up-1]=0;
      	 v_point_down[Line_Save][Flag_Down-1]=0;
      	 Flag_Up--;
      	 Flag_Down--;		
      }*/
  }
}
#pragma CODE_SEG DEFAULT
#pragma CODE_SEG NON_BANKED 
void  interrupt 66 testt(void)      
{
  EnableInterrupts;  //打开中断屏蔽以允许高级中断
  PITTF |= 0x01;     //清楚计时器中断标志
  speed_ctl();
}
#pragma CODE_SEG DEFAULT

