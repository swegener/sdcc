//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES 
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
// OTHER DEALINGS IN THE SOFTWARE.
// 
// Except as contained in this notice, the name of Dallas Semiconductor 
// shall not be used except as stated in the Dallas Semiconductor 
// Branding Policy. 
//---------------------------------------------------------------------------
//
//  thermodl.c - This utility uses to download the results of the
//               current mission of a DS1921 Thermochron iButton.
//
//  Version: 2.00
//    
//    History:
//           1.03 -> 2.00  Reorganization of Public Domain Kit 
//                         Y2K update, display all histogram bins, debug
//                         dump.  Supports multiple thermochons.
//

#include <stdio.h>
#include <stdlib.h>
#include "ownet.h"   
#include "thermo21.h"

// TINI hack
#define ExitProg(msg,exit_code) {printf("%s\n",msg); exit(exit_code);}

// defines
#define MAXDEVICES   20

// local function prototypes
static void PrintResults(ThermoStateType *,FILE *,int);

//----------------------------------------------------------------------
//  This is the Main routine for thermodl.
//
int main()
{
   int Fahrenheit=FALSE,num,i,j;
   FILE *fp=stdout;
   char return_msg[128];
   ThermoStateType ThermoState;
   uchar ThermoSN[MAXDEVICES][8]; //the serial numbers for the devices
   int portnum=0;

   // attempt to acquire the 1-Wire Net
   if (!owAcquire(portnum,""))
     return 0;

   // success

   //----------------------------------------
   // Introduction
   printf("\n/----------------------------------------------\n");
   printf("  Find and download DS1921 Thermochron iButton(s)\n" 
          "  Version 2.00\n\n");

   // check arguments for temperature conversion and filename
   Fahrenheit = FALSE;

   // get list of Thermochron's 
	num = FindDevices(portnum, &ThermoSN[0],THERMO_FAM, MAXDEVICES);

   // check if not present or more then 1 present
   if (num == 0)
      ExitProg("Thermochron not present on 1-Wire\n",1);   

   // loop to download each Thermochron
   for (i = 0; i < num; i++)
   {
      // set the serial number portion in the thermo state
      printf("\nDownloading: ");
      for (j = 7; j >= 0; j--)
      {
         ThermoState.MissStat.serial_num[j] = ThermoSN[i][j];
         printf("%02X",ThermoSN[i][j]);
      }
      printf("\n");

      // download the Thermochron found
      if (DownloadThermo(portnum,&ThermoSN[i][0],&ThermoState,stdout))
      {
         // interpret the results of the download
         InterpretStatus(&ThermoState.MissStat);
         InterpretAlarms(&ThermoState.AlarmData, &ThermoState.MissStat);
         InterpretHistogram(&ThermoState.HistData);
         InterpretLog(&ThermoState.LogData, &ThermoState.MissStat);

         // print the output
         PrintResults(&ThermoState,fp,Fahrenheit);
      }
      else
      {
         fprintf(fp,"\nError downloading device: ");
         for (j = 0; j < 8; j++)
            fprintf(fp,"%02X",ThermoSN[i][j]);
         fprintf(fp,"\n");
      }
   }

   // release the 1-Wire Net
   owRelease(portnum);
   printf("\n%s",return_msg);
   ExitProg("End program normally\n",0);

   return 0;
}

//--------------------------------------------------------------------------
//  Prints the mission data optionaly to a file or standard out
//
void PrintResults(ThermoStateType *ThermoState, FILE *fp, int ConvertToF)
{
  //char *str;  
  char str[80000];

   // check if need to use standard out
   if (fp == NULL)
      fp = stdout;

   // get big block to use as a buffer
#if 0
   str = malloc(80000);   
   if (str == NULL)
   {
      printf("Insufficient memory available to print!\n"); 
      return;
   }
#endif

   // mission status 
   MissionStatusToString(&ThermoState->MissStat, ConvertToF, &str[0]);
   fprintf(fp,"\n%s\n",str);

   // alarm events
   AlarmsToString(&ThermoState->AlarmData, &str[0]);
   fprintf(fp,"%s\n",str);

   // histogram
   HistogramToString(&ThermoState->HistData, ConvertToF, &str[0]);
   fprintf(fp,"%s\n",str);

   // log data
   LogToString(&ThermoState->LogData, ConvertToF, &str[0]);
   fprintf(fp,"%s\n",str);

   // debug raw data
   DebugToString(&ThermoState->MissStat, &ThermoState->AlarmData, 
      &ThermoState->HistData, &ThermoState->LogData, &str[0]); 
   fprintf(fp,"%s\n",str);

   // free the memory block used
   //free(str);
}

