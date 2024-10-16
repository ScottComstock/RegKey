/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : keygenc.h

     VERSION : 3.00

     PURPOSE : Header file for functions implemented in keygenc.c

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/

#include "regkey.h"

/* Boolean definitions */
#ifndef TRUE
#define TRUE (1 == 1)
#define FALSE (0 == 1)
#endif

/* Information structure definition */
#define KG_MAX_APPS         64
#define KG_APP_NAME_LEN     37
#define KG_FILENAME_LEN     129

#ifdef _MSC_VER
#pragma pack(1)
#endif

typedef struct
   {
   char szAppName[KG_APP_NAME_LEN];
   char bFileBased;
   char bPasswordNeeded;
   unsigned long int lPasswordCRC;
   char szGenerationCode[11];
   char szValidationCode[11];
   } KGAPPINFO;

typedef struct
   {
   char szFilename[KG_FILENAME_LEN];
   unsigned short int uNumApps;
   KGAPPINFO appInfo[KG_MAX_APPS];
   } KGINFO;

#ifdef _MSC_VER
#pragma pack()
#endif

/* Private function prototypes */
RKRETURN kgInit(const char *szCommandLine, KGINFO *pKGInfo);
RKRETURN kgWriteFile(KGINFO *pKGInfo);
void xorScrambler(char *szToScramble, const char *szKey);
unsigned long int crc32(char *szString);
char okayToAdd(KGINFO *pKGInfo);
unsigned short int kgIndexOf(const char *szString, KGINFO *pKGInfo);
RKRETURN kgAdd(const char *szAppName, const char *szPassword,
                         char bPasswordNeeded, char bFileBased,
                         const char *szGenCode, KGINFO *pKGInfo);
RKRETURN kgDelete(unsigned short int uWhich, KGINFO *pKGInfo);
char isPasswordValid(const char *szPassword,
                   unsigned short int uWhich, const KGINFO *pKGInfo);
char passwordNeeded(unsigned short int uWhich, const KGINFO *pKGInfo);
char fileBased(unsigned short int uWhich, const KGINFO *pKGInfo);
RKRETURN kgGen(unsigned short int uWhich, const char *szRegString,
                         const char *szRandSeed, const char *szPassword,
                         const char *szFileName, char *szResult,
                         const KGINFO *pKGInfo, char *szKey, char *bUserEntered);
void getAppInfo(unsigned short int uWhich, const char *szPassword,
                          KGAPPINFO *appInfo, const KGINFO *pKGInfo);
RKRETURN updateAppInfo(unsigned short int uWhich,
                                 const char *szOldPassword,
                                 const char *szNewPassword,
                                 KGAPPINFO *appInfo, KGINFO *pKGInfo);
char hasGenCodeChanged(unsigned short int uWhich,
                                 const char *szOriginalPassword,
                                 const char *szGenCode, const KGINFO *pKGInfo);
void filenameFromString(char *szOut, const char *szIn);
