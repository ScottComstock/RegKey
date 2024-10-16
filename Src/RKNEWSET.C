/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/



#define BUILDING_REGKEY
#include "regkey.h"
#include "rkintern.h"



/* RegKeyNewCodeSet()                                                        */
/*                                                                           */
/* Generates a registration key validation code corresponding to a           */
/* generation code. This set of generation and validation codes is unique    */
/* for each application using RegKey, and determines the unique registration */
/* key that corresponds to a particular user's name. The secret generation   */
/* code is used at registration key generation time, and the corresponding   */
/* validation code is used within your application when validating a         */
/* registration key. The validation and generation codes are each            */
/* represented as a ten-digit strings of numbers and upper-case letters.     */
/*                                                                           */
/* This function is called by KeyGen or your own utility, and is only used   */
/* once for each application using RegKey.                                   */
/*                                                                           */
/* szGenerationCode    INPUT: Ten digit generation code                      */
/* szValidateCode      OUTPUT: Ten digit validation code                     */

RKFUNCDEF RKRETURN RKCALL RegKeyNewCodeSet(
   CONST char *szGenerationCode,
   char *szValidationCode)
{
   BIGINT bGenCode;
   BIGINT bValCode;
   char szWork[21];
   char *cpFrom;
   char *cpTo;
   int iCount;
   
   /* Validate Parameters, returning RK_FAILURE if NULL */
   if(!szGenerationCode || !szValidationCode)
   {
      return(RK_FAILURE);
   }

   /* Ensure that generation code is between 1 and 10 digits in length */
   cpTo = (char *)szGenerationCode;
   for(iCount = 0; iCount <= 10 && *cpTo++;) ++iCount;
   if(iCount > 10 || iCount < 1)
   {
      return(RK_FAILURE);
   }

   /* Initialize modulus information, if not already done */
   initMod();

   /* Convert generation code from string representation to a BIGINT */
   stringToB(&bGenCode, szGenerationCode);

   /* Ensure that 0 <= bGenCode < bRegKeyMod */
   bMod(&bGenCode, regKeyModInfoGet());

   /* bValCode = bRegKeyBase ^ bGenCode (mod bRegKeyMod) */
   bExpModN(&bValCode, bRegKeyBaseGet(), &bGenCode, regKeyModInfoGet());

   /* Convert validation code to string representation */
   bToString((char *)&szWork, &bValCode);

   /* Return lower ten digits of string (higher digits will always be zero) */
   cpFrom = (char *)&szWork + 10;
   cpTo = (char *)szValidationCode;
   while(*cpFrom)
   {
      *cpTo++=*cpFrom++;
   }
   *cpTo = '\0';

   /* Return with success */
   return(RK_SUCCESS);
}
