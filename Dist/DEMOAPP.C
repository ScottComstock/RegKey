/* DemoAppW - Windows C Language version of the RegKey demonstration program.*/
/*            Demonstrates the use of file-based registration key validation */
/*            within a program using the RegKey system. Displays one of two  */
/*            simple messages based upon whether or not the user is          */
/*            registered. To test in registerd mode, use KeyGen to generate  */
/*            a *.KEY registration key file for DemoApp, and place that file */
/*            in the current default directory. To test in unregistered mode */
/*            remove any valid *.KEY files from the current default          */
/*            directory.                                                     */


#include "windows.h"
#include <stdio.h>                                 /* Included for sprintf() */

#include "regkey.h"     /* This must be included in any program using RegKey */


RKVALID RegisteredMode;              /* Variable to store mode to operate in */
char RegistrationString[256];            /* To store name of registered user */

char szMessage[200];                    /* String to hold message to display */


int PASCAL WinMain(hinstCurrent, hinstPrevious, lpCmdLine, nCmdShow)
{
   /* Check for a valid registration key file */

   RegKeyFileValidate("*.KEY",          /* Filespec of registration key file */
                      "0C9HMN1NDL",         /* Application's validation code */
                      "Your Name", 0L,      /* Your RegKey registration info */
                      RegistrationString,      /* Where to place reg. string */
                      255,                    /* Maximum size of reg. string */
        (RKVALID *)   &RegisteredMode);     /* To store result of validation */


   if(RegisteredMode == RK_REGISTERED)
   {
      /* If we are operating in registered mode, display registered message */

      sprintf(szMessage, "DemoApp is registered to: %s\n"
                         "Thanks for registering DemoApp!\n",
                         RegistrationString);
      MessageBox(NULL, szMessage, "DemoApp", MB_OK | MB_ICONINFORMATION);
   }
   else
   {
      /* If operating in UNregistered mode, display UNregistered message */

      sprintf(szMessage, "DemoApp is NOT registered\n"
                         "Please remember to register DemoApp!\n");
      MessageBox(NULL, szMessage, "DemoApp", MB_OK | MB_ICONSTOP);
   }

   return(0);
}
