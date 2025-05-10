/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : rkfile.c

     VERSION : 3.20

     PURPOSE : Source code for the file I/O routines used by RegKey

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/

#define BUILDING_REGKEY
#include "regkey.h"
#include "rkintern.h"

#ifdef _WIN32
#include <string.h>
#include <stdio.h>
#else
#ifndef RKASM
#include <stdio.h>
#ifdef _MSC_VER
#include <dos.h>
#else
#include <dir.h>
#endif
#endif
#endif


/* fileGet()                                                                 */
/*                                                                           */
/* Reads the contents of a registration key file. Finds the first file       */
/* matching the specified filename, opens the file, reads the registration   */
/* key, then reads up to the maximum number of characters of the             */
/* registration string from the file, and closes the file. The registration  */
/* string and key are null-terminated. The registration string should be at  */
/* least pcbSize+1 chars in size. Returns TRUE on success or FALSE on        */
/* failure.                                                                  */
/*                                                                           */
/* szFilename  Name of file to read, wildcards optional                      */
/* pContents   Pointer to buffer where file contents should be stored        */
/* pcbSize     Pointer to an int where maximum size of buffer is stored on   */
/*             calling, and where actual number of chars read is returned    */

BOOL fileGet(CONST char *szFilename, char *pKey, char *pContents,
             int *pcbSize)
{
   DIRENTRY dirEntry;
   char szActualFilename[MAX_PATH_CHARS+MAX_FILENAME_CHARS+1];
   CONST char *cpFrom;
   char *cpTo;
   short int iCount;

   /* Find first filename matching the szFilename specification */
   if(fileFindFirst(szFilename, &dirEntry, DE_NORMAL))
   {
      return(FALSE);
   }

   /* Copy at most MAX_PATH_CHARS from szFilename to szActualFilename */
   cpFrom = szFilename;
   cpTo = szActualFilename;
   iCount = 0;
   while((iCount++ < (MAX_PATH_CHARS+1)) && ((*cpTo++ = *cpFrom++) != 0));
   *cpTo = '\0';
   if(iCount >= (MAX_PATH_CHARS+1))
   {
      return(FALSE);
   }

   /* Trim szActualFilename to last '/' or '\\' characters */
   while(iCount--)
   {
      --cpTo;
      if(*cpTo == '\\' || *cpTo == '/')
      {
         ++cpTo;
         break;
      }
      *cpTo = '\0';
   }

   /* Append filename from directory entry to szActualFilename */
   cpFrom = (char *)dirEntry.name;
   while((*cpTo++ = *cpFrom++) != 0);

#if defined(RKASM) && !defined(_WIN32)
   {
      int handle;

      /* Save affected registers */
      ASM   PUSH    DS
      ASM   PUSH    ES
      ASM   PUSH    SI
      ASM   PUSH    DI

      /* Open file with handle for read */
      ASM   MOV     AX, SS
      ASM   MOV     DS, AX
      ASM   LEA     DX, szActualFilename
      ASM   MOV     AX, 0x3d00
      ASM   INT     0x21
      ASM   JC      return_error
      ASM   MOV     handle, AX

      /* Read key from file with handle */
      ASM   MOV     AH, 0x3f
      ASM   MOV     BX, handle
      ASM   MOV     CX, 20
      ASM   LDS     DX, pKey
      ASM   INT     0x21
      ASM   JC      close_error
      ASM   CMP     CX, 20
      ASM   JNE     close_error

      /* Read registration string from file with handle */
      ASM   MOV     AH, 0x3f
      ASM   MOV     BX, handle
      ASM   LDS     SI, pcbSize
      ASM   MOV     CX, DS:[SI]
      ASM   LDS     DX, pContents
      ASM   INT     0x21
      ASM   JC      close_error
      ASM   LDS     SI, pcbSize
      ASM   MOV     DS:[SI], AX

      /* Close file */
      ASM   MOV     AH, 0x3e
      ASM   MOV     BX, handle
      ASM   INT     0x21

      ASM   JMP     return_success

close_error:
      /* Close file */
      ASM   MOV     AH, 0x3e
      ASM   MOV     BX, handle
      ASM   INT     0x21

      /* Fallthrough ... */

return_error:
      /* Restore affected registers */
      ASM   POP     DI
      ASM   POP     SI
      ASM   POP     ES
      ASM   POP     DS

      /* Return with failure */
      return(FALSE);

return_success:
      /* Restore affected registers */
      ASM   POP     DI
      ASM   POP     SI
      ASM   POP     ES
      ASM   POP     DS

      /* Zero-terminate the buffers */
      pKey[20] = '\0';
      pContents[*pcbSize] = '\0';

      /* Return with success */
      return(TRUE);
   }
#else
   {
      FILE *fp;

      /* Open file */
      if((fp=fopen(szActualFilename,"r")) == NULL)
      {
         return(FALSE);
      }

      /* Read key from file */
      if(fread(pKey, sizeof(char), 20, fp) != 20)
      {
         fclose(fp);
         return(FALSE);
      }
      pKey[20] = '\0';

      /* Fill buffer from file */
      *pcbSize = fread(pContents, sizeof(char), *pcbSize, fp);

      /* Null-terminate the buffer */
      pContents[*pcbSize] = '\0';

      /* close file */
      fclose(fp);

      return(TRUE);
   }
#endif
}



/* filePut()                                                                 */
/*                                                                           */
/* Writes to a registration key file, overwriting any existing file with the */
/* specified filename. Opens the file for create/truncation, writes the      */
/* registration key and registration string to the file, and closes the      */
/* file. Returns TRUE on success or FALSE on failure.                        */
/*                                                                           */
/* szFilename  Name of file to read, wildcards optional                      */
/* pContents   Pointer to buffer where file contents should be stored        */
/* cbSize      An int containing the number of bytes to be written to file   */

