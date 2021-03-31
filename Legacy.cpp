// **************************************************************
//
// Author:      Bryan Ramos
// Course:      CS1336.010
// Date:        3/23/21
// Assignment:  LegacySoftware
// Compiler:    Xcode v12.2
//
// Description:
//
//
// **************************************************************

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>

static unsigned long long _previousTotalTicks = 0;
static unsigned long long _previousIdleTicks = 0;

using namespace std;

// GPA Calculation App v1.0 Prototypes
int gpaCalculation();
int letterToPoints(char grade);

// Payroll Calculator App v1.0 Prototypes
int annualPay();

// Settings App v1.0 Prototypes
void settings();
float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
float GetCPULoad();
int memoryUsage();

void loadingScreen();

int main()
{
   int selection;
   //int choices[3] = {1, 2, 3};
   
   cout << "Welcome to Legacy v1.0" << endl;
   
   
   cout << "Loading software..." << endl;
   loadingScreen();
   
   cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
   cout << "Legacy v1.0" << endl;
   cout << endl;
   
   cout << "Home Screen" << endl;
   cout << " ----------"  << "     " << "  ----------"  << "     "  << "  ----------" << "     "  << "  ----------" << endl;
   cout << "|         1|" << "     " << "|         2|"  << "     "  << "|         3|" << "     "  << "|         4|" << endl;
   cout << "|          |" << "     " << "|          |"  << "     "  << "|          |" << "     "  << "|          |" << endl;
   cout << "| Settings |" << "     " << "| GPA Calc |"  << "     "  << "|  Payroll |" << "     "  << "|          |" << endl;
   cout << "|          |" << "     " << "|          |"  << "     "  << "|   Calc   |" << "     "  << "|          |" << endl;
   cout << "|          |" << "     " << "|          |"  << "     "  << "|          |" << "     "  << "|          |" << endl;
   cout << " ----------"  << "     " << "  ----------"  << "     "  << "  ----------" << "     "  << "  ----------" << endl;
   cout << endl;
   
   cout << "Press a number corresponding the app to launch the software." << endl;
   cin >> selection;
   cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

   /*
   cout << "(1) »" << endl;
   cout << "(2) »" << endl;
   cout << "(3) »" << endl;
   */
   
   while (selection < 1 || selection > 3)
   {
      cout << "Error finding application with corresponding selection" << endl;
      cout << "Please input a selection number again" << endl;
      cin  >> selection;
      cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
   }
      
   if (selection == 1) {
      cout << "Launching application for selection " << selection  << "..." << endl;
      loadingScreen();
      settings();
   }
   
      
   else if (selection == 2) {
      cout << "Launching application for selection " << selection << "..." << endl;
      loadingScreen();
      gpaCalculation();
   }
      
   else if (selection == 3) {
      cout << "Launching application for selection " << selection << "..." << endl;
      loadingScreen();
      annualPay();
   }
   
   
}

// **************************************************************
//
//
// Settings App v1.0
//
//
// **************************************************************

void settings()
{
   int setSelection;
   
   cout << "Settings" << endl;
   cout << "1 » View Memory Usage" << endl;
   cout << "2 » View CPU Usage" << endl;
   cout << "3 » Coming Soon" << endl;
   
   cin  >> setSelection;

   
   while (setSelection < 1 || setSelection > 3)
   {
      cout << "Error finding selection" << endl;
      cout << "Please input a selection number again" << endl;
      cin  >> setSelection;
      cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
   }
      
   if (setSelection == 1) {
      cout << "Launching application for selection " << setSelection  << "..." << endl;
      memoryUsage();
   }
   
      
   else if (setSelection == 2) {
      cout << "Launching application for selection " << setSelection << "..." << endl;
      GetCPULoad();
   }
      
   else if (setSelection == 3) {
      cout << "Launching application for selection " << setSelection << "..." << endl;
   }
}


int memoryUsage()
{
   struct rusage r_usage;
      int *p = 0;
      while(1) {
         p = (int*)malloc(sizeof(int)*1000);
         int ret = getrusage(RUSAGE_SELF,&r_usage);
         if(ret == 0)
            printf("Memory usage: %ld kilobytes\n",r_usage.ru_maxrss);
         else
            printf("Error in getrusage. errno = %d\n", errno);
         usleep(10);
      }
   
      return 0;
}



// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.
float GetCPULoad()
{
   host_cpu_load_info_data_t cpuinfo;
   mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
   if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)
   {
      unsigned long long totalTicks = 0;
      
      for(int i = 0; i < CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];
      cout << (CalculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks) * 100) << "% CPU used out of 100%" << endl;
      
      return CalculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
      
   }
   else return -1.0f;
}

float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
  unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
  unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;
  float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);
  _previousTotalTicks = totalTicks;
  _previousIdleTicks  = idleTicks;
  return ret;
}

// **************************************************************
//
//
// Payroll Calculator App v1.0
//
//
// **************************************************************
int annualPay()
{
   // user input value (cin)
   int payAmount;
   
   // predefined pay period (we already know this number)
   int payPeriods = 26;
   
   // annualPay = payAmount * payPeriods
   // calculation
   int annualPay;
   
   cout << "Enter pay amount" << endl;
   cin  >> payAmount;
   
   annualPay = payAmount * payPeriods;
   
   cout << "Annual Pay: $" << annualPay << endl;
   
   
   return 0;
}

// **************************************************************
//
//
// GPA Calculation App v1.0
//
//
// **************************************************************
int gpaCalculation()
{
   int courseNumber;
   int creditHours;
   char letterGrade;
   
   int numberGrade;
   
   int totalNumberGPA = 0;
   int totalNumberCreditHours = 0;
   
   double calculatingGPA = 0;
   double finalGPA = 0;
   
   cout << "Enter the number of courses ";
   cin  >> courseNumber;
      
   for (int i = 1; i <= courseNumber; i++)
   {
      cout << "Enter credit hours & grade for course #" << i << ": ";
      cin  >> creditHours >> letterGrade;
            
      numberGrade = letterToPoints(letterGrade);
      
      // (2*4 + 3*3 + 4*4 +3*2) / (2+3+4+3) = 3.25
      
      calculatingGPA = calculatingGPA + (creditHours * numberGrade);
      
      totalNumberGPA = totalNumberGPA + numberGrade;
      totalNumberCreditHours = totalNumberCreditHours + creditHours;
      
   }
   
   finalGPA = calculatingGPA / totalNumberCreditHours;
   
   cout << "Final GPA: " << fixed << setprecision(2) << finalGPA << endl;
   
   return 0;
}

int letterToPoints(char grade) {
   
   int letterToGrade = 0;
   
   
   if (grade == 'A' || grade == 'a') {
      letterToGrade = 4;
   }
   
   else if (grade == 'B' || grade == 'b') {
      letterToGrade = 3;
   }
   
   else if (grade == 'C' || grade == 'c') {
      letterToGrade = 2;
   }
   
   else if (grade == 'D' || grade == 'd') {
      letterToGrade = 1;
   }
   
   else if (grade == 'F' || grade == 'f') {
      letterToGrade = 0;
   }
   
   return letterToGrade;
}


void loadingScreen()
{
   cout << '-' << flush;
   for (int i = 0; i <= 2; i++)
   {
      sleep(1);
      cout << "\b\\" << flush;
      sleep(1);
      cout << "\b|"  << flush;
      sleep(1);
      cout << "\b/"  << flush;
      sleep(1);
      cout << "\b-"  << flush;
      
      if (i == 2)
      {
         break;
      }
   }
}
