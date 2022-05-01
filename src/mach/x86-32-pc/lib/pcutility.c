/*
  - Date: 2018-03-18
  - Author: Joel
  - Purpose: PC Utility implementation:
			 Machine specific utility functions.
*/

/* INCLUDES */
#include <kldr.h>

/* PRIVATE DEFINITIONS */

/* PRIVATE VARIABLES */

/* PRIVATE FUNCTIONS */
UINT32 FromBCD(UINT32 valueInBcd);
UINT32 CalculateUtc(UINT32 years, UINT32 months, UINT32 days, UINT32 hours, UINT32 minutes, UINT32 seconds);

/* GLOBAL FUNCTIONS */
VOID PcUtilityBeep()
{
	NOT_IMPLEMENTED;
}

UINT32 PcUtilityGetDateTime()
{
	//NOT_IMPLEMENTED;
	UINT32 years;
	UINT32 months;
	UINT32 days;
	UINT32 hours;
	UINT32 minutes;
	UINT32 seconds;

	// GET DATE

	/*
		INT 1Ah - Date and Time Bios services
		
		On entry:
			AH = 04h get bios date
			AL = 0
		On return:
			Carry Flag not set
			CH = Century (19 or 20)
			CL = Years
			DH = Months
			DL = Days

			All in BCD format
	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = 0x04 << 8;
	regs.EBX = 0;
	regs.ECX = 0;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x1A, pRegs, pRegs);

	years = FromBCD(regs.ECX >> 8 & 0xFF) * 100 + FromBCD(regs.ECX & 0xFF);
	months = FromBCD((regs.EDX >> 8) & 0xFF);
	days = FromBCD(regs.EDX & 0xFF);

	// GET TIME

	/*
		INT 1Ah - Date and Time Bios services
		
		On entry:
			AH = 02h get bios time
			AL = 0
		On return:
			Carry Flag not set
			CH = Hours
			CL = Mins
			DH = Seconds

			All in BCD format
	*/	
	
	regs.EAX = 0x02 << 8;
	regs.EBX = 0;
	regs.ECX = 0;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x1A, pRegs, pRegs);

	hours = FromBCD((regs.ECX >> 8) & 0xFF);
	minutes = FromBCD(regs.ECX & 0xFF);
    seconds = FromBCD((regs.EDX >> 8) & 0xFF);


//years = 2022;
//months = 1;
//days = 15;
// hours = 22;
// minutes = 57;
// seconds = 0;


	return CalculateUtc(years, months, days, hours, minutes, seconds);
}

UINT32 FromBCD(UINT32 valueInBcd)
{
	return (valueInBcd >> 4 & 0x0F) * 10 + (valueInBcd & 0x0F);
}

UINT32 CalculateUtc(UINT32 years, UINT32 months, UINT32 days, UINT32 hours, UINT32 minutes, UINT32 seconds)
{
	years -= 1970;
	months -= 1;    // January should be 0.
	days -= 1;      // 1st should be 0.
	UINT32 utcDateTime = 0;
	UINT32 numberOfLeapYears = years / 4;// - years / 100;// + years / 400;
	UINT32 monthsInDays = 0;

	if (months > 0) monthsInDays += 31;  // Jan
	if (months > 1) monthsInDays += 28;  // Fen
	if (months > 2) monthsInDays += 31;  // Mar
	if (months > 3) monthsInDays += 30;  // Apr
	if (months > 4) monthsInDays += 31;  // May
	if (months > 5) monthsInDays += 30;  // Jun
	if (months > 6) monthsInDays += 31;  // Jul
	if (months > 7) monthsInDays += 31;  // Aug
	if (months > 8) monthsInDays += 30;  // Sep
	if (months > 9) monthsInDays += 31;  // Oct
	if (months > 10) monthsInDays += 30; // Nov

	days += numberOfLeapYears;

	utcDateTime += seconds;
	utcDateTime += minutes * 60;
	utcDateTime += hours * 60 * 60;

	utcDateTime += days * 24 * 60 * 60;
	utcDateTime += monthsInDays * 24 * 60 * 60;
	utcDateTime += years * 365 * 24 * 60 * 60;

	//utcDateTime -= 719561;

	return utcDateTime;	

//    UINT32 y;
//    UINT32 m;
//    UINT32 d;
//    UINT32 t;
 
//    //Year
//    y = years;
//    //Month of year
//    m = months;
//    //Day of month
//    d = days;
 
//    //January and February are counted as months 13 and 14 of the previous year
//    if(m <= 2)
//    {
//       m += 12;
//       y -= 1;
//    }
 
//    //Convert years to days
//    t = (365 * y) + (y / 4);// - (y / 100) + (y / 400);
//    //Convert months to days
//    t += (30 * m) + (3 * (m + 1) / 5) + d;
//    //Unix time starts on January 1st, 1970
//    t -= 719561;
//    //Convert days to seconds
//    t *= 86400;
//    //Add hours, minutes and seconds
//    t += (3600 * hours) + (60 * minutes) + seconds;
 
//    //Return Unix time
//    return t;
}

VOID PcUtilitySetDateTime(UINT32 dateTime)
{
	NOT_IMPLEMENTED;
}

VOID PcUtilitySetStatusLed(UINT8 ledId)
{
	NOT_IMPLEMENTED;
}

BOOL PcUtilityCheckProcessorCompatibility()
{
	return TRUE;
}

VOID PcUtilityDelay(UINT32 milliSecs)
{
	NOT_IMPLEMENTED;
}

VOID PcUtilityPause()
{
	asm("xchg %bx, %bx");
}

VOID PcUtilityHalt()
{
	for(;;);
}

VOID PcUtilityReboot()
{
	PcBiosReboot();
}
