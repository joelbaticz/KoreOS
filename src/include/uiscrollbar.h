#ifndef __UISCROLLBAR_H
#define __UISCROLLBAR_H

VOID RegisterScrollbar();
HWINDOW CreateScrollbar(PCHAR pName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags);

INT32 GetScrollSmallChange(HWINDOW hWindow);
INT32 GetScrollLargeChange(HWINDOW hWindow);
INT32 GetScrollMinimumPosition(HWINDOW hWindow);
INT32 GetScrollMaximumPosition(HWINDOW hWindow);
INT32 GetScrollCurrentPosition(HWINDOW hWindow);
INT32 GetScrollTrackingPosition(HWINDOW hWindow);

VOID SetScrollSmallChange(HWINDOW hWindow, INT32 value);
VOID SetScrollLargeChange(HWINDOW hWindow, INT32 value);
VOID SetScrollMinimumPosition(HWINDOW hWindow, INT32 position);
VOID SetScrollMaximumPosition(HWINDOW hWindow, INT32 position);
VOID SetScrollCurrentPosition(HWINDOW hWindow, INT32 position);
VOID SetScrollTrackingPosition(HWINDOW hWindow, INT32 position);

#endif /* __UISCROLLBAR_H */