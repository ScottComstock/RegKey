DECLARE FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
DECLARE FUNCTION RegKeyGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sRegKey$)
DECLARE FUNCTION RegKeyValidate% (sRegString$, sRegKey$, sValidationCode$, peRegistered%)
DECLARE FUNCTION RegKeyFileGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sFileName$)
DECLARE FUNCTION RegKeyFileValidate% (sFileName$, sValidationCode$, sRegString$, iMaxStringSize%, peRegistered%)

CONST RKFailure = 0
CONST RKSuccess = 1
CONST RKUnregistered = 0
CONST RKRegistered = 1

DIM I%, Igencode$, Ivalcode$, Ovalcode$
DIM Irandseed1$, Irandseed2$, Iregstring$, iregkey$
DIM Oregkey$, x$, isReg%, isRegF%
DIM Oregstring$
DIM gcf$, iCount&

CONST FALSE = 0
CONST TRUE = NOT FALSE

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

