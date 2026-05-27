/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jul 02 18:27:41 2010
 */
/* Compiler settings for D:\work\RS\RedStoneWL\RedStoneWL.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IRedStoneWL = {0x6E77E4DE,0x2890,0x4AA8,{0xB1,0x22,0x96,0x4F,0x4F,0xC0,0xB0,0x68}};


const IID LIBID_REDSTONEWLLib = {0x3DC591A7,0xF4F9,0x498F,{0x8C,0x68,0x9C,0x8F,0x3B,0x5F,0x0F,0x1C}};


const IID DIID__IRedStoneWLEvents = {0x096F7371,0xE7D0,0x42D9,{0x82,0x0C,0x92,0x44,0x16,0x8F,0x7E,0x77}};


const CLSID CLSID_RedStoneWL = {0xBCF37AD2,0xF141,0x4B0F,{0x9E,0x56,0xA1,0xE9,0x68,0xE2,0x55,0x9E}};


#ifdef __cplusplus
}
#endif

