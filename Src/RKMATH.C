/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : rkmath.c

     VERSION : 3.20

     PURPOSE : Source code for the 96-bit math routines used by RegKey

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/


#define BUILDING_REGKEY
#include "regkey.h"
#include "rkintern.h"



/* Multibyte integer CONSTants used throughout the RegKey system             */
/* Each CONSTant has an accessor function which returns a pointer to that    */
/* Constant                                                                  */

/* Constant BIGINT representing the value 1 */

static BIGINT bOne = {0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
BIGINT *bOneGet(void) {return(&bOne);}



/* Constant BIGINT containing modulus used by RegKey                         */

static BIGINT bRegKeyMod = {0xd525, 0x655f, 0x62a2, 0x0000, 0x0000, 0x0000};
BIGINT *bRegKeyModGet(void) {return(&bRegKeyMod);}
static MODINFO regKeyModInfo;
MODINFO *regKeyModInfoGet(void) {return(&regKeyModInfo);}
static BOOL bModInited = FALSE;
BOOL *bModInitedGet(void) {return(&bModInited);}



/* Constant BIGINT containing modulus - 1                                    */

static BIGINT bModMinusOne = {0xd524, 0x655f, 0x62a2, 0x0000, 0x0000, 0x0000};
BIGINT *bModMinusOneGet(void) {return(&bModMinusOne);}
static MODINFO modMinusOneInfo;
MODINFO *modMinusOneInfoGet(void) {return(&modMinusOneInfo);}



/* Constant BIGINT containing base used by RegKey                            */
/*                                                                           */
/* bRegKeyBase should be a primitive root of the modulus bRegKeyMod.         */
/* i.e. There should be some A such that bRegKeyBase^A (mod bRegKeyMod) = X  */
/* for any X where 0 <= X < bRegKeyMod.                                      */

static BIGINT bRegKeyBase = {0x9783, 0xcf08, 0x10b5, 0x0000, 0x0000, 0x0000};
BIGINT *bRegKeyBaseGet(void) {return(&bRegKeyBase);}

/* Array containing characters used in RegKey's base-32 representation. Uses */
/* all numeric characters, and all but four upper-case letters. The omitted  */
/* letters are G, I, O and S, which can easily be confused for the numerals  */
/* 6, 1, 0 and 5.                                                            */

static char base32chars[32] = {
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
   'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q',
   'R', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'  };



/* Array containing the value of each character in RegKey's base-32          */
/* representation.                                                           */

static WORD base32vals[256] = {
/*         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */

/* 0 */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 1 */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 2 */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 3 */    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
/* 4 */    0, 10,11,12,13,14,15,6, 16,1, 17,18,19,20,21,0,
/* 5 */    22,23,24,5, 25,26,27,28,29,30,31,0, 0, 0, 0, 0,
/* 6 */    0, 10,11,12,13,14,15,6, 16,1, 17,18,19,20,21,0,
/* 7 */    22,23,24,5, 25,26,27,28,29,30,31,0, 0, 0, 0, 0,
/* 8 */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 9 */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* A */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* B */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* C */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* D */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* E */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* F */    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  };



/* bInitMod()                                                                */
/*                                                                           */
/* Initializes the RegKey modulus information, if not already done.          */

void initMod(void)
{
   /* If initialization has already been done, return immediately */
   if(bModInited)
   {
      return;
   }

   /* Calculate modulus information for RegKey's modulus */
   genModInfo(&regKeyModInfo, &bRegKeyMod);

   /* Calculate modulus information for modulus - 1 */
   genModInfo(&modMinusOneInfo, &bModMinusOne);

   /* Indicate that initialization has now been done. */
   bModInited = TRUE;

   /* Use bRegKeyBase in this module so that compiler doesn't complain */
   bRegKeyBase.iWord[5] = 0;
}



/* bToString()                                                               */
/*                                                                           */
/* Converts the BIGINT pointed to by pbIn to a string in RegKey's base-32    */
/* representation. String is left padded with 0's to always be exactly 20    */
/* characters in length.                                                     */
/*                                                                           */
/* szOut       Pointer to string large enough for 20 characters + null byte  */
/* pbIn        Pointer to BIGINT which is to be converted                    */

void bToString(char *szOut, CONST BIGINT *pbIn)
{
   char *cp;
   signed short int iCount;
   BIGINT bWork;

   /* Validate parameters if operating in test mode */
   ASSERT(szOut != NULL);
   ASSERT(pbIn != NULL);

   /* Get a working copy of the BIGINT to be converted */
   bAssign(&bWork, pbIn);

   /* Set pointer to end of string, where null terminator will be placed */
   cp = szOut + 20;

   /* Place null terminator in string and move pointer to previous character */
   *cp-- = '\0';

   /* Loop for each digit to be placed in output string */
   for(iCount = 20; iCount--; )
   {
      /* Sets right-most remaining digit to appropriate value from */
      /* base32chars translation table */
      *cp-- = base32chars[bGetLowOrderWord(&bWork) & 0x1f];

      /* Shift the remaining bits of the BIGINT right 5 bits */
      bShiftRight(&bWork, 5);
   }
}

/* stringToB()                                                               */
/*                                                                           */
/* Converts the the string pointed to by szIn, containing a number in        */
/* RegKey's base-32 representation, to a BIGINT. Case is not significant,    */
/* and the letters G, I, O and S are treated as the numerials 6, 1, 0 and 5. */
/*                                                                           */
/* pbOut       Pointer to BIGINT where result is to be stored                */
/* szIn        Pointer to null-terminated string that is to be converted     */

void stringToB(BIGINT *pbOut, CONST char *szIn)
{
   signed short int iCount = 0;

   /* Validate parameters if operating in test mode */
   ASSERT(pbOut != NULL);
   ASSERT(szIn != NULL);

   /* Set destination BIGINT to 0 */
   bAssignWord(pbOut, 0);

   /* Skip any leading spaces in input string */
   while(*szIn == ' ') ++szIn;

   /* While not at the end of the input string and have not read more than */
   /* 20 characters */
   while(*szIn && iCount++ < 20)
   {
      /* Shift the current bits of the BIGINT left 5 bits */
      bShiftLeft(pbOut, 5);

      /* Place the five bits of the value corresponding to the next */
      /* character from szIn into the lower five bits of pbOut */
      bOrLowOrderWordWith(pbOut, base32vals[*szIn++]);
   }
}



/* bShiftLeft()                                                              */
/*                                                                           */
/* Shifts a BIGINT up to WORD_SIZE bits to the left.                         */
/*                                                                           */
/* pbX         Pointer to the BIGINT to be shifted                           */
/* wDistance   Number of bits to shift the BIGINT                            */

void bShiftLeft(BIGINT *pbX, WORD wDistance)
{
   signed short int iCount;
   WORD wCarrySize;
   WORD wCarry;
   WORD wNewVal;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(wDistance <= WORD_SIZE);

   /* Initially, carry is 0 */
   wCarry = 0;

   /* Determine number of bits to shift to generate carry */
   wCarrySize = WORD_SIZE - wDistance;

   /* Loop for each word in the BIGINT */
   for(iCount = 0; iCount < BIGINT_WORDS; ++iCount)
   {
      wNewVal = (pbX->iWord[iCount] << wDistance) | wCarry;
      wCarry = pbX->iWord[iCount] >> wCarrySize;
      pbX->iWord[iCount] = wNewVal;
   }
}



/* bEqual()                                                                  */
/*                                                                           */
/* Returns TRUE iff pbX == pbY                                               */
/*                                                                           */
/* pbX         Pointer to the BIGINT representing the first operand          */
/* pbY         Pointer to the BIGINT representing the second operand         */

BOOL bEqual(CONST BIGINT *pbX, CONST BIGINT *pbY)
{
   WORD *pwX = (WORD *)&pbX->iWord[BIGINT_WORDS-1];
   WORD *pwY = (WORD *)&pbY->iWord[BIGINT_WORDS-1];
   unsigned char bCounter = BIGINT_WORDS;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

   /* Loop through the words forming the BIGINTs, left to right */
   while(bCounter--)
   {
      /* If the two BIGINTs differ in this word */
      if(*pwX != *pwY)
      {
         /* Then entire BIGINTs cannot be different */
         return(FALSE);
      }
      /* Move each pointer to the previous word */
      --pwX;
      --pwY;
   }

   /* If two BIGINTs compare equal in all words, return TRUE */
   return(TRUE);
}



/* bShiftRight()                                                             */
/*                                                                           */
/* Shifts a BIGINT up to WORD_SIZE bits to the right.                        */
/*                                                                           */
/* pbX         Pointer to the BIGINT to be shifted                           */
/* wDistance   Number of bits to shift the BIGINT                            */

void bShiftRight(BIGINT *pbX, WORD wDistance)
{
   signed short int iCount;
   WORD wCarrySize;
   WORD wCarry;
   WORD wNewVal;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(wDistance <= WORD_SIZE);

   /* Initially, carry is 0 */
   wCarry = 0;

   /* Determine number of bits to shift to generate carry */
   wCarrySize = WORD_SIZE - wDistance;

   /* Loop for each word in the BIGINT */
   for(iCount = BIGINT_WORDS - 1; iCount >= 0; --iCount)
   {
      wNewVal = (pbX->iWord[iCount] >> wDistance) | wCarry;
      wCarry = pbX->iWord[iCount] << wCarrySize;
      pbX->iWord[iCount] = wNewVal;
   }
}



/* bShiftRightOne()                                                          */
/*                                                                           */
/* Shifts a BIGINT one bit to the right.                                     */
/*                                                                           */
/* pbX         Pointer to the BIGINT to be shifted                           */

void bShiftRightOne(BIGINT *pbX)
{
   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);

/* If inline 80x86 assembly language is available */
#ifdef RKASM
   /* Save register contents */
   ASM      PUSH    DS
   ASM      PUSH    SI

   /* Load DS:SI with address of highest order by of pbX */
   ASM      LDS     SI, pbX
   ASM      ADD     SI, (BIGINT_WORDS - 1) * 2;

   /* Set CX to number of words forming a BIGINT */
   ASM      MOV     CX, BIGINT_WORDS

   /* Clear carry flag for shift */
   ASM      CLC

word_loop:
   /* Shift next word of pbX, with carry propogation */
   ASM      RCR     word ptr DS:[SI], 1

   /* Decrement pointer to previous word */
   /* (INC is used in order that CF is preserved) */
   ASM      DEC     SI
   ASM      DEC     SI

   /* Decrement word count (in CX), looping until 0 */
   ASM      LOOP    word_loop

   /* Restore register contents */
   ASM      POP     SI
   ASM      POP     DS

/* If inline 80x86 assembly language is not available */
#else /* !defined(RKASM) */
   bShiftRight(pbX, 1);
#endif /* !defined(RKASM) */
}



/* bAdd()                                                                    */
/*                                                                           */
/* Calculates pbX = pbX + pbY                                                */
/*                                                                           */
/* pbX         Pointer to BIGINT in which first operand & result are stored  */
/* pbY         Pointer to BIGINT in which second operand is stored           */

void bAdd(BIGINT *pbX, CONST BIGINT *pbY)
{
   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

/* If inline 80x86 assembly language is available */
#ifdef RKASM
   /* Save register contents */
   ASM      PUSH    DS
   ASM      PUSH    ES
   ASM      PUSH    SI
   ASM      PUSH    DI

   /* Load ES:DI with address of lowest order byte of pbX */
   ASM      LES     DI, pbX

   /* Load DS:SI with address of lowest order byte of pbY */
   ASM      LDS     SI, pbY

   /* Set CX to number of words forming a BIGINT */
   ASM      MOV     CX, BIGINT_WORDS

   /* Clear carry flag for addition */
   ASM      CLC

word_loop:
   /* Load AX with next word from pbY */
   ASM      MOV     AX, word ptr DS:[SI]

   /* Add it to next word of pbX, with carry propogation */
   ASM      ADC     word ptr ES:[DI], AX

   /* Increment pointers to next words to add */
   /* (INC is used in order that CF is preserved) */
   ASM      INC     SI
   ASM      INC     SI
   ASM      INC     DI
   ASM      INC     DI

   /* Decrement word count (in CX), looping until 0 */
   ASM      LOOP    word_loop

   /* Restore register contents */
   ASM      POP     DI
   ASM      POP     SI
   ASM      POP     ES
   ASM      POP     DS

/* If inline 80x86 assembly language is not available */
#else /* !defined(RKASM) */
   {
      short int iCount;
      WORD *pwX;
      WORD *pwY;
      WORD wSum;
      WORD wCarry = 0;

      /* Set pointers to first word of each operand BIGINT */
      pwX = (WORD *)pbX;
      pwY = (WORD *)pbY;

      /* Loop for each word forming a BIGINT */
      iCount = BIGINT_WORDS;
      while(iCount--)
      {
         /* Calculate sum of all bits of word other than upper most bit */
         wSum = (*pwX & ~WORD_UPPER_BIT_MASK) + (*pwY & ~WORD_UPPER_BIT_MASK) + wCarry;

         /* Calculate upper two bits of addition result */
         wCarry = 0;
         if(wSum & WORD_UPPER_BIT_MASK) wCarry++;
         if(*pwX & WORD_UPPER_BIT_MASK) wCarry++;
         if(*pwY & WORD_UPPER_BIT_MASK) wCarry++;

         /* Calculate upper bit of this word, placing result in *pwX */
         *pwX = (wCarry & 0x0001) ? wSum | WORD_UPPER_BIT_MASK : wSum & ~WORD_UPPER_BIT_MASK;

         /* Calculate actual carry for next word */
         wCarry >>= 1;

         /* Increment word pointers */
         ++pwX;
         ++pwY;
      }
   }
#endif /* !defined(RKASM) */
}



/* bSub()                                                                    */
/*                                                                           */
/* Calculates pbX = pbX - pbY                                                */
/*                                                                           */
/* pbX         Pointer to BIGINT in which first operand & result are stored  */
/* pbY         Pointer to BIGINT in which second operand is stored           */

void bSub(BIGINT *pbX, CONST BIGINT *pbY)
{
   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

/* If inline 80x86 assembly language is available */
#ifdef RKASM
   /* Save register contents */
   ASM      PUSH    DS
   ASM      PUSH    ES
   ASM      PUSH    SI
   ASM      PUSH    DI

   /* Load ES:DI with address of lowest order byte of pbX */
   ASM      LES     DI, pbX

   /* Load DS:SI with address of lowest order byte of pbY */
   ASM      LDS     SI, pbY

   /* Set CX to number of words forming a BIGINT */
   ASM      MOV     CX, BIGINT_WORDS

   /* Clear carry flag for subtraction */
   ASM      CLC

word_loop:
   /* Load AX with next word from pbY */
   ASM      MOV     AX, word ptr DS:[SI]

   /* Subtract it from the next word of pbX, with borrow propogation */
   ASM      SBB     word ptr ES:[DI], AX

   /* Increment pointers to next words to add */
   /* (INC is used in order that CF is preserved) */
   ASM      INC     SI
   ASM      INC     SI
   ASM      INC     DI
   ASM      INC     DI

   /* Decrement word count (in CX), looping until 0 */
   ASM      LOOP    word_loop

   /* Restore register contents */
   ASM      POP     DI
   ASM      POP     SI
   ASM      POP     ES
   ASM      POP     DS

/* If inline 80x86 assembly language is not available */
#else /* !defined(RKASM) */
   {
      BIGINT bYNegated;

      /* Perform subtraction by adding first operand to negation of second */
      bAssign(&bYNegated, pbY);
      bNegate(&bYNegated);
      bAdd(pbX, &bYNegated);
   }
#endif /* !defined(RKASM) */
}



/* bSignedMult()                                                             */
/*                                                                           */
/* Multiplies two signed BIGINTs. Calculates pbX *= pbY. pbX and pbY may be  */
/* no larger than 2^(BIGINT_HALF*WORD_SIZE).                                 */
/*                                                                           */
/* pbX         Pointer to the BIGINT where the multiplicand is stored prior  */
/*             to multiplication, and where the product is stored afterwards */
/* pbY         Pointer to the BIGINT where the multiplier is stored          */

void bSignedMult(BIGINT *pbX, CONST BIGINT *pbY)
{
   BOOL bNegateResult = FALSE;
   BIGINT bY;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);
   ASSERT(bFitsInHalfOfBigint(pbX));
   ASSERT(bFitsInHalfOfBigint(pbY));

   /* Copy CONSTant parameter into working storage */
   bAssign(&bY, pbY);

   /* If pbX is negative, convert to positive */
   if(bNegative(pbX))
   {
      bNegateResult = TRUE;
      bNegate(pbX);
   }

   /* If bY is negative, convert to positive */
   if(bNegative(&bY))
   {
      bNegateResult = !bNegateResult;
      bNegate(&bY);
}

   /* Perform unsigned integer multiplication */
   bMult(pbX, &bY);

   /* If result should be negated, then do so */
   if(bNegateResult)
   {
      bNegate(pbX);
   }
}



