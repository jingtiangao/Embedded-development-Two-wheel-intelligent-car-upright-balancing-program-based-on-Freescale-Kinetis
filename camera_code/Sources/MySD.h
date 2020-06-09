#ifndef _H_TFCARD_
#define	_H_TFCARD_
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#define DELAY_TIME 2000 //SD卡的复位与初始化时SPI的延时参数，根据实际速率修改其值，否则会造成SD卡复位或初始化失败
#define TRY_TIME 200    //向SD卡写入命令之后，读取SD卡的回应次数，即读TRY_TIME次，如果在TRY_TIME次中读不到回应，产生超时错误，命令写入失败

//错误码定义
//-------------------------------------------------------------
#define INIT_CMD0_ERROR     0x01 //CMD0错误
#define INIT_CMD1_ERROR     0x02 //CMD1错误
#define WRITE_BLOCK_ERROR   0x03 //写块错误
#define READ_BLOCK_ERROR    0x04 //读块错误
//-------------------------------------------------------------

//函数声明
uchar SPIWriteByte(uchar value);
uchar SPIReadByte();
void SPIInit(void);
unsigned char SD_Reset(void);
unsigned char SD_Init(void);
unsigned char SD_Write_Sector(unsigned long addr,unsigned char *buffer);
unsigned char SD_Read_Sector(unsigned long addr,unsigned char *buffer);
unsigned char SD_Write_Image(unsigned long addr);
#endif