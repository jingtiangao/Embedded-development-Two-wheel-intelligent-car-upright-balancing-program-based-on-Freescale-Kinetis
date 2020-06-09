#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "nRF24L01.h"
#include "Define.h"

uchar const TX_ADDRESS[TX_ADR_WIDTH]={'z','c','1','2','3'}; // ���շ������ݵĵ�ַ.
uchar Tx_Buf[TX_PLOAD_WIDTH]={0};
uchar Rx_Buf[RX_PLOAD_WIDTH]={0};
uchar Status_Flag;
uchar Status_Value;

#define	RX_DR  (Status_Value & 0x40)
#define	TX_DS  (Status_Value & 0x20)
#define	MAX_RT (Status_Value & 0x10)


void Delay_02(uint i)  
{
  while(i--);
}
  
/************************************************************************************
Func Name:    SPI_Init                                
FuncDescrip:  SPI��ʼ��
************************************************************************************/
void SPI_Init(void)
{
  MODRR   = 0x10; //����SPI������ΪPM��  PM2:MISO  PM3:SS   PM4:MOSI    PM5:SCK
  SPI0CR2 = 0x00; //SSΪ�Զ�Ӳ��Ƭѡ��SPI 4Wireģʽ
  SPI0CR1 = 0x50; //ʱ�ӵ�ƽ����Ϊ�ͣ��ر��жϣ�MSB����  
  SPI0BR  = 0x40; //BR=busclk/((SPPR + 1)�� 2^(SPR + 1))   SPI:7M                
}


/************************************************************************************
Func Name:    SPI_Write_Byte                                
FuncDescrip:  SPIдһ���ֽ�
************************************************************************************/
uchar SPI_Write_Byte(uchar value)
{
  while(!SPI0SR_SPTEF); //�ȴ����ݴ洢�����
  SPI0DR = value;
  while(!SPI0SR_SPIF);
  return SPI0DR;   
}

                    
/************************************************************************************
Func Name:    SPI_RW_Reg                                
FuncDescrip:  дnRF24L01�ļĴ���
************************************************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{ 
  uchar status;
  
  PTM_PTM3=0;
  
  status = SPI_Write_Byte(reg);             // �����������ַ
  SPI_Write_Byte(value);             // д1 BYTE ֵ
  
  PTM_PTM3=1;
  
  return status; 
}

                    
/************************************************************************************
Func Name:    SPI_Read_Byte                                
FuncDescrip:  SPI��һ���ֽ�
************************************************************************************/
uchar SPI_Read_Byte(uchar reg)
{ 
  uchar Dat;
  
  PTM_PTM3=0; 
      
  SPI_Write_Byte(reg);
  Dat=SPI_Write_Byte(0);
  
  PTM_PTM3=1;
  
  return  Dat;
}

            
/************************************************************************************
Func Name:    nRF24L01_IOInit                                
FuncDescrip:  nRF24L01�˿ڳ�ʼ��
************************************************************************************/
void nRF24L01_IOInit(void)
{	
  DDRS_DDRS0 =1;     
  PTS_PTS0   =1;  
  DDRM_DDRM3 =1;  
  PTM_PTM3   =1;  
  SPI_Init();
}

                           
/************************************************************************************
Func Name:    SPI_Read_Buf                                
FuncDescrip:  nRF24L01��������
************************************************************************************/
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes)
{ 
  uchar status,byte_ctr;
  
  PTM_PTM3=0;
  
  status = SPI_Write_Byte(reg);       		// ������������ݵ�����.				61H
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
  pBuf[byte_ctr] = SPI_Write_Byte(0);    // �� 1 BYTE����
  
  PTM_PTM3=1;
  
  return status;                    // return nRF24L01 status byte
}

     
/************************************************************************************
Func Name:    SPI_Write_Buf                                
FuncDescrip:  nRF24L01д������
************************************************************************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{ 
  uchar status,byte_ctr;
  
  PTM_PTM3=0;
  
  status = SPI_Write_Byte(reg);      // ����д�������ݵ�����.				A0H
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // ���ͷ�������
  SPI_Write_Byte(*pBuf++);
  
  PTM_PTM3=1;
  
  return status;          // return nRF24L01 status byte
}


/************************************************************************************
Func Name:    RX_Mode                                
FuncDescrip:  nRF24L01ת������ģʽ
************************************************************************************/
void RX_Mode(void)
{
	PTS_PTS0=0;
	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
	
	PTS_PTS0=1;
}


