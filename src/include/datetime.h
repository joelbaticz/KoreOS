#ifndef __DATETIME_H
#define __DATETIME_H

typedef struct _DATETIME
{
	UINT32 Year;
	UINT32 Month;
	UINT32 Day;
    UINT32 DayOfWeek;
	
	UINT32 Hour;
	UINT32 Minute;
	UINT32 Second;
	UINT32 MilliSecond;

    UINT32 UtcDateTime;
} DATETIME, *PDATETIME;

VOID DateTimeInit();
VOID GetSystemDateTime(PDATETIME pDateTime);
UINT32 GetSystemDateTimeUtc();
UINT32 GetSystemHighFrequencyCounter();
VOID IncrementDateTime();

#endif /* __DATETIME_H */