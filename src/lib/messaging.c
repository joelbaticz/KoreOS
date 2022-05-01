/*
  - Date: 2021-11-18
  - Author: Joel
  - Purpose: Message bus
*/

#include <kldr.h>

#define MAX_MESSAGE_COUNT 100

HANDLE      hMessageQueue;
PMESSAGE    pMessageQueue;
UINT32      messageCount;
UINT32      counterLast;
BOOL        messageLoopInProgress;

VOID        FillMessage(PMESSAGE pMessage, UINT32 processId, HWINDOW hWindow, UINT32 message, UINT32 param1, UINT32 param2, UINT32 timeStamp);
VOID        FillEmptyMessage(PMESSAGE pMessage);

VOID MessagingInit()
{
    hMessageQueue = MemoryAllocate(MAX_MESSAGE_COUNT * sizeof(MESSAGE), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"SystemMessageQueue");

    if (hMessageQueue == (HANDLE) NULL)
    {
        UiDrawText("System Message Queue cannot be initialized");
        HALT;
    }

    pMessageQueue = MemoryLock(hMessageQueue);

    if (hMessageQueue == (HANDLE) NULL)
    {
        UiDrawText("System Message Queue cannot be initialized");
        HALT;
    }

    hMessageQueue = (HANDLE) NULL;
    messageCount = 0;
    counterLast = 0;
}

VOID MessagingDestroy()
{
    MemoryFree(hMessageQueue);
}

// Sends a message directly to a window's window procedure to handle
UINT32 SendMessage(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    UiDrawText("SendMessage()");

    if (hWindow == (HWINDOW) NULL)
    {
        return 0;
    }

    if (!IsValid(hWindow))
    {
        BugCheck(NULL, (PCHAR) &"Window handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);   
        return 0;
    }

    PWINDOW pWindow = MemoryLock(hWindow);

    if (pWindow == (PWINDOW) NULL)
    {
        return 0;
    }

    if (message == WINDOW_MESSAGE_PAINT || message == WINDOW_MESSAGE_PAINT_NON_CLIENT)
    {
        if (!IsVisible(hWindow))
        {
            return 0;
        }
    }

    UINT32 (*windowProcedure)(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
    
    // Send message to Non-client handling function
    //windowProcedure = &ApplicationWindowProcedure;
    //windowProcedure(hWindow, message, param1, param2);

    windowProcedure = pWindow->pWindowClass->pWindowProcedure;
    return windowProcedure(hWindow, message, param1, param2);
}

// Posts a message to the message queue
// Remarks: the current timestamp and the ProcessId is added
VOID PostMessage(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (!IsValid(hWindow))
    {
        BugCheck(NULL, (PCHAR) &"Window handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);   
        return;
    }

    PMESSAGE pNextMessage = pMessageQueue;
    pNextMessage += messageCount;

    FillMessage(pNextMessage, 1, hWindow, message, param1, param2, 1000);

    messageCount++;
}

// Provides a way to get the next message for a particular window
// Remarks:
//   - hWindow can be null, in this case the next message will be returned
//   - The message will be taken out of the message queue
VOID GetMessage(OUT PMESSAGE pMessage, HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        BugCheck(NULL, (PCHAR) &"Window handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);   
        return;
    }

    FillEmptyMessage(pMessage);

    if (messageCount == 0)
    {
        return;
    }

    if (hWindow == (HWINDOW) NULL)
    {
        pMessage[0] = pMessageQueue[0];

        // Move messages up by 1
        for(UINT32 i = 0; i < messageCount - 1; i++)
        {
            pMessageQueue[i] = pMessageQueue[i + 1];
        }

        messageCount--;

        // Clear last row
        FillEmptyMessage(&pMessageQueue[messageCount]);

        return;
    }

    // Look up window handle
    INT32 matchIndex = -1;
    for(UINT32 i = 0; i < messageCount; i++)
    {
        if (pMessageQueue[i].hWindow == hWindow)
        {
            matchIndex = i;

            break;
        }
    }    

    // Match found?
    if (matchIndex == -1)
    {
        return;
    }

    pMessage[0] = pMessageQueue[matchIndex];

    // Move messages up by 1
    for(UINT32 i = matchIndex; i < messageCount - 1; i++)
    {
        pMessageQueue[i] = pMessageQueue[i + 1];
    }

    messageCount--;

    // Clear last row
    FillEmptyMessage(&pMessageQueue[messageCount]);
}

// Provides a way to look into the next message for a particular window, if there is any
// Remarks: 
//   - The struct will be filled will NULL values if there are no messages for that particual window in the queue
//   - The message is not taken out of the message queue
VOID PeekMessage(OUT PMESSAGE pMessage, HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        BugCheck(NULL, (PCHAR) &"Window handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);   
        return;
    }
    
    pMessage[0] = pMessageQueue[0];
}

// Fills in a message struct with the given parameters
VOID FillMessage(PMESSAGE pMessage, UINT32 processId, HWINDOW hWindow, UINT32 message, UINT32 param1, UINT32 param2, UINT32 timeStamp)
{
    pMessage->ProcessId = processId;
    pMessage->hWindow = hWindow;
    pMessage->Message = message;
    pMessage->Param1 = param1;
    pMessage->Param2 = param2;
    pMessage->TimeStamp = timeStamp;
}

VOID FillEmptyMessage(PMESSAGE pMessage)
{
    FillMessage(pMessage, 0, (HWINDOW) NULL, WINDOW_MESSAGE_NULL_MESSAGE, 0, 0, 0);
}

VOID MessageLoop()
{
    for(;;)
    {
        HandleKeyboardEvents();
        HandleMouseEvents();

        // UINT32 counterNow = GetSystemHighFrequencyCounter();

        // if (counterNow > counterLast)
        // {
        //     counterLast = counterNow;
        //     // DO NOT DO THIS AS THESE HANDLES MIGHT NOT BE INITIALIZED YET!
        //     //SendMessage(hMystifyWindow, WINDOW_MESSAGE_ON_TIMER, 0, 0);
        //     //SendMessage(hClockWindow, WINDOW_MESSAGE_ON_TIMER, 0, 0);
        //     //BroadcastMessage(hDesktopWindow, WINDOW_MESSAGE_ON_SYSTEM_TICK);
        // }        

        // if (IsMystifyWindowCreated == 0xBAADB00B)
        // {
        //     //SendMessage(hMystifyWindow, WINDOW_MESSAGE_ON_TIMER, 0, 0);
        // }

        // if (MemoryMapWindowCreatedHash == 0xBAADB005)
        // {
        //     HWINDOW hScrollbar = GetFirstChild(hMemoryMapWindow);

        //     if (IsValid(hScrollbar))
        //     {
        //         SendMessage(hScrollbar, WINDOW_MESSAGE_ON_SYSTEM_TICK, 0, 0);
        //     }                
        // }

    //BroadcastMessage(hDesktopWindow, WINDOW_MESSAGE_ON_SYSTEM_TICK);

        //SendMessage(hClockWindow, WINDOW_MESSAGE_ON_TIMER, 0, 0);
        // // Get next event from queue and handle it
    }
}