/* bMult()                                                                   */
/*                                                                           */
/* Multiplies two positive BIGINTs. Calculates pbX *= pbY. pbX and pbY may   */
/* be no larger than 2^(BIGINT_HALF*WORD_SIZE).                              */
/*                                                                           */
/* pbX         Pointer to the BIGINT where the multiplicand is stored prior  */
/*             to multiplication, and where the product is stored afterwards */
/* pbY         Pointer to the BIGINT where the multiplier is stored          */

void bMult(BIGINT *pbX, CONST BIGINT *pbY)
{
/* If inline 80x86 assembly language is available */
#ifdef RKASM
   BIGINT bProd;
   BIGINT bA;
   BIGINT bB;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);
   ASSERT(bFitsInHalfOfBigint(pbX));
   ASSERT(bFitsInHalfOfBigint(pbY));

   /* Move operands into temporary variables */
   bAssign(&bA, pbX);
   bAssign(&bB, pbY);

   /* Initialize product to 0 */
   bAssignWord(&bProd, 0);

   /* Save contents of needed registers */
   ASM      PUSH    ES
   ASM      PUSH    DS
   ASM      PUSH    DI
   ASM      PUSH    SI

   /* Load DS and ES with SS address */
   ASM      MOV     AX, SS
   ASM      MOV     DS, AX
   ASM      MOV     ES, AX

   /* Clear CL (b_loop counter) */
   ASM      XOR     CL, CL

   /* Loop for each digit in multiplier */
