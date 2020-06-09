#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include"MySD.h"
#define SPInCS_0()  PTM_PTM3 = 0
#define SPInCS_1() 	PTM_PTM3 = 1   

/******************************************************************
 - ������������SD��д����
 - ����ģ�飺SD��ģ��
 - �������ԣ��ڲ�
 - ����˵����SD����������6���ֽڣ�pcmd��ָ�������ֽ����е�ָ��
 - ����˵��������д���SD���Ļ�Ӧֵ�����ò��ɹ���������0xff
 ******************************************************************/

unsigned char SD_Write_Cmd(unsigned char *pcmd) //��SD��д���pcmd�������ֽ����е��׵�ַ
{
	unsigned char temp,time=0;
	
	SPInCS_1();
	SPIWriteByte(0xff); //��߼����ԣ����û�������ЩSD�����ܲ�֧��   
	SPInCS_0();
	
	SPIWriteByte(pcmd[0]);
	SPIWriteByte(pcmd[1]);
	SPIWriteByte(pcmd[2]);
	SPIWriteByte(pcmd[3]);
	SPIWriteByte(pcmd[4]);
	SPIWriteByte(pcmd[5]);	
	do 
	{  
	temp = SPIReadByte();//һֱ����ֱ�������Ĳ���0xff��ʱ
	time++;
	}while((temp==0xff)&&(time<TRY_TIME)); 
	
	return(temp);
}

/******************************************************************
 - ������������λSD�����õ�CMD0��ʹ��SD���л���SPIģʽ
 - ����ģ�飺SD��ģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵������
 - ����˵�������óɹ�������0x00�����򷵻�INIT_CMD0_ERROR (sd.h���ж���)
 ******************************************************************/

unsigned char SD_Reset(void)//SD����λ������SPIģʽ��ʹ��CMD0������0��
{
	unsigned char time,temp,i;
	unsigned char pcmd[] = {0x40,0x00,0x00,0x00,0x00,0x95}; //����0���ֽ�����

	//S0SPCCR = 255; //SD����ʼ���ٶȲ��ܳ���400kbps  �˴�Ϊ300kbps

	SPInCS_1();  //�ر�Ƭѡ 

	for(i=0;i<0x0f;i++) //��λʱ������Ҫ��������74��ʱ���źţ����Ǳ���ģ�����
	{
		SPIWriteByte(0xff); //120��ʱ��
	}

	SPInCS_0(); //��Ƭѡ

	time=0;
	do
	{ 
		temp=SD_Write_Cmd(pcmd);//д��CMD0
		time++;
	if(time==TRY_TIME) 
	{ 
		SPInCS_1(); //�ر�Ƭѡ
	PORTB = 0xF0;
		//UARTSendString(UART1_BASE,"CMD0д��ʧ��SD����λʧ��\r\n");
		return(INIT_CMD0_ERROR);//CMD0д��ʧ��
	}
	}while(temp!=0x01);	
	//UARTSendString(UART1_BASE,"CMD0д��ɹ�SD����λ�ɹ�\r\n");
	SPInCS_1(); //�ر�Ƭѡ
		
	SPIWriteByte(0xff); //����SD���Ĳ���ʱ�������ﲹ8��ʱ�� 
	return 0;//����0,˵����λ�����ɹ�
}

/******************************************************************
 - ������������ʼ��SD����ʹ��CMD1
 - ����ģ�飺SD��ģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵������
 - ����˵�������óɹ�������0x00�����򷵻�INIT_CMD1_ERROR (sd.h���ж���)
 ******************************************************************/

unsigned char SD_Init(void)	//��ʼ����ʹ��CMD1������1��
{  
	unsigned char time,temp;
	unsigned char pcmd[] = {0x41,0x00,0x00,0x00,0x00,0xff}; //����1���ֽ�����

   	//S0SPCCR = 255; //SD����ʼ���ٶȲ��ܳ���400kbps  �˴�Ϊ300kbps

	SPInCS_0(); //��Ƭѡ

	time=0;
	do
	{ 
		temp=SD_Write_Cmd(pcmd);
		time++;
	if(time==TRY_TIME) 
	{ 
		SPInCS_1(); //�ر�Ƭѡ
		PORTB = 0x55;
		//UARTSendString(UART1_BASE,"CMD1д��ʧ��SD����ʼ��ʧ��\r\n");
		return(INIT_CMD1_ERROR);//CMD1д��ʧ��
	}
	}while(temp!=0);
   	//UARTSendString(UART1_BASE,"CMD1д��ɹ�SD����ʼ���ɹ�\r\n");
	SPInCS_1();  //�ر�Ƭѡ 

	SPIWriteByte(0xff); //����SD���Ĳ���ʱ�������ﲹ8��ʱ��

  SPI0BR  = 0x01; //BR=busclk/((SPPR + 1)�� 2^(SPR + 1))   SPI:20M
	return(0); //����0,˵����ʼ�������ɹ�
} 

