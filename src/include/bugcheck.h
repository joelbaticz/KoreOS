#ifndef __BUGCHECK_H
#define __BUGCHECK_H

VOID BugCheckInit();
VOID BugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PCHAR pExtra, UINT32 extraValue);

#endif /* __BUGCHECK_H */