BOOL filePut(CONST char *szFilename, CONST char *pKey,
             CONST char *pContents, int cbSize)
{
#if defined(RKASM) && !defined(_WIN32)
   {
      int handle;

      /* Save affected registers */
      ASM   PUSH    DS
      ASM   PUSH    ES
      ASM   PUSH    SI
      ASM   PUSH    DI

      /* Create file with handle */
      ASM   LDS     DX, szFilename
      ASM   MOV     AH, 0x3c
      ASM   MOV     CX, 0x00
      ASM   INT     0x21
      ASM   JC      return_error
      ASM   MOV     handle, AX

      /* Write key to file with handle */
      ASM   MOV     AH, 0x40
      ASM   MOV     BX, handle
      ASM   MOV     CX, 20
      ASM   LDS     DX, pKey
      ASM   INT     0x21
      ASM   JC      close_error
      ASM   CMP     AX, 20
      ASM   JNE     close_error

      /* Write registration key to file with handle */
      ASM   MOV     AH, 0x40
      ASM   MOV     BX, handle
      ASM   MOV     CX, cbSize
      ASM   LDS     DX, pContents
      ASM   INT     0x21
      ASM   JC      close_error
      ASM   CMP     AX, cbSize
      ASM   JNE     close_error

      /* Close file */
      ASM   MOV     AH, 0x3e
      ASM   MOV     BX, handle
      ASM   INT     0x21

      ASM   JMP     return_success

close_error:
      /* Close file */
      ASM   MOV     AH, 0x3e
      ASM   MOV     BX, handle
      ASM   INT     0x21

      /* Fallthrough ... */

return_error:
      /* Restore affected registers */
      ASM   POP     DI
      ASM   POP     SI
      ASM   POP     ES
      ASM   POP     DS

      /* Return with failure */
      return(FALSE);

return_success:
      /* Restore affected registers */
      ASM   POP     DI
      ASM   POP     SI
      ASM   POP     ES
      ASM   POP     DS

      /* Return with success */
      return(TRUE);
   }
#else
   {
      FILE *fp;

      /* Open file */
      if((fp=fopen(szFilename,"w")) == NULL)
      {
         return(FALSE);
      }

      /* Write key to file */
      if(fwrite(pKey, sizeof(char), 20, fp) != 20)
      {
         fclose(fp);
         return(FALSE);
      }

      /* Fill file from buffer */
      if(fwrite(pContents, sizeof(char), cbSize, fp) != (size_t)cbSize)
      {
         fclose(fp);
         return(FALSE);
      }

      /* close file */
      fclose(fp);

      return(TRUE);
   }
#endif
}



/* fileFindFirst()                                                           */
/*                                                                           */
/* Finds the directory entry (DIRENTRY) of the first file that matches       */
/* szPath and iAttrib. Returns directory entry in pDirEntry. Returns 0 on    */
/* success, non-zero on failure.                                             */
/*                                                                           */
/* szFilename  Name of file to read, wildcards optional                      */
/* pContents   Pointer to buffer where file contents should be stored        */
/* cbSize      An int containing the number of bytes to be written to file   */

int fileFindFirst(CONST char *szPath, DIRENTRY *pDirEntry, int iAttrib)
{
#ifdef _WIN32
   WIN32_FIND_DATA Win32FindData;
   HANDLE hFileFind = FindFirstFile(szPath, &Win32FindData);

   if(hFileFind == NULL)
   {
      return(1);
   }

   if(strlen(Win32FindData.cAlternateFileName) > 0)
   {
      strcpy(pDirEntry->name, Win32FindData.cAlternateFileName);
   }
   else
   {
      strcpy(pDirEntry->name, Win32FindData.cFileName);
   }

   FindClose(hFileFind);  

   return(0);

#else
#ifdef RKASM
   int to_return;

   /* Save DS */
   ASM      PUSH    DS

   /* Call int 0x21, ah=0x2f: Get current DOS DTA */
   ASM      MOV     AH, 0x2f
   ASM      INT     0x21

   /* Store segment and offset of current DTA on stack */
   ASM      PUSH    BX
   ASM      PUSH    ES

   /* Call int 0x21, ah=0x1a: Set new DOS DTA to far address of pDirEntry */
   ASM      MOV     AH, 0x1a
   ASM      LDS     DX, pDirEntry
   ASM      INT     0x21

   /* Call int 0x21, ah=0x4e, DOS findfirst function */
   ASM      MOV     AH, 0x4e
   ASM      MOV     CX, iAttrib
   ASM      LDS     DX, szPath
   ASM      INT     0x21
   ASM      JC      error
   ASM      MOV     to_return, 0
   ASM      JMP     after_result
error:
   ASM      MOV     to_return, -1
after_result:

   /* Call int 0x21, ah=0x1a to restore original DOS DTA */
   ASM      MOV     AH, 0x1a
   ASM      POP     DS
   ASM      POP     DX
   ASM      INT     0x21

   /* Restore DS as saved at function startup */
   ASM      POP     DS

   return(to_return);

#else
#ifdef _MSC_VER
   /* Microsoft C interface to DOS findfirst function */
   return(_dos_findfirst(szPath, (unsigned)iAttrib, (struct _find_t *)pDirEntry));
#else
   /* Borland C interface to DOS findfirst function */
   return(findfirst(szPath, (struct ffblk *)pDirEntry, iAttrib));
#endif
#endif
#endif
}
