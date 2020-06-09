/*
**==============================================================================
** MMA7660.H:             -- by Dr. ZhuoQing, 2011-12-1
**
**  Description:
**
**==============================================================================
*/
#ifndef __MMA7660__
#define __MMA7660__
//------------------------------------------------------------------------------
#ifdef MMA7660_GLOBALS
    #define MMA7660_EXT
#else
    #define MMA7660_EXT extern
#endif // MMA7660_GLOBALS
//------------------------------------------------------------------------------
//==============================================================================

//------------------------------------------------------------------------------

#define MMA7660_ADD			0x4c
unsigned char MMA7660Init(void);
unsigned char MMA7660GetXYZ(unsigned char * pucXYZ);
unsigned char MMA7660Result(unsigned char ucValue); //Change the MAA7660 result into unsigned char 
                                                // range : 0~63.

//------------------------------------------------------------------------------
//  MMA7660 Write/Read Function: return 0:OK
unsigned char MMA7660Read(unsigned char ucAddress, unsigned char * pucChar);
unsigned char MMA7660Write(unsigned char ucAddress, unsigned char ucChar);
unsigned char MMA7660ReadDim(unsigned char * pucBuffer, unsigned int nNumber);
unsigned char MMA7660WriteDim(unsigned char * pucBuffer, unsigned int nNumber);
//------------------------------------------------------------------------------


//==============================================================================
//             END OF THE FILE : MMA7660.H
//------------------------------------------------------------------------------
#endif // __MMA7660__