b_loop:
   /* Clear CH (a_loop counter) */
   ASM      XOR     CH, CH

   /* Calculate AX = (b_loop counter) * 2 (note that CH is currently clear) */
   ASM      MOV     AX, CX
   ASM      SHL     AX, 1

   /* Set DI = &bProd[b_loop counter] */
   ASM      LEA     DI, bProd
   ASM      ADD     DI, AX

   /* Set BX = bB[b_loop counter] (current word of multiplier) */
   ASM      LEA     SI, bB
   ASM      ADD     SI, AX
   ASM      MOV     BX, word ptr DS:[SI]

   /* Set SI = &bA */
   ASM      LEA     SI, bA

   /* Loop for each digit in multiplicand */
a_loop:
   /* Load AX with next word of multiplicand */
   ASM      MOV     AX, word ptr DS:[SI]

   /* Multiply it with the current word of the multiplier, result in DX:AX */
   ASM      MUL     BX

   /* Add AX to current word of overall product */
   ASM      ADD     word ptr ES:[DI], AX

   /* Move to next word of overall product (INC is used to preserve CF) */
   ASM      INC     DI
   ASM      INC     DI

   /* Add DX to next word of overall product, with carry propogation */
   ASM      ADC     word ptr ES:[DI], DX

   /* If no carry resulted from this addition, then there is no need for */
   /* further carry propogation at this time */
   ASM      JNC     no_more_carry

   /* Calcuate AH = (a_loop counter) + (b_loop counter) + 1 */
   /* (note that CF will always be set at this point) */
   ASM      MOV     AH, CH
   ASM      ADC     AH, CL

   /* Save current product pointer in DX */
   ASM      MOV     DX, DI

