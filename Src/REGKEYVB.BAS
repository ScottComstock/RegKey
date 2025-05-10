DIM mallocbuf%(2048)
COMMON SHARED /NMALLOC/ mallocbuf%()

DECLARE FUNCTION basictoc$ (st$)
DECLARE FUNCTION ctobasic$ (st$)
DECLARE FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
DECLARE FUNCTION RKNCS% CDECL ALIAS "_RegKeyNewCodeSet" (BYVAL sgc AS LONG, BYVAL sgv AS LONG)

DIM i%, vcf$, vcc$
DIM gcf$

OPEN "rkncs.tst" FOR INPUT AS #1

WHILE NOT EOF(1)
	INPUT #1, gcf$, vcf$
	i% = RegKeyNewCodeSet(gcf$, vcc$)
	IF i% = 1 THEN
		PRINT "GC:" + gcf$ + " VCF:" + vcf$ + " VCC:" + vcc$ + " TEST:";
		IF (vcf$ = vcc$) THEN
			PRINT "PASS"
		ELSE
			PRINT "FAIL"
			STOP
		END IF
	ELSE
		STOP
	END IF
WEND

CLOSE #1
END


REM FUNCTION RegKeyFileGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sFileName$)
REM     srs$ = basictoc(sRegString$)
REM   sgc$ = basictoc(sGenerationCode$)
REM     sras$ = basictoc(sRandomSeed$)
REM     srk$ = basictoc(sRegKey$)
REM     sfn$ = basictoc(sFileName$)
REM     RegKeyFileGenerate% = RKFG%(srs$, )
REM END FUNCTION

REM FUNCTION RegKeyFileValidate% (sFileName$, sValidationCode$, sYourName$, nYourKey AS LONG, sRegString$, cbMaxStringSize AS INTEGER, peRegistered AS INTEGER)
REM END FUNCTION

REM FUNCTION RegKeyGenerate% (sRegString$, sGenerationCode$, sRandomSeed$, sRegKey$)
REM     DIM srs$, sgc$, sras$, srk$
REM     srs$ = basictoc(sRegString$)
REM     sgc$ = basictoc(sGenerationCode$)
REM     sras$ = basictoc(sRandomSeed$)
REM     srk$ = basictoc(sRegKey$)
	
REM     RegKeyGenerate% = RKG%(srs$, sgc$, sras$, srk$)
REM     sRegKey$ = ctobasic(srk$)
REM END FUNCTION



REM FUNCTION RegKeyValidate% (sRegString$, sRegKey$, sValidationCode$, sYourName$, nYourKey AS LONG, peRegistered AS INTEGER)
REM END FUNCTION

FUNCTION basictoc$ (st$)
	basictoc$ = st$ + CHR$(0)
END FUNCTION

FUNCTION ctobasic$ (st$)
	DIM i%
	i% = 1
	DO UNTIL MID$(st$, i%, 1) = CHR$(0)
		Ret$ = Ret$ + MID$(st$, i%, 1)
		i% = i% + 1
	LOOP
	ctobasic$ = Ret$
END FUNCTION

FUNCTION RegKeyNewCodeSet% (sGenerationCode$, sValidationCode$)
	DIM sg$, sv$, Ret%
	sg$ = basictoc$(LEFT$(sGenerationCode$, 10))
	sv$ = basictoc$(STRING$(10, 32))
	Ret% = RKNCS%(SSEGADD(sg$), SSEGADD(sv$))
	RegKeyNewCodeSet% = Ret%
	sValidationCode$ = ctobasic(sv$)
END FUNCTION

