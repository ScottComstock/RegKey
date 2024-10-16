' DemoAppW - Visual Basic version of the RegKey demonstration program.
'            Demonstrates the use of file-based registration key validation
'            within a program using the RegKey system. Displays one of two
'            simple messages based upon whether or not the user is
'            registered. To test in registerd mode, use KeyGen to generate
'            a *.KEY registration key file for DemoApp, and place that file
'            in the current default directory. To test in unregistered mode
'            remove any valid *.KEY files from the current default directory.


Dim Shared RegisteredMode As Long        ' Variable to store mode to operate in
Dim Shared RegistrationString As String      ' To store name of registered user

Sub main ()
   ' Check for a valid registration key file

   ' The following information is passed to the RegKeyFileValidate() function
   '    "*.KEY"             - Filespec of registration key file
   '    "0C9HMN1NDL"        - Application's validation code
   '    "Your Name", 0      - Your RegKey registration info
   '    RegistrationString$ - Where registration string will be placed
   '    255                 - Maximum size of registration string
   '    RegisteredMode%     - Where to store result of validation

   RegistrationString$ = String(255, vbNullChar)
   Result = RegKeyFileValidate("*.KEY", "0C9HMN1NDL", "Your Name", 0, RegistrationString$, 255, RegisteredMode)
   
   If RegisteredMode = RKRegistered Then
      ' If we are operating in registered mode, display registered message

      sMessage$ = "DemoApp is registered to: " + RegistrationString$ + Chr$(13) + "Thanks for registering DemoApp!"

      MsgBox sMessage$, 64, "DemoApp"

   Else
      ' If we are operating in UNregistered mode, display UNregistered message

      sMessage$ = "DemoApp is NOT registered" + Chr$(13) + "Please remember to register DemoApp!"

      MsgBox sMessage$, 16, "DemoApp"
   End If
End Sub

