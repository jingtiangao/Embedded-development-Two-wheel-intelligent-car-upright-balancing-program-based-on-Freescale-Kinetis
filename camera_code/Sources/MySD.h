#ifndef _H_TFCARD_
#define	_H_TFCARD_
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#define DELAY_TIME 2000 //SD���ĸ�λ���ʼ��ʱSPI����ʱ����������ʵ�������޸���ֵ����������SD����λ���ʼ��ʧ��
#define TRY_TIME 200    //��SD��д������֮�󣬶�ȡSD���Ļ�Ӧ����������TRY_TIME�Σ������TRY_TIME���ж�������Ӧ��������ʱ��������д��ʧ��

//�����붨��
//-------------------------------------------------------------
#define INIT_CMD0_ERROR     0x01 //CMD0����
#define INIT_CMD1_ERROR     0x02 //CMD1����
#define WRITE_BLOCK_ERROR   0x03 //д�����
#define READ_BLOCK_ERROR    0x04 //�������
//-------------------------------------------------------------

//��������
uchar SPIWriteByte(uchar value);
uchar SPIReadByte();
void SPIInit(void);
unsigned char SD_Reset(void);
unsigned char SD_Init(void);
unsigned char SD_Write_Sector(unsigned long addr,unsigned char *buffer);
unsigned char SD_Read_Sector(unsigned long addr,unsigned char *buffer);
unsigned char SD_Write_Image(unsigned long addr);
#endif