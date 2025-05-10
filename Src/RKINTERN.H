/*





                                                                    R E G K E Y

 ------------------------------------------------------------------------------


                                                                   Version 3.20


                                    The Registration Key System For Programmers


                                                                    Source Code




          (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
        FILE : rkintern.h

     VERSION : 3.20

     PURPOSE : Internal header file used for the RegKey source code

  NOTICE - The conditions of use of this source code are specified in the
           RegKey documentation. Use of the RegKey source code is subject
           to a non-disclosure agreement. Please remember that you are not
           permitted to redistribute this source code.
*/


/* 80x86 INLINE ASSEMBLY DEFINITIONS */
/* --------------------------------- */

/* #define RKASM to use inline 80x86 assembly */
/* #undef RKASM to cause equivalent C code to be used in place of assembly */
#ifndef _WIN32
#define RKASM
#endif
               
/* Inline assembly keyword */
#ifdef _MSC_VER
#define ASM             __asm
#else
#define ASM             asm
#endif



/* CONFIGURABLE PARAMETERS */
/* ----------------------- */

/* (String sizes do not include string terminator character */
#define MAX_PATH_CHARS         80
#define MAX_FILENAME_CHARS     12
#define INTERNAL_STRING_CHARS  80



/* POINTER DEFINITIONS */
/* ------------------- */
#ifndef NULL
#define NULL ((void *)(0))
#endif



/* BOOLEAN DEFINITIONS */
/* ------------------- */

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif



/* BIG INTEGER DEFINITIONS */
/* ----------------------- */

#define BIGINT_WORDS          6
#define BIGINT_HALF           3
#ifndef _WIN32
#define WORD                  unsigned short int
#endif
#define WORD_SIZE             (sizeof(WORD) * 8)
#define WORD_UPPER_BIT_MASK   0x8000
#define TWOS_COMPLEMENT

typedef struct
{
   /* Lowest-order word stored in [0] */
   WORD iWord[BIGINT_WORDS];
} BIGINT;

/* Macro to assign value to all words forming a BIGINT */
#define bAssignAll(pbDest, iWord6, iWord5, iWord4, iWord3, iWord2, iWord1) \
   (pbDest)->iWord[0] = (iWord1); \
   (pbDest)->iWord[1] = (iWord2); \
   (pbDest)->iWord[2] = (iWord3); \
   (pbDest)->iWord[3] = (iWord4); \
   (pbDest)->iWord[4] = (iWord5); \
   (pbDest)->iWord[5] = (iWord6)

/* Macro to assign value to low-order word of a BIGINT, setting remaining */
/* words to 0                                                             */
#define bAssignWord(pbDest, iWord) \
   bAssignAll((pbDest), 0, 0, 0, 0, 0, (iWord))

/* Macro to return the low-order word of a BIGINT */
#define bGetLowOrderWord(pbSource) \
   ((pbSource)->iWord[0])

/* Macro to set specified bits of the low-order word of a BIGINT */
#define bOrLowOrderWordWith(pbDest, wSource) \
   ((pbDest)->iWord[0] |= (wSource))

/* Macro returns true iff the BIGINT pbX is == 0 */
#define bZero(pbX) \
   ((pbX)->iWord[0] == 0 && \
    (pbX)->iWord[1] == 0 && \
    (pbX)->iWord[2] == 0 && \
    (pbX)->iWord[3] == 0 && \
    (pbX)->iWord[4] == 0 && \
    (pbX)->iWord[5] == 0)

/* Macro returns true if pbX <= 2^(BIGINT_HALF*WORD_SIZE) */
#define bFitsInHalfOfBigint(pbX) \
   ((pbX)->iWord[3] == 0 && \
    (pbX)->iWord[4] == 0 && \
    (pbX)->iWord[5] == 0)

/* Macro subtractsd one from a BIGINT */
#define bDecrement(pbX) \
   bSub((pbX), &bOne)