/****************************************************************************
 - ������������bufferָ���512���ֽڵ�����д�뵽SD����addr������
 - ����ģ�飺SD��ģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵����addr:������ַ
             buffer:ָ�����ݻ�������ָ��
 - ����˵�������óɹ�������0x00�����򷵻�WRITE_BLOCK_ERROR (sd.h���ж���)
 - ע��SD����ʼ���ɹ��󣬶�д����ʱ��������SPI�ٶ������������Ч��
 ****************************************************************************/

unsigned char SD_Write_Sector(unsigned long addr,unsigned char *buffer)	//��SD���е�ָ����ַ������д��512���ֽڣ�ʹ��CMD24������24��
{  
 unsigned char temp,time;
 unsigned int i;
 unsigned char pcmd[] = {0x58,0x00,0x00,0x00,0x00,0xff}; //��SD���е����飨512�ֽڣ�һ��������д�����ݣ���CMD24

 addr<<=9; //addr = addr * 512	�����ַ��������ַ��תΪ�ֽڵ�ַ �������������SD�����������Ϊ4G��
	
 pcmd[1]=((addr&0xff000000)>>24); //���ֽڵ�ַд�뵽CMD24�ֽ�������
 pcmd[2]=((addr&0x00ff0000)>>16);
 pcmd[3]=((addr&0x0000ff00)>>8);

 SPInCS_0(); //��SD��Ƭѡ

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd);
  time++;
  if(time==TRY_TIME) 
  { 
   SPInCS_1();  //�ر�Ƭѡ
   return(temp); //����д��ʧ��
  }
 }while(temp!=0); 

 for(i=0;i<100;i++) //����Ҫ��������ʱ���ź�
 {
  SPIWriteByte(0xff);
 }
	
 SPIWriteByte(0xfe);//д�뿪ʼ�ֽ� 0xfe���������Ҫд���512���ֽڵ�����	
	
 for(i=0;i<512;i++) //����������Ҫд���512���ֽ�д��SD��
 {
  SPIWriteByte(0xf3);
 }
  
 SPIWriteByte(0xff); 
 SPIWriteByte(0xff); //�����ֽڵ�CRCУ���룬���ù���
   
    
 temp=SPIReadByte();   //��ȡ����ֵ
 if((temp&0x1F)!=0x05) //�������ֵ�� XXX00101˵�������Ѿ���SD��������
 {
  SPInCS_1(); 
  return(WRITE_BLOCK_ERROR); //д������ʧ��
 }
 
 while(SPIReadByte()!=0xff);//�ȵ�SD����æ�����ݱ������Ժ�SD��Ҫ����Щ����д�뵽�����FLASH�У���Ҫһ��ʱ�䣩
						     //æʱ����������ֵΪ0x00,��æʱ��Ϊ0xff

 SPInCS_1();  //�ر�Ƭѡ

 SPIWriteByte(0xff);//����SD���Ĳ���ʱ�������ﲹ8��ʱ��
 return(0);		 //����0,˵��д���������ɹ�
} 

/****************************************************************************
 - ������������ȡaddr������512���ֽڵ�bufferָ������ݻ�����
 - ����ģ�飺SD��ģ��
 - �������ԣ��ⲿ�����û�����
 - ����˵����addr:������ַ
             buffer:ָ�����ݻ�������ָ��
 - ����˵�������óɹ�������0x00�����򷵻�READ_BLOCK_ERROR (sd.h���ж���)
 - ע��SD����ʼ���ɹ��󣬶�д����ʱ��������SPI�ٶ������������Ч��
 ****************************************************************************/

unsigned char SD_Read_Sector(unsigned long addr,unsigned char *buffer)//��SD����ָ�������ж���512���ֽڣ�ʹ��CMD17��17�����
{
 unsigned int j;
 unsigned char time,temp;
 unsigned char pcmd[]={0x51,0x00,0x00,0x00,0x00,0xff}; //CMD17���ֽ�����
   
 addr<<=9; //addr=addr*512	   �����ַ��������ַ��תΪ�ֽڵ�ַ

 pcmd[1]=((addr&0xff000000)>>24);//���ֽڵ�ַд�뵽CMD17�ֽ�������
 pcmd[2]=((addr&0x00FF0000)>>16);
 pcmd[3]=((addr&0x0000FF00)>>8);

 SPInCS_0(); //��Ƭѡ 

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd); //д��CMD17
  time++;
  if(time==TRY_TIME) 
  {
   return(READ_BLOCK_ERROR); //����ʧ��
  }
 }while(temp!=0); 
   			
 while (SPIReadByte()!= 0xfe); //һֱ����������0xfeʱ��˵���������512�ֽڵ�������

 for(j=0;j<512;j++)	 //������д�뵽���ݻ�������
 {	
  buffer[j]=SPIReadByte();
 }

 SPIReadByte();
 SPIReadByte();//��ȡ�����ֽڵ�CRCУ���룬���ù�������

 SPInCS_1();   //SD���ر�Ƭѡ 

 SPIWriteByte(0xff);//����SD���Ĳ���ʱ�������ﲹ8��ʱ��

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
  while(!SPI0SR_SPTEF); //�ȴ����ݴ洢�����
  SPI0DR = value;
  while(!SPI0SR_SPIF);
  return SPI0DR;   
}

