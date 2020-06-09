#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include"MySD.h"
#define SPInCS_0()  PTM_PTM3 = 0
#define SPInCS_1() 	PTM_PTM3 = 1   

/******************************************************************
 - 功能描述：向SD卡写命令
 - 隶属模块：SD卡模块
 - 函数属性：内部
 - 参数说明：SD卡的命令是6个字节，pcmd是指向命令字节序列的指针
 - 返回说明：命令写入后，SD卡的回应值，调用不成功，将返回0xff
 ******************************************************************/

unsigned char SD_Write_Cmd(unsigned char *pcmd) //向SD卡写命令，pcmd是命令字节序列的首地址
{
	unsigned char temp,time=0;
	
	SPInCS_1();
	SPIWriteByte(0xff); //提高兼容性，如果没有这里，有些SD卡可能不支持   
	SPInCS_0();
	
	SPIWriteByte(pcmd[0]);
	SPIWriteByte(pcmd[1]);
	SPIWriteByte(pcmd[2]);
	SPIWriteByte(pcmd[3]);
	SPIWriteByte(pcmd[4]);
	SPIWriteByte(pcmd[5]);	
	do 
	{  
	temp = SPIReadByte();//一直读，直到读到的不是0xff或超时
	time++;
	}while((temp==0xff)&&(time<TRY_TIME)); 
	
	return(temp);
}

/******************************************************************
 - 功能描述：复位SD卡，用到CMD0，使用SD卡切换到SPI模式
 - 隶属模块：SD卡模块
 - 函数属性：外部，供用户调用
 - 参数说明：无
 - 返回说明：调用成功，返回0x00，否则返回INIT_CMD0_ERROR (sd.h中有定义)
 ******************************************************************/

unsigned char SD_Reset(void)//SD卡复位，进入SPI模式，使用CMD0（命令0）
{
	unsigned char time,temp,i;
	unsigned char pcmd[] = {0x40,0x00,0x00,0x00,0x00,0x95}; //命令0的字节序列

	//S0SPCCR = 255; //SD卡初始化速度不能超过400kbps  此处为300kbps

	SPInCS_1();  //关闭片选 

	for(i=0;i<0x0f;i++) //复位时，首先要发送最少74个时钟信号，这是必须的！！！
	{
		SPIWriteByte(0xff); //120个时钟
	}

	SPInCS_0(); //打开片选

	time=0;
	do
	{ 
		temp=SD_Write_Cmd(pcmd);//写入CMD0
		time++;
	if(time==TRY_TIME) 
	{ 
		SPInCS_1(); //关闭片选
	PORTB = 0xF0;
		//UARTSendString(UART1_BASE,"CMD0写入失败SD卡复位失败\r\n");
		return(INIT_CMD0_ERROR);//CMD0写入失败
	}
	}while(temp!=0x01);	
	//UARTSendString(UART1_BASE,"CMD0写入成功SD卡复位成功\r\n");
	SPInCS_1(); //关闭片选
		
	SPIWriteByte(0xff); //按照SD卡的操作时序在这里补8个时钟 
	return 0;//返回0,说明复位操作成功
}

/******************************************************************
 - 功能描述：初始化SD卡，使用CMD1
 - 隶属模块：SD卡模块
 - 函数属性：外部，供用户调用
 - 参数说明：无
 - 返回说明：调用成功，返回0x00，否则返回INIT_CMD1_ERROR (sd.h中有定义)
 ******************************************************************/

unsigned char SD_Init(void)	//初始化，使用CMD1（命令1）
{  
	unsigned char time,temp;
	unsigned char pcmd[] = {0x41,0x00,0x00,0x00,0x00,0xff}; //命令1的字节序列

   	//S0SPCCR = 255; //SD卡初始化速度不能超过400kbps  此处为300kbps

	SPInCS_0(); //打开片选

	time=0;
	do
	{ 
		temp=SD_Write_Cmd(pcmd);
		time++;
	if(time==TRY_TIME) 
	{ 
		SPInCS_1(); //关闭片选
		PORTB = 0x55;
		//UARTSendString(UART1_BASE,"CMD1写入失败SD卡初始化失败\r\n");
		return(INIT_CMD1_ERROR);//CMD1写入失败
	}
	}while(temp!=0);
   	//UARTSendString(UART1_BASE,"CMD1写入成功SD卡初始化成功\r\n");
	SPInCS_1();  //关闭片选 

	SPIWriteByte(0xff); //按照SD卡的操作时序在这里补8个时钟

  SPI0BR  = 0x01; //BR=busclk/((SPPR + 1)· 2^(SPR + 1))   SPI:20M
	return(0); //返回0,说明初始化操作成功
} 

