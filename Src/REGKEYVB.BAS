DIM mallocbuf%(2048)
COMMON SHARED /NMALLOC/ mallocbuf%()

CONST GCLENGTH = 10, VCLENGTH = 10, RANDLENGTH = 10, RKLENGTH = 20
CONST FALSE = 0
CONST TRUE = NOT FALSE

CONST RKFailure = 0
CONST RKSuccess = 1
CONST RKUnregistered = 0
CONST RKRegistered = 1

DECLARE FUNCTION basictoc$ (st$)
DECLARE FUNCTION ctobasic$ (st$)
DECLARE FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
DECLARE FUNCTION RegKeyGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sRegKey$)
DECLARE FUNCTION RegKeyValidate% (sRegString$, sRegKey$, sValidationCode$, peRegistered%)
DECLARE FUNCTION RegKeyFileGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sFileName$)
DECLARE FUNCTION RegKeyFileValidate% (sFileName$, sValidationCode$, sRegString$, iMaxStringSize%, peRegistered%)
DECLARE FUNCTION RKNCS% CDECL ALIAS "_RegKeyNewCodeSet" (BYVAL SGC AS LONG, BYVAL sgv AS LONG)
DECLARE FUNCTION RKG% CDECL ALIAS "_RegKeyGenerate" (BYVAL SRS AS LONG, BYVAL SGC AS LONG, BYVAL SRDS AS LONG, BYVAL srk AS LONG)
DECLARE FUNCTION RKV% CDECL ALIAS "_RegKeyValidate" (BYVAL SRS AS LONG, BYVAL srk AS LONG, BYVAL SVC AS LONG, BYVAL SYN AS LONG, BYVAL YK AS LONG, BYVAL PX AS LONG)
DECLARE FUNCTION RKFG% CDECL ALIAS "_RegKeyFileGenerate" (BYVAL SRS AS LONG, BYVAL SGC AS LONG, BYVAL SRDS AS LONG, BYVAL SFN AS LONG)
DECLARE FUNCTION RKFV% CDECL ALIAS "_RegKeyFileValidate" (BYVAL SFN AS LONG, BYVAL SVC AS LONG, BYVAL SYN AS LONG, BYVAL YK AS LONG, BYVAL SRS AS LONG, BYVAL SMAX AS INTEGER, BYVAL PX AS LONG)

DIM I%, Igencode$, Ivalcode$, Ovalcode$
DIM Irandseed1$, Irandseed2$, Iregstring$, iregkey$
DIM Oregkey$, x$, isReg%, isRegF%
DIM Oregstring$
DIM gcf$, iCount&

CLS
OPEN "tests.csv" FOR INPUT AS #1

iCount& = 1

WHILE NOT EOF(1)
	isReg% = -1: isRegF% = -1: Ovalcode$ = "": Oregkey$ = ""
	INPUT #1, Igencode$, Ivalcode$, Irandseed1$, Irandseed2$, Iregstring$, iregkey$, x$

	I% = RegKeyNewCodeSet(Igencode$, Ovalcode$)
	VCTest% = (Ivalcode$ = Ovalcode$)
	IF I% <> RKSuccess THEN STOP

	I% = RegKeyGenerate(Iregstring$, Igencode$, Irandseed1$, Oregkey$)
	GenRegTest% = (iregkey$ = Oregkey$)
	IF I% <> RKSuccess THEN STOP

	I% = RegKeyValidate(Iregstring$, Oregkey$, Ovalcode$, isReg%)
	IF I% <> RKSuccess THEN STOP

	I% = RegKeyFileGenerate(Iregstring$, Igencode$, Irandseed2$, "test.key")
	IF I% <> RKSuccess THEN STOP

	I% = RegKeyFileValidate("test.key", Ovalcode$, Oregstring$, 255, isRegF%)
	IF I% <> RKSuccess THEN STOP

	PRINT "Count: " + LTRIM$(RTRIM$(STR$(iCount&)))
	PRINT "Gencode:" + Igencode$ + " Expected VC:" + Ivalcode$ + " Calculated VC:" + Ovalcode$ + " VCTEST: " + LTRIM$(RTRIM$(STR$(VCTest%)))
	PRINT "Expected RK: " + iregkey$ + " Actual RK: " + Oregkey$ + " RKTEST: " + LTRIM$(RTRIM$(STR$(GenRegTest%)))
	PRINT "Validated (manual): " + LTRIM$(RTRIM$(STR$(isReg%))) + " Validated (file): " + LTRIM$(RTRIM$(STR$(isRegF%)))
	KILL "test.key"
	IF NOT (VCTest% = TRUE AND GenRegTest% = TRUE AND isReg% = RKRegistered AND isRegF% = RKRegistered) THEN
		STOP
	END IF
	iCount& = iCount& + 1
