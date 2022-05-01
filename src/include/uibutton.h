#ifndef __UIBUTTON_H
#define __UIBUTTON_H

VOID RegisterButton();
HWINDOW CreateButton(PCHAR pName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags);

BOOL ButtonGetEnabled(HWINDOW hButton);
BOOL ButtonGetChecked(HWINDOW hButton);
UINT32 ButtonGetState(HWINDOW hButton);
BOOL ButtonGetFocus(HWINDOW hButton);
BOOL ButtonGetDefault(HWINDOW hButton);
VOID ButtonGetText(HWINDOW hButton, PCHAR pTextBuffer, UINT32 bufferSize);

VOID ButtonSetEnabled(HWINDOW hButton, BOOL isEnabled);
VOID ButtonSetChecked(HWINDOW hButton, BOOL isChecked);
VOID ButtonSetState(HWINDOW hButton, UINT32 stateFlags);
VOID ButtonSetFocus(HWINDOW hButton);
VOID ButtonSetDefault(HWINDOW hButton);
VOID ButtonSetText(HWINDOW hButton, PCHAR pText);

#endif /* __UIBUTTON_H */