/****************************************************************************
 - 功能描述：将buffer指向的512个字节的数据写入到SD卡的addr扇区中
 - 隶属模块：SD卡模块
 - 函数属性：外部，供用户调用
 - 参数说明：addr:扇区地址
             buffer:指向数据缓冲区的指针
 - 返回说明：调用成功，返回0x00，否则返回WRITE_BLOCK_ERROR (sd.h中有定义)
 - 注：SD卡初始化成功后，读写扇区时，尽量将SPI速度提上来，提高效率
 ****************************************************************************/

unsigned char SD_Write_Sector(unsigned long addr,unsigned char *buffer)	//向SD卡中的指定地址的扇区写入512个字节，使用CMD24（命令24）
{  
 unsigned char temp,time;
 unsigned int i;
 unsigned char pcmd[] = {0x58,0x00,0x00,0x00,0x00,0xff}; //向SD卡中单个块（512字节，一个扇区）写入数据，用CMD24

 addr<<=9; //addr = addr * 512	将块地址（扇区地址）转为字节地址 ［这里就限制了SD卡的最大容量为4G］
	
 pcmd[1]=((addr&0xff000000)>>24); //将字节地址写入到CMD24字节序列中
 pcmd[2]=((addr&0x00ff0000)>>16);
 pcmd[3]=((addr&0x0000ff00)>>8);

 SPInCS_0(); //打开SD卡片选

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd);
  time++;
  if(time==TRY_TIME) 
  { 
   SPInCS_1();  //关闭片选
   return(temp); //命令写入失败
  }
 }while(temp!=0); 

 for(i=0;i<100;i++) //这里要插入若干时钟信号
 {
  SPIWriteByte(0xff);
 }
	
 SPIWriteByte(0xfe);//写入开始字节 0xfe，后面就是要写入的512个字节的数据	
	
 for(i=0;i<512;i++) //将缓冲区中要写入的512个字节写入SD卡
 {
  SPIWriteByte(0xf3);
 }
  
 SPIWriteByte(0xff); 
 SPIWriteByte(0xff); //两个字节的CRC校验码，不用关心
   
    
 temp=SPIReadByte();   //读取返回值
 if((temp&0x1F)!=0x05) //如果返回值是 XXX00101说明数据已经被SD卡接受了
 {
  SPInCS_1(); 
  return(WRITE_BLOCK_ERROR); //写块数据失败
 }
 
 while(SPIReadByte()!=0xff);//等到SD卡不忙（数据被接受以后，SD卡要将这些数据写入到自身的FLASH中，需要一个时间）
						     //忙时，读回来的值为0x00,不忙时，为0xff

 SPInCS_1();  //关闭片选

 SPIWriteByte(0xff);//按照SD卡的操作时序在这里补8个时钟
 return(0);		 //返回0,说明写扇区操作成功
} 

/****************************************************************************
 - 功能描述：读取addr扇区的512个字节到buffer指向的数据缓冲区
 - 隶属模块：SD卡模块
 - 函数属性：外部，供用户调用
 - 参数说明：addr:扇区地址
             buffer:指向数据缓冲区的指针
 - 返回说明：调用成功，返回0x00，否则返回READ_BLOCK_ERROR (sd.h中有定义)
 - 注：SD卡初始化成功后，读写扇区时，尽量将SPI速度提上来，提高效率
 ****************************************************************************/

unsigned char SD_Read_Sector(unsigned long addr,unsigned char *buffer)//从SD卡的指定扇区中读出512个字节，使用CMD17（17号命令）
{
 unsigned int j;
 unsigned char time,temp;
 unsigned char pcmd[]={0x51,0x00,0x00,0x00,0x00,0xff}; //CMD17的字节序列
   
 addr<<=9; //addr=addr*512	   将块地址（扇区地址）转为字节地址

 pcmd[1]=((addr&0xff000000)>>24);//将字节地址写入到CMD17字节序列中
 pcmd[2]=((addr&0x00FF0000)>>16);
 pcmd[3]=((addr&0x0000FF00)>>8);

 SPInCS_0(); //打开片选 

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd); //写入CMD17
  time++;
  if(time==TRY_TIME) 
  {
   return(READ_BLOCK_ERROR); //读块失败
  }
 }while(temp!=0); 
   			
 while (SPIReadByte()!= 0xfe); //一直读，当读到0xfe时，说明后面的是512字节的数据了

 for(j=0;j<512;j++)	 //将数据写入到数据缓冲区中
 {	
  buffer[j]=SPIReadByte();
 }

 SPIReadByte();
 SPIReadByte();//读取两个字节的CRC校验码，不用关心它们

 SPInCS_1();   //SD卡关闭片选 

 SPIWriteByte(0xff);//按照SD卡的操作时序在这里补8个时钟

 return 0;
}