WEND

CLOSE #1
END

FUNCTION basictoc$ (st$)
	basictoc$ = st$ + CHR$(0)
END FUNCTION

FUNCTION ctobasic$ (st$)
	DIM I%
	I% = 1
	DO UNTIL MID$(st$, I%, 1) = CHR$(0)
		Ret$ = Ret$ + MID$(st$, I%, 1)
		I% = I% + 1
	LOOP
	ctobasic$ = Ret$
END FUNCTION

FUNCTION RegKeyFileGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sFileName$)
	DIM Ret%, SRS$, SGC$, SRDS$, SFN$
	SRS$ = basictoc$(sRegString$)
	SGC$ = basictoc$(LEFT$(sGenerationCode$ + STRING$(GCLENGTH, 32), GCLENGTH))
	SRDS$ = basictoc$(LEFT$(sRandomSeed$ + STRING$(RANDLENGTH, 32), RANDLENGTH))
	SFN$ = basictoc$(sFileName$)
	Ret% = RKFG%(SSEGADD(SRS$), SSEGADD(SGC$), SSEGADD(SRDS$), SSEGADD(SFN$))
	RegKeyFileGenerate% = Ret%
END FUNCTION

FUNCTION RegKeyFileValidate% (sFileName$, sValidationCode$, sRegString$, iMaxStringSize%, peRegistered%)
	DIM SFN$, SVC$, SRS$, immsptr&, pptr&, pr%, Ret%, SYN$
	IF (iMaxStringSize% <= 0) THEN
		iMaxStringSize% = 255
	END IF
	pr% = -1
	SFN$ = basictoc$(sFileName$)
	SVC$ = basictoc$(LEFT$(sValidationCode$ + STRING$(VCLENGTH, 32), VCLENGTH))
	SRS$ = basictoc$(STRING$(iMaxStringSize%, 32))
	SYN$ = ""
	pptr& = CLNG(VARSEG(pr%) * &H10000 + VARPTR(pr%))
	Ret% = RKFV(SSEGADD(SFN$), SSEGADD(SVC$), SSEGADD(SYN$), 0, SSEGADD(SRS$), iMaxStringSize%, pptr&)
	RegKeyFileValidate% = Ret%
	peRegistered% = pr%
END FUNCTION

FUNCTION RegKeyGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sRegKey$)
	DIM Ret%, SRS$, SGC$, SRDS$, srk$
	SRS$ = basictoc$(sRegString$)
	SGC$ = basictoc$(LEFT$(sGenerationCode$ + STRING$(GCLENGTH, 32), GCLENGTH))
	SRDS$ = basictoc$(LEFT$(sRandomSeed$ + STRING$(RANDLENGTH, 32), RANDLENGTH))
	srk$ = basictoc$(STRING$(RKLENGTH, 32))
	Ret% = RKG%(SSEGADD(SRS$), SSEGADD(SGC$), SSEGADD(SRDS$), SSEGADD(srk$))
	RegKeyGenerate% = Ret%
	sRegKey$ = ctobasic(srk$)
END FUNCTION

FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
	DIM sg$, sv$, Ret%
	sg$ = basictoc$(LEFT$(sGenerationCode$ + STRING$(GCLENGTH, 32), GCLENGTH))
	sv$ = basictoc$(STRING$(VCLENGTH, 32))
	Ret% = RKNCS%(SSEGADD(sg$), SSEGADD(sv$))
	RegKeyNewCodeSet% = Ret%
	sValidationCode$ = ctobasic(sv$)
END FUNCTION

FUNCTION RegKeyValidate% (sRegString$, sRegKey$, sValidationCode$, peRegistered AS INTEGER)
	DIM Ret%, SRS$, srk$, SVC$, SYN$, pr%, pptr&
	SRS$ = basictoc$(sRegString$)
	srk$ = basictoc$(sRegKey$)
	SVC$ = basictoc$(LEFT$(sValidationCode$ + STRING$(VCLENGTH, 32), VCLENGTH))
	SYN$ = "": pr% = -1
	pptr& = CLNG(VARSEG(pr%) * &H10000 + VARPTR(pr%))
	Ret% = RKV%(SSEGADD(SRS$), SSEGADD(srk$), SSEGADD(SVC$), SSEGADD(SYN$), 0, pptr&)
	RegKeyValidate% = Ret%
	peRegistered = pr%
END FUNCTION