carry_loop:
   /* Increment product word counter */
   ASM      INC     AH

   /* Stop looping when end of product word has been reached even if */
   /* overflow has resulted */
   ASM      CMP     AH, BIGINT_WORDS
   ASM      JGE     end_carry_loop

   /* Move to next word of product */
   ASM      ADD     DI, 2

   /* Add 1 to this word */
   ASM      ADD     word ptr ES:[DI], 1

   /* Continue looping if there is futher carry propogtion */
   ASM      JC      carry_loop

end_carry_loop:
   /* Restore current product pointer from DX */
   ASM      MOV     DI, DX

no_more_carry:
   /* Move SI to next digit of multiplicand */
   ASM      ADD     SI, 2

   /* Increment a_loop counter (CH) */
   ASM      INC     CH

   /* Loop until all valid words of the multiplicand have been dealt with */
   ASM      CMP     CH, BIGINT_HALF
   ASM      JL      a_loop

   /* Increment b_loop counter */
   ASM      INC     CL

   /* Loop until all valid words of the multiplier have been dealt with */
   ASM      CMP     CL, BIGINT_HALF
   ASM      JL      b_loop

   /* Restore contents of needed registers */
   ASM      POP     SI
   ASM      POP     DI
   ASM      POP     DS
   ASM      POP     ES

   /* Copy product into destination address */
   bAssign(pbX, &bProd);