/* Macro adds one to a BIGINT */
#define bIncrement(pbX) \
   bAdd((pbX), bOneGet())

/* Macro returns true iff BIGINT is < 0 */
#define bNegative(pbX) \
   (pbX)->iWord[5] & 0x8000

/* Macro copies one BIGINT to another */
#define bAssign(pbX, pbY) \
   (pbX)->iWord[0] = (pbY)->iWord[0]; \
   (pbX)->iWord[1] = (pbY)->iWord[1]; \
   (pbX)->iWord[2] = (pbY)->iWord[2]; \
   (pbX)->iWord[3] = (pbY)->iWord[3]; \
   (pbX)->iWord[4] = (pbY)->iWord[4]; \
   (pbX)->iWord[5] = (pbY)->iWord[5]

/* Macro returns true iff pbX >= pbY. Optimized for speed. */
#define bGreaterOrEqual(pbX, pbY) \
    (((pbX)->iWord[5] != (pbY)->iWord[5]) ? \
         ((pbX)->iWord[5] > (pbY)->iWord[5]) : \
     ((pbX)->iWord[4] != (pbY)->iWord[4]) ? \
         ((pbX)->iWord[4] > (pbY)->iWord[4]) : \
     ((pbX)->iWord[3] != (pbY)->iWord[3]) ? \
         ((pbX)->iWord[3] > (pbY)->iWord[3]) : \
     ((pbX)->iWord[2] != (pbY)->iWord[2]) ? \
         ((pbX)->iWord[2] > (pbY)->iWord[2]) : \
     ((pbX)->iWord[1] != (pbY)->iWord[1]) ? \
         ((pbX)->iWord[1] > (pbY)->iWord[1]) : \
     ((pbX)->iWord[0] >= (pbY)->iWord[0]))



/* MODULUS INFORMATION STRUCTURE */
/* ----------------------------- */

typedef struct
{
   int iSize;
   BIGINT bModMult[BIGINT_WORDS*WORD_SIZE];
} MODINFO;

extern MODINFO regKeyModInfo;
extern MODINFO modMinusOneInfo;
extern BOOL bModInited;



/* TESTING DEFINITIONS */
/* ------------------- */

#ifdef RKTEST
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif



/* DOS DIRECTORY ENTRY DEFINITIONS */
/* ------------------------------- */
typedef struct
{
   char        reserved[21];
   char        attrib;
   unsigned    ftime;
   unsigned    fdate;
   long        fsize;
   char        name[13];
} DIRENTRY;

#define DE_NORMAL   0x00
#define DE_RDONLY   0x01
#define DE_HIDDEN   0x02
#define DE_SYSTEM   0x04
#define DE_VOLID    0x08
#define DE_SUBDIR   0x10
#define DE_ARCH     0x20



/* PRIVATE FUNCTION PROTOTYPES */
/* --------------------------- */

void bAdd(BIGINT *pbX, const BIGINT *pbY);
void bToString(char *szOut, const BIGINT *pbIn);
void stringToB(BIGINT *pbOut, const char *szIn);
void bShiftLeft(BIGINT *pbX, WORD wDistance);
void bShiftRight(BIGINT *pbX, WORD wDistance);
void bShiftRightOne(BIGINT *pbX);
void bSub(BIGINT *pbX, const BIGINT *pbY);
void genModInfo(MODINFO *modInfo, const BIGINT *pbMod);
void bMod(BIGINT *pbX, const MODINFO *pModInfo);
void bHash(BIGINT *pbHash, const char *szString,
           const MODINFO *pModInfo);
void bMult(BIGINT *pbX, const BIGINT *pbY);
void bSignedMult(BIGINT *pbX, const BIGINT *pbY);
void bSignedMod(BIGINT *pbX, const MODINFO *pModInfo);
void bExpModN(BIGINT *pbOut, const BIGINT *pbBase, const BIGINT *pbExponent,
              const MODINFO *pModInfo);