/************************************************************************************
Func Name:    TX_Mode                                
FuncDescrip:  nRF24L01ת������ģʽ
************************************************************************************/
void TX_Mode(uchar *Dat)
{
  PTS_PTS0=0;
  
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  SPI_Write_Buf(WR_TX_PLOAD, Dat, TX_PLOAD_WIDTH); // Writes data to TX payloads
  
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0E);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
  
  PTS_PTS0=1;
}

                                  
/************************************************************************************
Func Name:    nRF24L01_Config                                
FuncDescrip:  nRF24L01����ģʽ
************************************************************************************/
void nRF24L01_Config()
{
  PTS_PTS0=0;
  
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack

  SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
  SPI_RW_Reg(WRITE_REG + RF_CH, 97);        // Select RF channel 7
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR

	PTS_PTS0=1;	
}

                                    
/************************************************************************************
Func Name:    nRF24L01Init                                
FuncDescrip:  nRF24L01��ʼ��
************************************************************************************/
void nRF24L01Init()  
{
  uchar xDat;
	Status_Flag=0;
	//IRQCR_IRQEN=1;        //��ֹIRQ�ж�
	nRF24L01_IOInit();		// Initialize IO port
 	nRF24L01_Config();
 	//for(xDat=0;xDat<32;xDat++) 	
 	{
 	  Tx_Buf[0]=0x78;
 	} 	
 	TX_Mode(Tx_Buf);				// set TX mode
 	IRQCR_IRQEN=1;                  //ʹ��IRQ�ж�
 	//RX_Mode();				// set TX mode
}


/************************************************************************************
Func Name:    IRQ_ISR                                
FuncDescrip:  �ⲿIRQ�жϺ���
************************************************************************************/
#pragma CODE_SEG  NON_BANKED //ָʾ�ó����ڲ���ҳ�� 

void  interrupt 6 IRQ_ISR(void)        //���жϺ���
{
    Status_Value=SPI_Read_Byte(STATUS);
    
  	if(RX_DR)																				  // �ж��Ƿ��ǽ����ж�
  	{
  		Status_Flag=1;
  		SPI_Read_Buf(RD_RX_PLOAD,Rx_Buf,RX_PLOAD_WIDTH);// ��ȡ�յ�������
  		//if(Rx_Buf[2]==7)  SendFrameFlag=1;
  	}
  	else if(TX_DS)
  	{
  		Status_Flag = 2;
  	}
  	else if(MAX_RT)
  	{
  		SPI_RW_Reg(FLUSH_TX,0);
  		Status_Flag = 3;
  	}
  	SPI_RW_Reg(WRITE_REG+STATUS,Status_Value);									// clear RX_DR or TX_DS or MAX_RT interrupt flag  
}
#pragma CODE_SEG DEFAULT


/************************************************************************************
Func Name:    Wireless_Send_Byte                                
FuncDescrip:  ���߷���һ���ֽ�
************************************************************************************/
void Wireless_Send_Byte(uchar Dat)
{
    Tx_Buf[0]=Dat;
    if((Status_Flag==2) || (Status_Flag==3))
    {
      IRQCR_IRQEN=0;                  //ʹ��IRQ�ж�
      Status_Flag=0;
      TX_Mode(Tx_Buf);
      IRQCR_IRQEN=1;
    }
    Delay_02(6000); 
}


/************************************************************************************
Func Name:    Wireless_Send_Buf                                
FuncDescrip:  ���߷��ͻ�����������
************************************************************************************/
void Wireless_Send_Buf(uchar *Buf,uint BufSize) 
{
  uint iCot;
  for(iCot=0;iCot<BufSize;iCot++) 
  {
    Wireless_Send_Byte(*Buf++); 
  }
}


/************************************************************************************
Func Name:    Wireless_Send_Buf                                
FuncDescrip:  ���߷��ͻ�����������
************************************************************************************/
void Wireless_Send_IntNumBuf(uint *Buf,uint BufSize) 
{
  uint iCot;
  for(iCot=0;iCot<BufSize;iCot++) 
  {  
    Wireless_Send_Byte(Buf[iCot]/100+48);
    Wireless_Send_Byte(Buf[iCot]/10%10+48);
    Wireless_Send_Byte(Buf[iCot]%10+48);
    Wireless_Send_Byte(' '); 
  }
}