/* If inline 80x86 assembly language is not available */
#else /* !defined(RKASM) */
   BIGINT bXVal;
   BIGINT bYVal;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);
   ASSERT(bFitsInHalfOfBigint(pbX));
   ASSERT(bFitsInHalfOfBigint(pbY));

   /* Move operands into temporary variables */
   bAssign(&bXVal, pbX);
   bAssign(&bYVal, pbY);

   /* Initialize product to 0 */
   bAssignWord(pbX, 0);

   /* Loop while multiplier is not zero */
   while(!bZero(&bYVal))
   {
      /* If least significant bit of multiplier is set */
      if(bGetLowOrderWord(&bYVal) & 0x0001)
      {
         /* Add multiplicand to product */
         bAdd(pbX, &bXVal);
      }

      /* Shift multiplicand left one bit */
      bShiftLeft(&bXVal, 1);

      /* Shift multiplier right one bit */
      bShiftRight(&bYVal, 1);
   }
#endif /* !defined(RKASM) */
}



/* bMod()                                                                    */
/*                                                                           */
/* Calculates pbX = pbX mod y. The value for y is determined by the modulus  */
/* passed to genModInfo when initializing pModInfo.                          */
/*                                                                           */
/* pbX         Pointer to the BIGINT where the divisor and residue are       */
/*             stored                                                        */
/* pModInfo    Pointer to a MODINFO structure to be initialized              */

void bMod(BIGINT *pbX, CONST MODINFO *pModInfo)
{
   int iIndex;
   BIGINT *pbModMult;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pModInfo != NULL);
   ASSERT(pModInfo->iSize > 0 && pModInfo->iSize < BIGINT_WORDS*WORD_SIZE);

   /* Determine index of largest power of the modulus */
   iIndex = pModInfo->iSize;

   /* Get address of largest power of the modulus */
   pbModMult = ((BIGINT *)&(pModInfo->bModMult)) + (iIndex - 1);

   /* Loop for each power of the modulus */
   while(iIndex--)
   {
      /* If pbX is >= this power of the modulus */
      if(bGreaterOrEqual(pbX, pbModMult))
      {
         /* Then subtract the power of the modulus from pbX */
         bSub(pbX, pbModMult);
      }

      /* Move modulus power pointer to previous power */
      --pbModMult;
   }
}



/* bSignedMod()                                                                    */
/*                                                                           */
/* Calculates pbX = pbX mod y. The value for y is determined by the modulus  */
/* passed to genModInfo when initializing pModInfo. pbX may be a signed      */
/* value.                                                                    */
/*                                                                           */
/* pbX         Pointer to the BIGINT where the divisor and residue are       */
/*             stored                                                        */
/* pModInfo    Pointer to a MODINFO structure to be initialized              */

void bSignedMod(BIGINT *pbX, CONST MODINFO *pModInfo)
{
   BOOL bFixResult;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pModInfo != NULL);
   ASSERT(pModInfo->iSize > 0 && pModInfo->iSize < BIGINT_WORDS*WORD_SIZE);

   if(bNegative(pbX))
   {
      bFixResult = TRUE;
      bNegate(pbX);
   }
   else
   {
      bFixResult = FALSE;
   }

   bMod(pbX, pModInfo);

   if(bFixResult)
   {
      bNegate(pbX);
      bAdd(pbX, &pModInfo->bModMult[0]);
   }
}


/* genModInfo()                                                              */
/*                                                                           */
/* Performs one-time preprocessing for bMod() that is specific to a          */
/* particular modulus. Initializes the contents of a MODINFO structure.      */
/*                                                                           */
/* pModInfo    Pointer to a MODINFO structure to be initialized              */
/* pbMod       Pointer to BIGINT in which the modulus is stored              */