/************************************************************************************
Func Name:    SPI_Read_Byte                                
FuncDescrip:  SPI��һ���ֽ�
************************************************************************************/
uchar SPIReadByte()
{  
  return  SPIWriteByte(0xFF);
}
/************************************************************************************
Func Name:    SPI_Init                                
FuncDescrip:  SPI��ʼ��
************************************************************************************/
void SPIInit(void)
{
  MODRR   = 0x10; //����SPI������ΪPM��  PM2:MISO  PM3:SS   PM4:MOSI    PM5:SCK
  SPI0CR2 = 0x00; //SSΪ�Զ�Ӳ��Ƭѡ��SPI 4Wireģʽ
  SPI0CR1 = 0x50; //ʱ�ӵ�ƽ����Ϊ�ͣ��ر��жϣ�MSB����  
  SPI0BR  = 0x66; //BR=busclk/((SPPR + 1)�� 2^(SPR + 1))   SPI:5M
  DDRM_DDRM3 = 1;  //Ƭѡ�ź����              
}
unsigned char SD_Write_Image(unsigned long addr)	//��SD���е�ָ����ַ������д��512���ֽڣ�ʹ��CMD24������24��
{  
 unsigned char temp,time;
 unsigned int i,j;
 unsigned char pcmd[] = {0x58,0x00,0x00,0x00,0x00,0xff}; //��SD���е����飨512�ֽڣ�һ��������д�����ݣ���CMD24

 //addr<<=9; //addr = addr * 512	�����ַ��������ַ��תΪ�ֽڵ�ַ �������������SD�����������Ϊ4G��
 addr=addr*2500;
 pcmd[1]=((addr&0xff000000)>>24); //���ֽڵ�ַд�뵽CMD24�ֽ�������
 pcmd[2]=((addr&0x00ff0000)>>16);
 pcmd[3]=((addr&0x0000ff00)>>8);

 SPInCS_0(); //��SD��Ƭѡ

 time=0;
 do
 {  
  temp=SD_Write_Cmd(pcmd);
  time++;
  if(time==TRY_TIME) 
  { 
   SPInCS_1();  //�ر�Ƭѡ
   return(temp); //����д��ʧ��
  }
 }while(temp!=0); 

 for(i=0;i<100;i++) //����Ҫ��������ʱ���ź�
 {
  SPIWriteByte(0xff);
 }
	
 SPIWriteByte(0xfe);//д�뿪ʼ�ֽ� 0xfe���������Ҫд���512���ֽڵ�����	
	
 
 for(i=0;i<30;i++) 
 {
    for(j=0;j<70;j++) 
    {
       SPIWriteByte(0x00);
    }
    
    SPIWriteByte(0x20);////////д��һ�пո�
    SPIWriteByte(0x20);////////д��һ�пո�
    SPIWriteByte(0x20);////////д��һ�пո�
    SPIWriteByte(0x20);////////д��һ�пո�
    SPIWriteByte(0x20);
    SPIWriteByte(0x20);
    SPIWriteByte(0x0d);////////д��һ�лس�
 }
 SPIWriteByte(0x0d);////////д��һ���س�
 SPIWriteByte(0x0d);////////д��һ���س�
 SPIWriteByte(0x0d);////////д��һ���س�
 SPIWriteByte(0x0d);////////д��һ���س�
  
 
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
 SPIWriteByte(0x0d);////////д��س�
 
  
  
 
             
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 SPIWriteByte(0xff); 
 SPIWriteByte(0xff); //�����ֽڵ�CRCУ���룬���ù���
   
    
 temp=SPIReadByte();   //��ȡ����ֵ
 if((temp&0x1F)!=0x05) //�������ֵ�� XXX00101˵�������Ѿ���SD��������
 {
  SPInCS_1(); 
  return(WRITE_BLOCK_ERROR); //д������ʧ��
 }
 
 while(SPIReadByte()!=0xff);//�ȵ�SD����æ�����ݱ������Ժ�SD��Ҫ����Щ����д�뵽�����FLASH�У���Ҫһ��ʱ�䣩
						     //æʱ����������ֵΪ0x00,��æʱ��Ϊ0xff

 SPInCS_1();  //�ر�Ƭѡ

 SPIWriteByte(0xff);//����SD���Ĳ���ʱ�������ﲹ8��ʱ��
 return(0);		 //����0,˵��д���������ɹ� 
}