void TFCardWriteNum(ulong Sector,uint *Num,uint Cont) 
{
  uint i,j,k; 
  {
    SD_Write_Sector(Sector,Num);
    Sector++;
    Num+=512;  
  }
}
uchar SPIWriteByte(uchar value)
{
  while(!SPI0SR_SPTEF); //等待数据存储器清空
  SPI0DR = value;
  while(!SPI0SR_SPIF);
  return SPI0DR;   
}

/************************************************************************************
Func Name:    SPI_Read_Byte                                
FuncDescrip:  SPI读一个字节
************************************************************************************/
uchar SPIReadByte()
{  
  return  SPIWriteByte(0xFF);
}
/************************************************************************************
Func Name:    SPI_Init                                
FuncDescrip:  SPI初始化
************************************************************************************/
void SPIInit(void)
{
  MODRR   = 0x10; //设置SPI的引脚为PM口  PM2:MISO  PM3:SS   PM4:MOSI    PM5:SCK
  SPI0CR2 = 0x00; //SS为自动硬件片选，SPI 4Wire模式
  SPI0CR1 = 0x50; //时钟电平空闲为低，关闭中断，MSB传输  
  SPI0BR  = 0x66; //BR=busclk/((SPPR + 1)· 2^(SPR + 1))   SPI:5M
  DDRM_DDRM3 = 1;  //片选信号输出              
}
unsigned char SD_Write_Image(unsigned long addr)	//向SD卡中的指定地址的扇区写入512个字节，使用CMD24（命令24）
{  
 unsigned char temp,time;
 unsigned int i,j;
 unsigned char pcmd[] = {0x58,0x00,0x00,0x00,0x00,0xff}; //向SD卡中单个块（512字节，一个扇区）写入数据，用CMD24

 //addr<<=9; //addr = addr * 512	将块地址（扇区地址）转为字节地址 ［这里就限制了SD卡的最大容量为4G］
 addr=addr*2500;
 pcmd[1]=((addr&0xff000000)>>24); //将字节地址写入到CMD24字节序列中
 pcmd[2]=((addr&0x00ff0000)>>16);
 pcmd[3]=((addr&0x0000ff00)>>8);

 SPInCS_0(); //打开SD卡片选

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd);
  time++;
  if(time==TRY_TIME) 
  { 
   SPInCS_1();  //关闭片选
   return(temp); //命令写入失败
  }
 }while(temp!=0); 

 for(i=0;i<100;i++) //这里要插入若干时钟信号
 {
  SPIWriteByte(0xff);
 }
	
 SPIWriteByte(0xfe);//写入开始字节 0xfe，后面就是要写入的512个字节的数据	
	
 
 for(i=0;i<30;i++) 
 {
    for(j=0;j<70;j++) 
    {
       SPIWriteByte(0x00);
    }
    
    SPIWriteByte(0x20);////////写完一行空格
    SPIWriteByte(0x20);////////写完一行空格
    SPIWriteByte(0x20);////////写完一行空格
    SPIWriteByte(0x20);////////写完一行空格
    SPIWriteByte(0x20);
    SPIWriteByte(0x20);
    SPIWriteByte(0x0d);////////写完一行回车
 }
 SPIWriteByte(0x0d);////////写完一场回车
 SPIWriteByte(0x0d);////////写完一场回车
 SPIWriteByte(0x0d);////////写完一场回车
 SPIWriteByte(0x0d);////////写完一场回车
  
 
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x20);
 SPIWriteByte(0x0d);////////写完回车
 
  
  
 
             
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 SPIWriteByte(0xff); 
 SPIWriteByte(0xff); //两个字节的CRC校验码，不用关心
   
    
 temp=SPIReadByte();   //读取返回值
 if((temp&0x1F)!=0x05) //如果返回值是 XXX00101说明数据已经被SD卡接受了
 {
  SPInCS_1(); 
  return(WRITE_BLOCK_ERROR); //写块数据失败
 }
 
 while(SPIReadByte()!=0xff);//等到SD卡不忙（数据被接受以后，SD卡要将这些数据写入到自身的FLASH中，需要一个时间）
						     //忙时，读回来的值为0x00,不忙时，为0xff

 SPInCS_1();  //关闭片选

 SPIWriteByte(0xff);//按照SD卡的操作时序在这里补8个时钟
 return(0);		 //返回0,说明写扇区操作成功 
}