void genModInfo(MODINFO *pModInfo, CONST BIGINT *pbMod)
{
   BIGINT bWork;
   BIGINT *pbMult;
   int iIndex = 0;

   /* Validate parameters if operating in test mode */
   ASSERT(pModInfo != NULL);
   ASSERT(pbMod != NULL);

   /* Test for division by zero in test mode */
   ASSERT(!bZero(pbMod));

   /* Initialize working variables */
   bAssign(&bWork, pbMod);

   /* Loop until all possible powers of the modulus have been calculated */
   for(;;)
   {
      /* Store current multiple of the modulus */
      pbMult=((BIGINT *)&(pModInfo->bModMult))+(iIndex++);
      bAssign(pbMult, &bWork);

      /* If most significant bit of bWork is set */
      if(bWork.iWord[5]&0x8000)
      {
         /* All possible powers have been generated, so exit loop */
         break;
      }

      /* In test mode, ensure that we have not overrun array in pModInfo */
      ASSERT(iIndex < BIGINT_WORDS*WORD_SIZE);

      /* Shift bWork left one bit, to achieve bWork *= 2 */
      bShiftLeft(&bWork, 1);
   }

   /* Record the total number of modulus multiples stored in pModInfo */
   pModInfo->iSize = iIndex;
}



/* bExpModN()                                                                */
/*                                                                           */
/* Calculates x = a^z mod n.                                                 */
/*                                                                           */
/* pbOut       Pointer to a BIGINT where result is to be stored (x)          */
/* pbBase      Pointer to a BIGINT representing the base (a)                 */
/* pbExponent  Pointer to a BIGINT representing the exponent (z)             */
/* pModInfo    Pointer to a MODINFO structure representing the mod (n)       */

void bExpModN(BIGINT *pbOut, CONST BIGINT *pbBase, CONST BIGINT *pbExponent,
              CONST MODINFO *pModInfo)
{
   BIGINT bA;
   BIGINT bZ;

   /* Validate parameters if operating in test mode */
   ASSERT(pbOut != NULL);
   ASSERT(pbBase != NULL);
   ASSERT(pbExponent != NULL);
   ASSERT(pModInfo != NULL);

   /* Initialize bA and bZ from parameters */
   bAssign(&bA, pbBase);
   bAssign(&bZ, pbExponent);

   /* Initialize result to 1 */
   bAssignWord(pbOut, 1);

   /* While exponent is not zero */
   while(!bZero(&bZ))
   {
      /* Square a while exponent is even */
      while(!(bZ.iWord[0]&0x0001))
      {
         /* Divide exponent by 2 (using shift) */
         bShiftRightOne(&bZ);

         /* Calculate a = a^2 (mod n) */
         bMult(&bA, &bA);
         bMod(&bA, pModInfo);
      }

      /* Subtract one from exponent */
      bDecrement(&bZ);

      /* Multiply result by a (mod n) */
      bMult(pbOut, &bA);
      bMod(pbOut, pModInfo);
   }
}



/* bHash()                                                                   */
/*                                                                           */
/* Hash function produces a 48-bit hash value from a string of arbitrary     */
/* length.                                                                   */
/*                                                                           */
/* pbHash      Pointer to BIGINT in which hash value will be stored          */
/* szString    Pointer to string to be hashed                                */
/* pModInfo    Pointer to the MODINFO structure generated for the modulus    */
/*             to be used                                                    */

void bHash(BIGINT *pbHash, CONST char *szString,
           CONST MODINFO *pModInfo)
{
   BIGINT bTemp;
   char *cp;
   char bCount;

   /* Validate parameters if operating in test mode */
   ASSERT(pbHash != NULL);
   ASSERT(szString != NULL);
   ASSERT(pModInfo != NULL);

   /* Set initial hash value */
   bAssignAll(pbHash, 0x0000, 0x0000, 0x0000, 0xe8f4, 0x4d92, 0x7af1);

   while(*szString)
   {
      /* Get next six characters from string, to fill lower 48 bits of bTemp */
      bAssignWord(&bTemp, 0x0000);
      cp = (char *)&bTemp;
      bCount = 6;
      while(bCount-- && *szString)
      {
         *cp++ = *szString++;
      }

      /* Add the bTemp to the current hash value */
      bAdd(pbHash, &bTemp);
      bMod(pbHash, pModInfo);

      /* Square the hash value and calculate residue */
      bMult(pbHash, pbHash);
      bMod(pbHash, pModInfo);
   }

   /* Add final CONSTant to hash value, calculating residue */
   bAssignAll(&bTemp, 0x0000, 0x0000, 0x0000, 0x6c72, 0x92d3, 0x926a);
   bAdd(pbHash, &bTemp);
   bMod(pbHash, pModInfo);
}



/* bSpecialMod()                                                             */
/*                                                                           */
/* Calculates pbX mod pbY for a modulus that is used only once.              */
/*                                                                           */
/* pbX         Pointer to BIGINT where operand and result are stored         */
/* pbY         Pointer to BIGINT containing the modulus                      */

