/*
  - Date: 2021-08-18
  - Author: Joel
  - Purpose: Prommable Interrupt Contoller driver
  - Remark: Maybe it should be the part of Machine specific stuff
*/

#ifndef __PIC_H
#define __PIC_H

VOID PicDriverInit();

VOID PicMapSystemIRQs();
VOID PicMapBiosIRQs();
VOID PicSaveBiosIRQs();
VOID PicRestoreBiosIRQs();
VOID PicDisableIRQs();
VOID PicEnableSpecificIRQs();

#endif /* __PIC_H */