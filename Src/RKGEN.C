/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : rkgen.c

     VERSION : 3.20

     PURPOSE : Source code for the RegKeyGenerate() function

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/


#define BUILDING_REGKEY
#include "regkey.h"
#include "rkintern.h"



/* RegKeyGenerate()                                                          */
/*                                                                           */
/* Generates a registration key for a particular user, using the secret      */
/* generation code corresponding to a particular application (as passed to   */
/* RegKeyNewCodeSet()). The registration string is usually the name of the   */
/* registered user, but may also contain other information, such as the      */
/* version registered or date of expiry. The registration string may be zero */
/* to 64K characters in length, and is null-terminated. The registration key */
/* is returned as a string of letters and upper-case numbers. The string     */
/* pointed to by szRegKey must be large enough to hold 20 digits, plus a     */
/* string terminator character. szRandomSeed should contain 10 random        */
/* numbers and upper-case letters, which are required during the             */
/* registration key generation process.                                      */
/*                                                                           */
/* This function is called by KeyGen or your own registration key generation */
/* utility, each time a registration key is generated for a new user. This   */
/* function is used for user-entered registration keys; compare with         */
/* RegKeyFileGenerate().                                                     */
/*                                                                           */
/* szRegString         INPUT: Registration string                            */
/* szGenerationCode    INPUT: App's generation code                          */
/* szRandomSeed        INPUT: Random number seed                             */
/* szRegKey            OUTPUT: 20-digit registration key                     */

RKFUNCDEF RKRETURN RKCALL RegKeyGenerate(
   CONST char *szRegString,
   CONST char *szGenerationCode,
   CONST char *szRandomSeed,
   char *szRegKey)
{
   BIGINT bRandom;
   BIGINT bR;
   BIGINT bS;
   BIGINT bRegKey;
   BIGINT bB;
   BIGINT bTemp;
   BIGINT bGenCode;

   /* Validate Parameters */
   if(!szRegString || !szGenerationCode || !szRandomSeed
      || !szRegKey)
   {
      return(RK_FAILURE);
   }

   /* Initialize modulus information, if not already done */
   initMod();

   /* Convert generation code from string to BIGINT format */
   stringToB(&bGenCode, szGenerationCode);

   /* Ensure that 0 <= bGenCode < bRegKeyMod */
   bMod(&bGenCode, regKeyModInfoGet());

   /* Convert random seed from string to BIGINT format */
   stringToB(&bRandom, szRandomSeed);
   bMod(&bRandom, regKeyModInfoGet());

   /* Calculate hash code of registration string */
   bHash(&bB, szRegString, regKeyModInfoGet());

   /* Find a bRandom such that gcd(bRandom, bRegKeyMod - 1) == 1 */
   for(;;)
   {
      /* If gcd(bRandom, bRegKeyMod - 1) == 1, then finished loop */
      bGCD(&bTemp, &bRandom, bModMinusOneGet());
      if(bEqual(&bTemp, bOneGet()))
      {
         break;
      }

      /* Otherwise, if this bRandom is unsuitable, try next value */
      bIncrement(&bRandom);
      if(bGreaterOrEqual(&bRandom, bRegKeyModGet()))
      {
         bAssignWord(&bRandom, 0);
      }
   }

   /* Calculate bR portion of registration key to be                         */
   /* bR = bRegKeyBase ^ bRandom (mod bRegKeyMod)                            */
   bExpModN(&bR, bRegKeyBaseGet(), &bRandom, regKeyModInfoGet());

   /* Solve the modular linear equation:                                     */
   /*                                                                        */
   /*  hash(szRegString) = (bGenCode * bR)+(bRandom * bS) (mod bRegKeyMod-1) */
   /*                                                                        */
   /* in order to determine the bS portion of the registration key.          */
   bMult(&bGenCode, &bR);
   bMod(&bGenCode, modMinusOneInfoGet());
   bSub(&bB, &bGenCode);
   bSignedMod(&bB, modMinusOneInfoGet());
   bLinearSolve(&bRandom, &bB, &bS, bModMinusOneGet(), modMinusOneInfoGet());

   /* Form registration key from (bR, bS) */
   bRegKey.iWord[5] = bR.iWord[2];
   bRegKey.iWord[4] = bR.iWord[1];
   bRegKey.iWord[3] = bR.iWord[0];
   bRegKey.iWord[2] = bS.iWord[2];
   bRegKey.iWord[1] = bS.iWord[1];
   bRegKey.iWord[0] = bS.iWord[0];
   bToString(szRegKey, &bRegKey);

   /* Return with success */
   return(RK_SUCCESS);
}
