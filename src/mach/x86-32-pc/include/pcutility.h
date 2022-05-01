/*
  - Date: 2018-03-18
  - Author: Joel
  - Purpose: Definitions for PC Utility functions
*/

/* GLOBAL DEFINITIONS */

/* GLOBAL VARIABLES */

/* GLOBAL FUNCTIONS */
VOID PcUtilityBeep();
UINT32 PcUtilityGetDateTime();
VOID PcUtilitySetDateTime(UINT32 dateTime);
VOID PcUtilitySetStatusLed(UINT8 ledId);
BOOL PcUtilityCheckProcessorCompatibility();
VOID PcUtilityDelay(UINT32 milliSecs);
VOID PcUtilityPause();
VOID PcUtilityHalt();
VOID PcUtilityReboot();


