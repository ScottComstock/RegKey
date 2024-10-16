/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : keygenc.c

     VERSION : 3.10

     PURPOSE : Source code for the KeyGen facilities that are common to
               both DOS and Windows versions of KeyGen; the non-
               user interface - specific porttions of KeyGen.

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUILDING_REGKEY

#include "regkey.h"
#include "keygenc.h"


unsigned long crc_32_tab[256]={
0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL, 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L, 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L, 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L, 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L, 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL, 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL};


void xorScrambler(char *szToScramble, const char *szKey)
   {
   char *cpCurKey = (char *)szKey;

   /* Validate parameters if operating in test mode */
   ASSERT(szToScramble != NULL);
   ASSERT(szKey != NULL);
   ASSERT(*szKey != '\0');

   /* Scramble each character of the string szToScramble */
   while(*szToScramble)
      {
      *szToScramble++ ^= *cpCurKey;
      if(!*++cpCurKey) cpCurKey = (char *)szKey;
      }
   }


unsigned long int crc32(char *szString)
   {
   unsigned long int crc32;
/*   unsigned long int oldcrc;
   char nextchar;
   unsigned char index;
   char string[100]; */

   /* Validate parameters if operating in test mode */
   ASSERT(szString != NULL);

   crc32=0xffffffffL;

   while(*szString)
      {
      crc32=crc_32_tab[(unsigned char)(((crc32^(long)(unsigned char )(*(unsigned char *)szString)))&0xff)]^((crc32<<8)&0x00ffffffL);
      ++szString;
      }

   return(crc32);
   }


RKRETURN kgWriteFile(KGINFO *pKGInfo)
   {
   FILE *fp;
   RKRETURN returnCode = RK_SUCCESS;

   /* Validate parameters if operating in test mode */
   ASSERT(pKGInfo != NULL);

   /* Open the file */
   if((fp = fopen(pKGInfo->szFilename,"wb")) == NULL) return(RK_FAILURE);

   /* Write information to file */
   if(fwrite(&pKGInfo->uNumApps, sizeof(unsigned short int), 1, fp) != 1
      || fwrite(&pKGInfo->appInfo, sizeof(KGAPPINFO), pKGInfo->uNumApps, fp)
         != pKGInfo->uNumApps)
      {
      returnCode = RK_FAILURE;
      }

   /* Close the file */
   fclose(fp);

   return(returnCode);
   }


RKRETURN kgInit(const char *szCommandLine, KGINFO *pKGInfo)
   {
   char *cp;
   FILE *fp;

   /* Validate parameters if operating in test mode */
   ASSERT(szCommandLine != NULL);
   ASSERT(pKGInfo != NULL);

   /* Get filename of data file */
   strncpy(pKGInfo->szFilename, szCommandLine, KG_FILENAME_LEN - 11);
   pKGInfo->szFilename[KG_FILENAME_LEN - 1] = '\0';
   if((cp = strchr(pKGInfo->szFilename, ' ')) != NULL)
      {
      *cp = '\0';
      }
   if((cp = strrchr(pKGInfo->szFilename, '\\')) != NULL)
      {
      *++cp = '\0';
      }
   else
      {
      pKGInfo->szFilename[0]='\0';
      }
   strcat(pKGInfo->szFilename, "KEYGEN.DAT");

   /* Attempt to open existing KEYGEN.DAT file */
   if((fp = fopen(pKGInfo->szFilename,"rb")) != NULL)
      {
      /* Get number of records in file */
      if(fread(&pKGInfo->uNumApps, sizeof(unsigned short int), 1, fp) != 1)
         {
         fclose(fp);
         return(RK_FAILURE);
         }

      /* Ensure that number of records is not greater than maximumm */
      if(pKGInfo->uNumApps > KG_MAX_APPS)
         {
         fclose(fp);
         return(RK_FAILURE);
         }

      /* Read records from file */
      if(fread(&pKGInfo->appInfo, sizeof(KGAPPINFO), pKGInfo->uNumApps, fp)
         != pKGInfo->uNumApps)
         {
         fclose(fp);
         return(RK_FAILURE);
         }

      /* Close the file */
      fclose(fp);
      }

   /* If unable to open existing file, attempt to create new KEYGEN.DAT file */
   else
      {
      /* New file will have zero applications to begin with */
      pKGInfo->uNumApps = 0;

      /* Attempt to write new file */
      if(kgWriteFile(pKGInfo) == RK_FAILURE)
         {
         /* If failed, then KeyGen initialization must fail */
         return(RK_FAILURE);
         }
      }

   return(RK_SUCCESS);
   }


unsigned short int kgIndexOf(const char *szString, KGINFO *pKGInfo)
   {
   unsigned short int uWhich;
   
   for(uWhich = 0; uWhich < pKGInfo->uNumApps; ++uWhich)
      {
      if(strcmp(szString, pKGInfo->appInfo[uWhich].szAppName) == 0)
         {
         return(uWhich);
         }
      }
      
   return(0xffff);   
   }


char okayToAdd(KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(pKGInfo != NULL);

   return(pKGInfo->uNumApps < KG_MAX_APPS);
   }


