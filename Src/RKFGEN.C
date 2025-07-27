/*





																	R E G K E Y

 ------------------------------------------------------------------------------


																   Version 3.20


									The Registration Key System For Programmers


																	Source Code




		  (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.

*/


/*
		FILE : rkfgen.c

	 VERSION : 3.20

	 PURPOSE : Source code for the RegKeyFileGenerate() function

  NOTICE - The conditions of use of this source code are specified in the
		   RegKey documentation. Use of the RegKey source code is subject
		   to a non-disclosure agreement. Please remember that you are not
		   permitted to redistribute this source code.
*/


#define BUILDING_REGKEY
#include "regkey.h"
#include "rkintern.h"



/* RegFileKeyGenerate()                                                      */
/*                                                                           */
/* Generates a file-based registration key for a particular user, using the  */
/* secret generation code corresponding to a particular application (as      */
/* passeed to RegKeyNewCodeSet()). The registration string is usually the    */
/* name of the registered user, but may also contain other information, such */
/* as the version registered or date of expiry. The registration string may  */
/* be zero to 64K characters in length, and is null-terminated. A            */
/* registration key file is generated, using the specified filename,         */
/* containing the registration string and the resulting registration key.    */
/* If a file with the specified name already exists, it is overwritten.      */
/* szRandomSeed should contain 10 random numbers and upper-case letters,     */
/* which are required during the registration key generation process.        */
/*                                                                           */
/* This function is called by KeyGen or your own registration key generation */
/* utility, each time a registration key is generated for a new user. This   */
/* function is used for file-based registration keys; compare with           */
/* RegKeyGenerate().                                                         */
/*                                                                           */
/* szRegString         INPUT: Registration string                            */
/* szGenerationCode    INPUT: App's generation code                          */
/* szRandomSeed        INPUT: Random number seed                             */
/* szFileName          INPUT: Registration key file name                     */

#ifdef WIN32
#ifdef NOT_C_TARGET
#ifdef VB_TARGET
RKFUNCDEF RKRETURN pascal far __export rkfg(
	const char* szRegString,
	const char* szGenerationCode,
	const char* szRandomSeed,
	const char* szFileName)
#else
#ifdef PASCAL_TARGET
RKFUNCDEF RKRETURN rkfg(
	const char* szRegString,
	const char* szGenerationCode,
	const char* szRandomSeed,
	const char* szFileName)
#else
RKFUNCDEF RKRETURN rkfg(
	const char* szRegString,
	const char* szGenerationCode,
	const char* szRandomSeed,
	const char* szFileName)
#endif
#endif
#else
RKFUNCDEF RKRETURN RKCALL RegKeyFileGenerate(
	CONST char* szRegString,
	CONST char* szGenerationCode,
	CONST char* szRandomSeed,
	CONST char* szFileName)
#endif
#endif

#ifdef MSDOS
#ifdef NOT_C_TARGET
#ifdef VBDOS_TARGET
	RKFUNCDEF RKRETURN RKCALL rkfg(
		CONST char* szRegString,
		CONST char* szGenerationCode,
		CONST char* szRandomSeed,
		CONST char* szFileName)
#endif
#else
	RKFUNCDEF RKRETURN RKCALL RegKeyFileGenerate(
		CONST char* szRegString,
		CONST char* szGenerationCode,
		CONST char* szRandomSeed,
		CONST char* szFileName)
#endif
#endif
{
	char szFilenameBuffer[13];
	char szRegKey[21];
	int cbRegString;
	CONST char* cp;

	/* Validate Parameters */
	if (!szRegString || !szGenerationCode || !szRandomSeed)
	{
		DPRINT(("rkfg: invalid parameters"));
		return(RK_FAILURE);
	}

	/* If filename is NULL or an empty string */
	if (!szFileName || *szFileName == '\0')
	{
		/* Generate filename from registration string */
		genFn((char*)szFilenameBuffer, szRegString);

		/* Set szFileName to now point to automatically generated filename */
		szFileName = szFilenameBuffer;
	}

	DPRINT(("rkfg: szRegString=\"%s\" len=%i", szRegString, strlen(*szRegString)));
	DPRINT(("rkfg: szGenerationCode=\"%s\" len=%i", szGenerationCode, strlen(*szGenerationCode)));
	DPRINT(("rkfg: szRandomSeed=\"%s\" len=%i", szRandomSeed, strlen(*szRandomSeed)));
	DPRINT(("rkfg: szFileName=\"%s\" len=%i", szFileName, strlen(*szFileName)));

	/* Call RegKeyGenerate() to perform actual registration key generation */
#ifdef NOT_C_TARGET
	if (rkg(szRegString, szGenerationCode, szRandomSeed, szRegKey)
		!= RK_SUCCESS)
#else
	if (RegKeyGenerate(szRegString, szGenerationCode, szRandomSeed, szRegKey)
		!= RK_SUCCESS)
#endif
	{

		DPRINT(("rkfg: rkg returned failure"));
		return(RK_FAILURE);
	}

	/* Determine the number of characters in the registration string */
	cbRegString = 0;
	cp = szRegString;
	while (*cp++) ++cbRegString;

	/* Create and write registration key file */
	if (!filePut(szFileName, szRegKey, szRegString, cbRegString))
	{

		DPRINT(("rkfg: filePut returned failure"));
		return(RK_FAILURE);
	}

	/* Return with success */
	DPRINT(("rkfg: OK"));
	return(RK_SUCCESS);
}



/* genFn()                                                                   */
/*                                                                           */
/* Automatically generates a registration key filename based upon the        */
/* registration string                                                       */
/*                                                                           */
/* szOut               Pointer to string where filename should be stored     */
/* szIn                Pointer to registration string                        */

void genFn(char* szOut, CONST char* szIn)
{
	char chars = 0;
	char* cp;

	/* Validate parameters if operating in test mode */
	ASSERT(szOut != NULL);
	ASSERT(szIn != NULL);

	/* Copy up to eight alphabetical characters from szIn to szOut */
	while (*szIn && chars < 8)
	{
		/* If next character in szIn is an upper case letter */
		if (*szIn >= 'A' && *szIn <= 'Z')
		{
			/* Increment total number of characters */
			chars++;

			/* Copy letter */
			*szOut++ = *szIn;
		}

		/* If next character in szIn is a lower case letter */
		else if (*szIn >= 'a' && *szIn <= 'z')
		{
			/* Increment total number of characters */
			chars++;

			/* Copy letter, converting to upper case */
			*szOut++ = *szIn - ('a' - 'A');
		}
		/* Increment input string pointer */
		++szIn;
	}

	/* If no alphabetical characters were found in input string */
	if (chars == 0)
	{
		/* Copy "REGISTER.KEY" to output string */
		cp = "REGISTER.KEY";
		while ((*szOut++ = *cp++) != 0);
	}

	/* If at least one alphabetical character was found in input string */
	else
	{
		/* Append ".KEY" extension to filename */
		cp = ".KEY";
		while ((*szOut++ = *cp++) != 0);
	}
}
