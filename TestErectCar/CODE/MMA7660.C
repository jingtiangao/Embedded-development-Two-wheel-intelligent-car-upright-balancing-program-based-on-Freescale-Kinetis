/*
**==============================================================================
** MMA7660.C:             -- by Dr. ZhuoQing, 2011-12-1
**
**==============================================================================
*/

#include "I2C1.h"
#define MMA7660_GLOBALS        1                       // Define the global variables
#include "MMA7660.H"
//==============================================================================

//------------------------------------------------------------------------------
unsigned char MMA7660Init(void) {
	if(I2C1_SelectSlave(MMA7660_ADD)) return 3;
	if(MMA7660Write(0x7, 0x1) != ERR_OK) 	return 1; 	// Start convert
  	if(MMA7660Write(0x8, 0xe0) != ERR_OK) 	return 2;
	return 0;
}
unsigned char MMA7660GetXYZ(unsigned char * pucXYZ) {
	return MMA7660ReadDim(pucXYZ, 3);
}

//------------------------------------------------------------------------------
// Change the sign value of ucValue into unsigned value
//------------------------------------------------------------------------------
unsigned char MMA7660Result(unsigned char ucValue) {
	if(ucValue & 0x20) {
		ucValue |= (0xc0);
		return (unsigned char)(((unsigned char)(32 + (signed char)ucValue)) << 2);
	} else {
		ucValue &= 0x1f;
		return (unsigned char)(((unsigned char)(32 + ucValue)) << 2);
	}
}
//------------------------------------------------------------------------------
//  MMA7660 Write/Read Function: return 0:OK
unsigned char MMA7660Read(unsigned char ucAddress, unsigned char * pucChar) {
	unsigned char ucReturn;
	ucReturn = I2C1_SendChar(ucAddress);
	if(ucReturn != ERR_OK) return ucReturn;
	ucReturn = I2C1_RecvChar(pucChar);
	if(ucReturn != ERR_OK) return ucReturn;
	return 0;
}

//------------------------------------------------------------------------------
unsigned char g_ucMMA7660Buffer[16];
unsigned char MMA7660Write(unsigned char ucAddress, unsigned char ucChar) {
	g_ucMMA7660Buffer[0] = ucAddress;
	g_ucMMA7660Buffer[1] = ucChar;
	if(I2C1_SendBlock(g_ucMMA7660Buffer, 2, (word *)&I2C1_SndRcvTemp) == ERR_OK) return 0;
	else return 1;
}

unsigned char MMA7660ReadDim(unsigned char * pucBuffer, unsigned int nNumber) {
	return I2C1_RecvBlock(pucBuffer, nNumber, (word *)&I2C1_SndRcvTemp);
}	
unsigned char MMA7660WriteDim(unsigned char * pucBuffer, unsigned int nNumber) {
	unsigned int i;
	for(i = 0; i < nNumber; i ++)
		g_ucMMA7660Buffer[i + 1] = *(pucBuffer + i);
	g_ucMMA7660Buffer[0] = 0;
	
	if(I2C1_SendBlock(g_ucMMA7660Buffer, nNumber + 1, (word *)&I2C1_SndRcvTemp) == ERR_OK) return 0;
	else return 1;
}
	


//==============================================================================
//                END OF THE FILE : MMA7660.C
//------------------------------------------------------------------------------