void bGCD(BIGINT *pbOut, const BIGINT *pbX, const BIGINT *pbY);
void bSpecialMod(BIGINT *pbX, const BIGINT *pbY);
void bDiv(BIGINT *pbQuotient, const BIGINT *pbDividend,
          const BIGINT *pbDivisor);
void bNegate(BIGINT *pbX);
void bSignedDiv(BIGINT *pbQuotient, const BIGINT *pbDividend,
          const BIGINT *pbDivisor);
BOOL fileGet(const char *szFilename, char *pKey, char *pContents,
             int *pcbSize);
BOOL filePut(const char *szFilename, const char *pKey,
             const char *pContents, int cbSize);
void initMod(void);
BOOL bEqual(const BIGINT *pbX, const BIGINT *pbY);
void bGCDWithCo(const BIGINT *pbA, const BIGINT *pbB,
                BIGINT *pbD, BIGINT *pbX, BIGINT *pbY);
void bLinearSolve(const BIGINT *pbA, const BIGINT *pbB,
                  BIGINT *pbX, const BIGINT *pbN,
                  const MODINFO *pModInfo);
int fileFindFirst(const char *szPath, DIRENTRY *pDirEntry, int iAttrib);
void genFn(char *szOut, const char *szIn);
BIGINT *bOneGet(void);
BIGINT *bRegKeyModGet(void);
MODINFO *regKeyModInfoGet(void);
BOOL *bModInitedGet(void);
BIGINT *bModMinusOneGet(void);
MODINFO *modMinusOneInfoGet(void);
BIGINT *bRegKeyBaseGet(void);


#ifdef NOT_C_TARGET
#ifdef VB_TARGET
RKFUNCDEF RKRETURN pascal far __export rkncs(
   const char *szGenerationCode,
   char *szValidationCode);
RKFUNCDEF RKRETURN pascal far __export rkg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   char *szRegKey);
RKFUNCDEF RKRETURN pascal far __export rkv(
   const char *szRegString,
   const char *szRegKey,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   RKVALID *peRegistered);
RKFUNCDEF RKRETURN pascal far __export rkfg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   const char *szFileName);
RKFUNCDEF RKRETURN pascal far __export rkfv(
   const char *szFileName,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   char *szRegString,
   unsigned short int cbMaxStringSize,
   RKVALID *peRegistered);
#else
#ifdef PASCAL_TARGET
RKFUNCDEF RKRETURN rkncs(
   const char *szGenerationCode,
   char *szValidationCode);
RKFUNCDEF RKRETURN rkg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   char *szRegKey);
RKFUNCDEF RKRETURN rkv(
   const char *szRegString,
   const char *szRegKey,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   RKVALID *peRegistered);
RKFUNCDEF RKRETURN rkfg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   const char *szFileName);
RKFUNCDEF RKRETURN rkfv(
   const char *szFileName,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   char *szRegString,
   unsigned short int cbMaxStringSize,
   RKVALID *peRegistered);
#else
RKFUNCDEF RKRETURN rkncs(
   const char *szGenerationCode,
   char *szValidationCode);
RKFUNCDEF RKRETURN rkg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   char *szRegKey);
RKFUNCDEF RKRETURN rkv(
   const char *szRegString,
   const char *szRegKey,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   RKVALID *peRegistered);
RKFUNCDEF RKRETURN rkfg(
   const char *szRegString,
   const char *szGenerationCode,
   const char *szRandomSeed,
   const char *szFileName);
RKFUNCDEF RKRETURN rkfv(
   const char *szFileName,
   const char *szValidationCode,
   const char *szYourName,
   unsigned long int nYourKey,
   char *szRegString,
   unsigned short int cbMaxStringSize,
   RKVALID *peRegistered);
#endif
#endif
#endif

void rkr(CONST char *szYourName, unsigned long int nYourKey);
