/*******************************************************************************
*
* FILE:         seedx.h
*
* DESCRIPTION:  header file for seedx.cpp
*
*******************************************************************************/

#ifndef SEED_H
#define SEED_H

/********************** Include files ************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/********************* Type Definitions **********************/

#ifndef TYPE_DEFINITION
    #define TYPE_DEFINITION
    #if defined(__alpha)
        typedef unsigned int        DWORD;
        typedef unsigned short      WORD;
    #else
        typedef unsigned long int   DWORD;
        typedef unsigned short int  WORD;
    #endif
    typedef unsigned char           BYTE;
#endif


/******************* Constant Definitions *********************/

#define NoRounds         16
#define NoRoundKeys      (NoRounds*2)
#define SeedBlockSize    16    /* in bytes */
#define SeedBlockLen     128   /* in bits */


/********************** Common Macros ************************/

#if defined(_MSC_VER)
    #define ROTL(x, n)     (_lrotl((x), (n)))
    #define ROTR(x, n)     (_lrotr((x), (n)))
#else
    #define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
    #define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
#endif


/**************** Function Prototype Declarations **************/

#ifndef PROTOTYPES
#define PROTOTYPES  1
#endif

#if PROTOTYPES
#define PROTO_LIST(list)    list
#else
#define PROTO_LIST(list)    ()
#endif

class cSEED
{
public:
static void SeedInit();
static void SeedEncrypt 
    PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
static void SeedDecrypt
    PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
static void SeedEncRoundKey
    PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));
static void SeedDecRoundKey
    PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));
static void	EncryptOR(BYTE *data,int size,BYTE	key);
static void	DecryptOR(BYTE *data,int size,BYTE	key);
};
/******************************************************************/
#endif