RKRETURN kgAdd(const char *szAppName, const char *szPassword,
                         char bPasswordNeeded, char bFileBased,
                         const char *szGenCode, KGINFO *pKGInfo)
   {
   KGAPPINFO *appInfo;

   /* Validate parameters if operating in test mode */
   ASSERT(szAppName != NULL);
   ASSERT(szPassword != NULL);
   ASSERT(szGenCode != NULL);
   ASSERT(pKGInfo != NULL);

   /* Check for existance of application with this name */
   if(kgIndexOf(szAppName, pKGInfo) != 0xffff)
      {
      return(RK_FAILURE);
      }

   /* Get pointer to KGAPPINFO structure where new information will go */
   appInfo = &pKGInfo->appInfo[pKGInfo->uNumApps];

   /* Copy application name */
   strncpy(appInfo->szAppName, szAppName, KG_APP_NAME_LEN);
   appInfo->szAppName[KG_APP_NAME_LEN - 1] = '\0';

   /* Copy file-based setting */
   appInfo->bFileBased = bFileBased;

   /* Copy generation code */
   strncpy(appInfo->szGenerationCode, szGenCode, 10);
   appInfo->szGenerationCode[10] = '\0';

   /* Calculate validation code */
   if(RegKeyNewCodeSet(appInfo->szGenerationCode, appInfo->szValidationCode) == RK_FAILURE)
      {
      return(RK_FAILURE);
      }

   /* Deal with password, if required */
   appInfo->bPasswordNeeded = bPasswordNeeded;
   if(bPasswordNeeded)
      {
      appInfo->lPasswordCRC = crc32((char *)szPassword);
      xorScrambler(appInfo->szGenerationCode, szPassword);
      xorScrambler(appInfo->szValidationCode, szPassword);
      }

   /* Increment total number of applications */
   pKGInfo->uNumApps++;

   /* Attempt to add new record to file */
   if(kgWriteFile(pKGInfo) == RK_FAILURE)
      {
      /* If failed, decrement total number of application and return failure */
      pKGInfo->uNumApps--;
      return(RK_FAILURE);
      }

   /* Return with success */
   return(RK_SUCCESS);
   }


RKRETURN kgDelete(unsigned short int uWhich, KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Decrement total number of applications */
   --pKGInfo->uNumApps;

   /* Move any following applications up one spot */
   while(uWhich < pKGInfo->uNumApps)
      {
      pKGInfo->appInfo[uWhich] = pKGInfo->appInfo[uWhich+1];
      ++uWhich;
      }

   /* Update file on disk */
   return(kgWriteFile(pKGInfo));
   }


char isPasswordValid(const char *szPassword,
unsigned short int uWhich, const KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(szPassword != NULL);
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* If no password required, return TRUE */
   if(!passwordNeeded(uWhich, pKGInfo))
      {
      return(TRUE);
      }

   /* Otherwise, return true iff password's crc matches crc stored in file */
   return(pKGInfo->appInfo[uWhich].lPasswordCRC == crc32((char *)szPassword));
   }


char passwordNeeded(unsigned short int uWhich, const KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Return true iff a password is required for the specified application */
   return(pKGInfo->appInfo[uWhich].bPasswordNeeded);
   }


char fileBased(unsigned short int uWhich, const KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Return true iff file-based registration keys are used for this application */
   return(pKGInfo->appInfo[uWhich].bFileBased);
   }


RKRETURN kgGen(unsigned short int uWhich, const char *szRegString,
                         const char *szRandSeed, const char *szPassword,
                         const char *szFileName, char *szResult,
                         const KGINFO *pKGInfo, char *szKey, char *bUserEntered)
   {
   char szGenCode[11];
   char szRegKey[21];
   KGAPPINFO *appInfo;

   /* Validate parameters if operating in test mode */
   ASSERT(szRegString != NULL);
   ASSERT(szRandSeed != NULL);
   ASSERT(szFileName != NULL);
   ASSERT(szPassword != NULL);
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Get pointer to KGAPPINFO structure for this application */
   appInfo = (KGAPPINFO *)&pKGInfo->appInfo[uWhich];

   /* Check that generation password is valid */
   if(!isPasswordValid(szPassword, uWhich, pKGInfo))
      {
      sprintf(szResult, "Incorrect password for %s", appInfo->szAppName);
      return(RK_FAILURE);
      }

   /* Get copy of generation code */
   strcpy(szGenCode, appInfo->szGenerationCode);

   /* Unscramble generation code, if necessary */
   if(appInfo->bPasswordNeeded)
      {
      xorScrambler(szGenCode, szPassword);
      }
   
   if(bUserEntered != NULL)
      {
      *bUserEntered = !(appInfo->bFileBased);
      }
   
   /* If file-based registration key */
   if(appInfo->bFileBased)
      {
      /* Use the file-base registration key generation function */
      if(RegKeyFileGenerate(szRegString, szGenCode, szRandSeed, szFileName)
         == RK_SUCCESS)
         {
         /* On success, generate registration key information message */
         strcpy(szResult, "Registration key file has been generated.\n\nDo you wish to generate another key for this application?");
         return(RK_SUCCESS);
         }
      else
         {
         /* On failure, generate error message */
         strcpy(szResult, "ERROR - Unable to generate registration key file");
         return(RK_FAILURE);
         }
      }

   /* If user-entered registration key */
   else
      {
      /* Use the registration key string generation function */
      if(RegKeyGenerate(szRegString, szGenCode, szRandSeed, szRegKey)
         == RK_SUCCESS)
         {
         /* On success, generate registration key information message */
         sprintf(szResult, "Registration key is:\n\n        %s\n\nDo you wish to generate another key for this application?", szRegKey);
         if(szKey != NULL)
            {
            strcpy(szKey, szRegKey);
            }
         return(RK_SUCCESS);
         }
      else
         {
         /* On failure, generate error message */
         strcpy(szResult, "ERROR - Unable to generate registration key");
         return(RK_FAILURE);
         }
      }
   }