void bSpecialMod(BIGINT *pbX, CONST BIGINT *pbY)
{
   BIGINT bWork;
   int iShifted = 0;

   /* Validate parameters if operating in test mode */
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

   /* Initialize working variables */
   bAssign(&bWork, pbY);

   /* Loop while pbX is greater than power of modulus */
   while(bGreaterOrEqual(pbX, &bWork) && iShifted < BIGINT_WORDS*WORD_SIZE )
   {
      /* Double modulus */
      bShiftLeft(&bWork, 1);

      /* Count number of doublings */
      ++iShifted;
   }

   do
   {
      /* If pbX is >= this power of the modulus */
      if(bGreaterOrEqual(pbX, &bWork))
      {
         /* Then subtract the power of the modulus from pbX */
         bSub(pbX, &bWork);
      }

      /* Divide power of modulus by two */
      bShiftRightOne(&bWork);

      /* Repeat for each time that modulus has been doubled */
   } while(iShifted--);
}



/* bGCD()                                                                    */
/*                                                                           */
/* Returns the greatest common divisor of two BIGINTs (pbX, pbY).            */
/*                                                                           */
/* pbOut       Pointer to BIGINT in which GCD should be stored               */
/* pbX         Pointer to BIGINT containing the first value                  */
/* pbY         Pointer to BIGINT containing the second value                 */

void bGCD(BIGINT *pbOut, CONST BIGINT *pbX, CONST BIGINT *pbY)
{
   BIGINT bTemp, bWork;

   /* Validate parameters if operating in test mode */
   ASSERT(pbOut != NULL);
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

   /* Copy parameters into working variables */
   bAssign(&bWork, pbY);
   bAssign(pbOut, pbX);

   /* Loop until greatest common divisor has been isolated */
   while(!bZero(&bWork))
   {
      /* Store temporary copy of initial bWork */
      bAssign(&bTemp, &bWork);

      /* Store pbOut mod bTemp in bWork */
      bAssign(&bWork, pbOut);
      bSpecialMod(&bWork, &bTemp);

      /* Store initial bWork in pbOut */
      bAssign(pbOut, &bTemp);
   }
}



/* bSignedDiv()                                                              */
/*                                                                           */
/* Performs signed BIGINT division with truncation. Calculates               */
/*    pbQuotient = floor(pbDividend / pbDivisor)                             */
/* pbQuotient and pbDivident may point to the same BIGINT.                   */
/*                                                                           */
/* pbQuotient  Pointer to BIGINT in where result should be stored            */
/* pbDividend  Pointer to BIGINT containing the dividend                     */
/* pbDivisor   Pointer to BIGINT containing the divisor                      */

void bSignedDiv(BIGINT *pbQuotient, CONST BIGINT *pbDividend,
          CONST BIGINT *pbDivisor)
{
   BOOL bNegateResult = FALSE;
   BIGINT bDividend;
   BIGINT bDivisor;

   /* Validate parameters if operating in test mode */
   ASSERT(pbQuotient != NULL);
   ASSERT(pbDividend != NULL);
   ASSERT(pbDivisor != NULL);

   /* Copy parameters into working storage */
   bAssign(&bDividend, pbDividend);
   bAssign(&bDivisor, pbDivisor);

   /* If dividend is negative, convert to positive */
   if(bNegative(&bDividend))
   {
      bNegateResult = TRUE;
      bNegate(&bDividend);
   }

   /* If divisor is negative, convert to positive */
   if(bNegative(&bDivisor))
   {
      bNegateResult = !bNegateResult;
      bNegate(&bDivisor);
   }

   /* Perform unsigned integer division */
   bDiv(pbQuotient, &bDividend, &bDivisor);

   /* If result should be negated, then do so */
   if(bNegateResult)
   {
      bNegate(pbQuotient);
   }
}



/* bDiv()                                                                    */
/*                                                                           */
/* Performs integer division with truncation. Calculates                     */
/*    pbQuotient = floor(pbDividend / pbDivisor)                             */
/*                                                                           */
/* pbQuotient  Pointer to BIGINT in where result should be stored            */
/* pbDividend  Pointer to BIGINT containing the dividend                     */
/* pbDivisor   Pointer to BIGINT containing the divisor                      */

void bDiv(BIGINT *pbQuotient, CONST BIGINT *pbDividend,
          CONST BIGINT *pbDivisor)
{
   int iTimes = 0;
   BIGINT bLeft;
   BIGINT bMultiple;

   /* Validate parameters if operating in test mode */
   ASSERT(pbQuotient != NULL);
   ASSERT(pbDividend != NULL);
   ASSERT(pbDivisor != NULL);

   /* Check for division by zero if operating in test mode              */
   /* (RegKey has complete control over divisor, so there is no need to */
   /* perform this check in release code. An attempt to divide by zero  */
   /* will result in an infinite loop.)                                 */
   ASSERT(!bZero(pbDivisor));

   /* Initialize working variables */
   bAssign(&bLeft, pbDividend);
   bAssign(&bMultiple, pbDivisor);

   /* Initialize quotient to 0 */
   bAssignWord(pbQuotient, 0);

   /* Determine largest required multiple of divisor */
   while(bGreaterOrEqual(&bLeft, &bMultiple))
   {
      bShiftLeft(&bMultiple, 1);
      ++iTimes;
   }

   /* Loop across for all multiples of divisor, beginning with the largest */
   do
   {
      /* If current remainder is >= this multiple of the divisor */
      if(bGreaterOrEqual(&bLeft, &bMultiple))
      {
         /* Subtract the multiple of the divisor from the remainder */
         bSub(&bLeft, &bMultiple);

         /* The next bit of the quotient should be a 1 */
         bShiftLeft(pbQuotient, 1);
      bOrLowOrderWordWith(pbQuotient, 0x0001);
         }

      /* If current remainder is less than this multiple of the divisor */
      else
      {
         /* The next bit of the quotient should be 0 */
         bShiftLeft(pbQuotient, 1);
      }

      /* Divide current multiple of divisor by two */
      bShiftRightOne(&bMultiple);

      /* Repeat for all multiples of the divisor */
   } while(iTimes--);
}



