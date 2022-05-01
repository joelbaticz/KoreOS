/*
  - Date: 2021-11-18
  - Author: Joel
  - Purpose: Message bus
*/

#ifndef __MESSAGING_H
#define __MESSAGING_H

typedef struct _MESSAGE
{
    UINT32 ProcessId;
    HWINDOW hWindow;
    UINT32 Message;
    UINT32 Param1;
    UINT32 Param2;
    UINT32 TimeStamp;
} MESSAGE, *PMESSAGE;

VOID MessagingInit();
VOID MessagingDestroy();

UINT32 SendMessage(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID PostMessage(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID GetMessage(OUT PMESSAGE pMessage, HWINDOW hWindow);
VOID PeekMessage(OUT PMESSAGE pMessage, HWINDOW hWindow);

VOID MessageLoop();

#endif // __MESSAGING_H