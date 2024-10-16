'
'
'
'
'
'
'                                                                   R E G K E Y
'
'------------------------------------------------------------------------------
'
'
'                                                                  Version 3.20
'
'
'                                   The Registration Key System For Programmers
'
'
'                                                        Visual Basic Interface
'
'
'
'
'         (C) Copyright Brian Pirie, 1993 - 1998. All Rights Reserved.
'
'



' FUNCTION RETURN VALUES
' ----------------------

Public Const RKFailure = 0
Public Const RKSuccess = 1



' REGISTRATION KEY VALIDATION RESULTS
' -----------------------------------

Public Const RKUnregistered = 0
PUblic Const RKRegistered = 1



' DLL FUNCTION DECLARATIONS
' -------------------------

Declare Function RegKeyNewCodeSet Lib "RK32.DLL" (ByVal sGenerationCode As String, ByVal sValidationCode As String) As Long
Declare Function RegKeyGenerate Lib "RK32.DLL" (ByVal sRegString As String, ByVal sGenerationCode As String, ByVal sRandomSeed As String, ByVal sRegKey As String) As Long
Declare Function RegKeyValidate Lib "RK32.DLL" (ByVal sRegString As String, ByVal sRegKey As String, ByVal sValidationCode As String, ByVal sYourName As String, ByVal nYourKey As Long, peRegistered As Long) As Long
Declare Function RegKeyFileGenerate Lib "RK32.DLL" (ByVal sRegString As String, ByVal sGenerationCode As String, ByVal sRandomSeed As String, ByVal sFileName As String) As Long
Declare Function RegKeyFileValidate Lib "RK32.DLL" (ByVal sFileName As String, ByVal sValidationCode As String, ByVal sYourName As String, ByVal nYourKey As Long, ByVal sRegString As String, ByVal cbMaxStringSize As Integer, peRegistered As Long) As Long
