#include <kldr.h>

// January – 31 days
// February – 28 days in a common year and 29 days in leap years
// March – 31 days
// April – 30 days
// May – 31 days
// June – 30 days
// July – 31 days
// August – 31 days
// September – 30 days
// October – 31 days
// November – 30 days
// December – 31 day

static UINT32 utcDateTime;
static UINT32 millisecondsPassed;
static UINT32 highFrequencyCounter;

VOID AquireDateTime();

VOID DateTimeInit()
{
    UiDrawText("DateTimeInit()");
    AquireDateTime();

    millisecondsPassed = 0;
}

UINT32 GetSystemDateTimeUtc()
{
    //LoaderShowVariableNameAndValue("UTCDateTime", utcDateTime, 10);

    return utcDateTime;
}

UINT32 GetSystemHighFrequencyCounter()
{
    return highFrequencyCounter;
}

VOID AquireDateTime()
{
    utcDateTime = MachineUtilityGetDateTime();
}

VOID GetSystemDateTime(PDATETIME pDateTime)
{
   uint32_t t = utcDateTime; 
   uint32_t a;
   uint32_t b;
   uint32_t c;
   uint32_t d;
   uint32_t e;
   uint32_t f;
 
   //Negative Unix time values are not supported
   if(t < 1)
      t = 0;
 
   //Clear milliseconds
   pDateTime->MilliSecond = 0;
 
   //Retrieve hours, minutes and seconds
   pDateTime->Second = t % 60;
   t /= 60;
   pDateTime->Minute = t % 60;
   t /= 60;
   pDateTime->Hour = t % 24;
   t /= 24;
 
   //Convert Unix time to date
   a = (uint32_t) ((4 * t + 102032) / 146097 + 15);
   b = (uint32_t) (t + 2442113 + a - (a / 4));
   c = (20 * b - 2442) / 7305;
   d = b - 365 * c - (c / 4);
   e = d * 1000 / 30601;
   f = d - e * 30 - e * 601 / 1000;
 
   //January and February are counted as months 13 and 14 of the previous year
   if(e <= 13)
   {
      c -= 4716;
      e -= 1;
   }
   else
   {
      c -= 4715;
      e -= 13;
   }
 
   //Retrieve year, month and day
   pDateTime->Year = c;
   pDateTime->Month = e;
   pDateTime->Day = f;

   pDateTime->UtcDateTime = utcDateTime;
}

VOID IncrementDateTime()
{
    millisecondsPassed += 5;  // 1000 / 18.2 = 54.9
    highFrequencyCounter += 5;

    if (millisecondsPassed >= 1000)
    {
        utcDateTime++;
        millisecondsPassed = 0;
    }
}
