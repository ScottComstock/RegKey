' RegKey 3.20 VB/DOS bridge code
' (C)2025 Scott F. Comstock (scottcomstock@GitHub)
' To be distributed under the same license as RegKey itself (MIT)

DIM mallocbuf%(2048)
COMMON SHARED /NMALLOC/ mallocbuf%()

CONST GCLENGTH = 10, VCLENGTH = 10, RANDLENGTH = 10, RKLENGTH = 20

DECLARE FUNCTION basictoc$ (st$)
DECLARE FUNCTION ctobasic$ (st$)
DECLARE FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
DECLARE FUNCTION RegKeyGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sRegKey$)
DECLARE FUNCTION RegKeyValidate% (sRegString$, sRegKey$, sValidationCode$, peRegistered%)
DECLARE FUNCTION RegKeyFileGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sFileName$)
DECLARE FUNCTION RegKeyFileValidate% (sFileName$, sValidationCode$, sRegString$, iMaxStringSize%, peRegistered%)
Declare Function RKNCS% CDECL Alias "_rkncs" (ByVal SGC As Long, ByVal sgv As Long)
Declare Function RKG% CDECL Alias "_rkg" (ByVal SRS As Long, ByVal SGC As Long, ByVal SRDS As Long, ByVal srk As Long)
Declare Function RKV% CDECL Alias "_rkv" (ByVal SRS As Long, ByVal srk As Long, ByVal SVC As Long, ByVal SYN As Long, ByVal YK As Long, ByVal PX As Long)
Declare Function RKFG% CDECL Alias "_rkfg" (ByVal SRS As Long, ByVal SGC As Long, ByVal SRDS As Long, ByVal SFN As Long)
Declare Function RKFV% CDECL Alias "_rkfv" (ByVal SFN As Long, ByVal SVC As Long, ByVal SYN As Long, ByVal YK As Long, ByVal SRS As Long, ByVal SMAX As Integer, ByVal PX As Long)

Function basictoc$ (st$)
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

