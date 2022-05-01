#ifndef __UITEXTBOX_H
#define __UITEXTBOX_H

VOID RegisterTextBox();
HWINDOW CreateTextBox(PCHAR pText, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags);

#endif /* __UITEXTBOX_H */