/* bNegate()                                                                 */
/*                                                                           */
/* Negates a BIGINT.                                                         */
/*                                                                           */
/* pbX         Pointer to the BIGINT to be negated                           */

void bNegate(BIGINT *pbX)
{
   unsigned char bCount = BIGINT_WORDS;
   WORD *pwCurrent;

   /* Validate parameters of operating in test mode */
   ASSERT (pbX != NULL);

   /* Set pwCurrent to point to the first word of pbX */
   pwCurrent = (WORD *)pbX;

   /* Perform a logical not on all */
   while (bCount--)
   {
      *pwCurrent=~*pwCurrent;
      ++pwCurrent;
   }

   /* If using two's complement representation for negative numbers, add one */
   /* two result */
#ifdef TWOS_COMPLEMENT
   bIncrement(pbX);
#endif
}



/* bGCDWithCo()                                                              */
/*                                                                           */
/* Calculates the greatest common divisor of two BIGINTs, and also finds     */
/* the coefficients x and y such that d = gcd(a,b) = ax + by.                */
/*                                                                           */
/* pbA         Pointer to first BIGINT on which to calculate the gcd         */
/* pbB         Pointer to second BIGINT on which to calculate the gcd        */
/* pbD         Pointer to BIGINT where result should be stored               */
/* pbX         Pointer to BIGINT where coefficient of pbA should be stored   */
/* pbY         Pointer to BIGINT where coefficient of pbB should be stored   */

void bGCDWithCo(CONST BIGINT *pbA, CONST BIGINT *pbB,
                BIGINT *pbD, BIGINT *pbX, BIGINT *pbY)
{
   BIGINT bATemp;
   BIGINT bXTemp;
   BIGINT bYTemp;

   /* Validate parameters if operating in test mode */
   ASSERT(pbA != NULL);
   ASSERT(pbB != NULL);
   ASSERT(pbD != NULL);
   ASSERT(pbX != NULL);
   ASSERT(pbY != NULL);

   /* End recursion when pbB == 0 */
   if(bZero(pbB))
   {
      bAssign(pbD, pbA);
      bAssignWord(pbX, 1);
      bAssignWord(pbY, 0);
      return;
   }

   bAssign(&bATemp, pbA);

   bSpecialMod(&bATemp, pbB);
   bGCDWithCo(pbB, &bATemp, pbD, &bXTemp, &bYTemp);

   bAssign(pbX, &bYTemp);

   bAssign(&bATemp, pbA);
   bSignedDiv(&bATemp, &bATemp, pbB);
   bSignedMult(&bATemp, &bYTemp);
   bAssign(pbY, &bXTemp);
   bSub(pbY, &bATemp);
}



/* bLinearSolve()                                                            */
/*                                                                           */
/* Finds a solution to a modular linear equation of the form ax = b (mod n)  */
/*                                                                           */
/* pbA         Pointer to BIGINT where a from equation is stored             */
/* pbB         Pointer to BIGINT where b from equation is stored             */
/* pbX         Pointer to BIGINT where the solution should be stored         */
/* pbN         Pointer to BIGINT containing the modulus n                    */
/* pModInfo    Pointer to a MODINFO structure corresponding to the modulus n */

void bLinearSolve(CONST BIGINT *pbA, CONST BIGINT *pbB,
                  BIGINT *pbX, CONST BIGINT *pbN,
                  CONST MODINFO *pModInfo)
{
   BIGINT bD;
   BIGINT bX;
   BIGINT bY;

   /* Validate parameters if operating in test mode */
   ASSERT(pbA != NULL);
   ASSERT(pbB != NULL);
   ASSERT(pbX != NULL);
   ASSERT(pbN != NULL);
   ASSERT(pModInfo != NULL);

   /* Find the GCD and coefficients of pbA and pbN */
   bGCDWithCo(pbA, pbN, &bD, &bX, &bY);

   /* bX * (pbB / bD) (mod pbN) is a solution to the equation */
   bAssign(pbX, pbB);
   bSignedDiv(pbX, pbX, &bD);
   bSignedMod(pbX, pModInfo);
   bSignedMult(pbX, &bX);
   bSignedMod(pbX, pModInfo);
}