char hasGenCodeChanged(unsigned short int uWhich,
                                 const char *szOriginalPassword,
                                 const char *szGenCode, const KGINFO *pKGInfo)
   {
   char szOldGenCode[11];
   KGAPPINFO *appInfo;

   /* Validate parameters if operating in test mode */
   ASSERT(szGenCode != NULL);
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Get pointer to KGAPPINFO structure for this application */
   appInfo = (KGAPPINFO *)&pKGInfo->appInfo[uWhich];

   /* Get copy of generation code */
   strcpy(szOldGenCode, appInfo->szGenerationCode);

   /* Unscramble generation code, if necessary */
   if(appInfo->bPasswordNeeded)
      {
      xorScrambler(szOldGenCode, szOriginalPassword);
      }

   /* Return true iff the generation code stored for application is not the */
   /* same as that which was passed. */
   return(strcmp(szOldGenCode, szGenCode) != 0);
   }


void getAppInfo(unsigned short int uWhich, const char *szPassword,
                          KGAPPINFO *appInfo, const KGINFO *pKGInfo)
   {
   /* Validate parameters if operating in test mode */
   ASSERT(szPassword != NULL);
   ASSERT(appInfo != NULL);
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Copy KGAPPINFO structure for this application */
   *appInfo = pKGInfo->appInfo[uWhich];

   /* Unscramble validation and generation codes, if necessary */
   if(appInfo->bPasswordNeeded)
      {
      xorScrambler(appInfo->szGenerationCode, szPassword);
      xorScrambler(appInfo->szValidationCode, szPassword);
      }
   }


RKRETURN updateAppInfo(unsigned short int uWhich,
                                 const char *szOldPassword,
                                 const char *szNewPassword,
                                 KGAPPINFO *appInfo, KGINFO *pKGInfo)
   {
   KGAPPINFO *oldAppInfo;
   char bNewCodes;

   /* Validate parameters if operating in test mode */
   ASSERT(szOldPassword != NULL);
   ASSERT(appInfo != NULL);
   ASSERT(pKGInfo != NULL);
   ASSERT(uWhich < pKGInfo->uNumApps);

   /* Get pointer to original KGAPPINFO structure for this application */
   oldAppInfo = &pKGInfo->appInfo[uWhich];

   /* If any changes have been made to application information */
   if((bNewCodes=hasGenCodeChanged(uWhich, szOldPassword,
                                   appInfo->szGenerationCode, pKGInfo)) != 0 ||
      strcmp(oldAppInfo->szAppName, appInfo->szAppName) != 0 ||
      oldAppInfo->bPasswordNeeded != appInfo->bPasswordNeeded ||
      oldAppInfo->bFileBased != appInfo->bFileBased ||
      strcmp(szOldPassword, szNewPassword) != 0)
      {
      /* Copy application information from working structure into data file */
      *oldAppInfo = *appInfo;

      /* If new validation code must be calculated */
      if(bNewCodes)
         {
         /* Generate validation code from generation code */
         if(RegKeyNewCodeSet(oldAppInfo->szGenerationCode, oldAppInfo->szValidationCode) == RK_FAILURE)
            {
            return(RK_FAILURE);
            }
         }

      /* Deal with password, if required */
      if(oldAppInfo->bPasswordNeeded)
         {
         oldAppInfo->lPasswordCRC = crc32((char *)szNewPassword);
         xorScrambler(oldAppInfo->szGenerationCode, szNewPassword);
         xorScrambler(oldAppInfo->szValidationCode, szNewPassword);
         }

      /* Attempt to rewrite KeyGen data file */
      if(kgWriteFile(pKGInfo) == RK_FAILURE)
         {
         return(RK_FAILURE);
         }
      }

   /* Return with success */
   return(RK_SUCCESS);
   }


void filenameFromString(char *szOut, const char *szIn)
   {
   char chars = 0;
   
   while(*szIn && chars < 8)
      {
      if(isalpha(*szIn))
         {
         chars++;
         *szOut++ = toupper(*szIn);
         }
      ++szIn;
      }
      
   *szOut = '\0';   
      
   if(chars == 0)
      {
      strcpy(szOut, "REGISTER.KEY");
      }
   else
      {
      strcat(szOut, ".KEY");
      }
   }


