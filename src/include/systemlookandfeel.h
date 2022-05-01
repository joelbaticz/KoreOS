/*
  - Date: 2021-11-20
  - Author: Joel
  - Purpose: System colors
*/

#ifndef __SYSTEMLOOKANDFEEL_H
#define __SYSTEMLOOKANDFEEL_H

// Desktop
#define COLOR_DESKTOP                             0x0000
#define COLOR_DESKTOP_TEXT                        0x0001

// Window
#define COLOR_WINDOW_ACTIVE_BORDER                0x0010
#define COLOR_WINDOW_ACTIVE_FRAME                 0x0011
#define COLOR_WINDOW_ACTIVE_TITLE_BAR             0x0012
#define COLOR_WINDOW_ACTIVE_TEXT                  0x0013
#define COLOR_WINDOW_INACTIVE_BORDER              0x0014
#define COLOR_WINDOW_INACTIVE_FRAME               0x0015
#define COLOR_WINDOW_INACTIVE_TITLE_BAR           0x0016
#define COLOR_WINDOW_INACTIVE_TEXT                0x0017

#define COLOR_WINDOW_ACTIVE_TITLE_BAR_GRADIENT    0x0018
#define COLOR_WINDOW_INACTIVE_TITLE_BAR_GRADIENT  0x0019

// Content Area
#define COLOR_CONTROL_CONTENT_AREA                0x0020 // Checkbox, Radiobutton, Textbox, Combobox background
#define COLOR_CONTROL_CONTENT_AREA_DISABLED       0x0021 // Textbox background
#define COLOR_CONTROL_CONTENT_AREA_SELECTED       0x0022 // Textbox background

#define COLOR_CONTROL_CONTENT_AREA_TEXT           0x0023 // Textbox glyph & text
#define COLOR_CONTROL_CONTENT_AREA_TEXT_DISABLED  0x0024 // Textbox glyph & text
#define COLOR_CONTROL_CONTENT_AREA_TEXT_SELECTED  0x0025 // Textbox glyph & text

#define COLOR_CONTROL_EDGE_LIGHT                  0x0030
#define COLOR_CONTROL_EDGE_LIGHTER                0x0031
#define COLOR_CONTROL_EDGE_DARK                   0x0032
#define COLOR_CONTROL_EDGE_DARKER                 0x0033
#define COLOR_CONTROL_FRAME_NORMAL                0x0034
#define COLOR_CONTROL_FRAME_DISABLED              0x0035
#define COLOR_CONTROL_FRAME_FOCUSED               0x0036
#define COLOR_CONTROL_FRAME_ONHOVER               0x0037
#define COLOR_CONTROL_SURFACE_NORMAL              0x0038
#define COLOR_CONTROL_SURFACE_DISABLED            0x0039
#define COLOR_CONTROL_SURFACE_FOCUSED             0x003A
#define COLOR_CONTROL_SURFACE_ONHOVER             0x003B
#define COLOR_CONTROL_TEXT_NORMAL                 0x003C
#define COLOR_CONTROL_TEXT_DISABLED               0x003D
#define COLOR_CONTROL_TEXT_FOCUSED                0x003E
#define COLOR_CONTROL_TEXT_ONHOVER                0x003F

// Menu
#define COLOR_MENU_SURFACE                        0x0040
#define COLOR_MENUBAR_SURFACE                     0x0041
#define COLOR_MENU_TEXT                           0x0042
#define COLOR_MENU_ITEM_NORMAL                    0x0043
#define COLOR_MENU_ITEM_TEXT_NORMAL               0x0044
#define COLOR_MENU_ITEM_SELECTED                  0x0045
#define COLOR_MENU_ITEM_TEXT_SELECTED             0x0046

// Metrics
#define METRIC_FRAME_SIZE                         0x0001
#define METRIC_BORDER_SIZE                        0x0002
#define METRIC_EDGE_SIZE                          0x0003
#define METRIC_WINDOW_TITLE_BAR_SIZE              0x0004
#define METRIC_WINDOW_CLOSE_BUTTON_WIDTH          0x0005
#define METRIC_WINDOW_CLOSE_BUTTON_HEIGHT         0x0006
#define METRIC_WINDOW_CLOSE_BUTTON_MARGIN_X       0x0007
#define METRIC_WINDOW_CLOSE_BUTTON_MARGIN_Y       0x0008
#define METRIC_WINDOW_BUTTON_WIDTH                0x0009
#define METRIC_WINDOW_BUTTON_HEIGHT               0x000A
#define METRIC_WINDOW_BUTTON_MARGIN_X             0x000B
#define METRIC_WINDOW_BUTTON_MARGIN_Y             0x000C
#define METRIC_MENU_BAR_HEIGHT                    0x000D
#define METRIC_MENU_ITEM_HEIGHT                   0x000E
#define METRIC_SCROLLBAR_SIZE                     0x000F
#define METRIC_CHECKBOX_SIZE                      0x0010
#define METRIC_RADIOBUTTON_SIZE                   0x0020

// Desktop
COLOR ColorDesktop;
COLOR ColorDesktopText;

// Window
COLOR ColorWindowActiveBorder;
COLOR ColorWindowActiveFrame;
COLOR ColorWindowActiveTitleBar;
COLOR ColorWindowActiveTitleBarGradient;
COLOR ColorWindowActiveText;
COLOR ColorWindowInactiveBorder;
COLOR ColorWindowInactiveFrame;
COLOR ColorWindowInactiveTitleBar;
COLOR ColorWindowInactiveTitleBarGradient;
COLOR ColorWindowInactiveText;

// Control
COLOR ColorControlContentArea;              // We might want to have different colors for the different states
COLOR ColorControlContentAreaDisabled;
COLOR ColorControlContentAreaSelected;
COLOR ColorControlContentAreaText;
COLOR ColorControlContentAreaTextDisabled;
COLOR ColorControlContentAreaTextSelected;

COLOR ColorControlEdgeLight;
COLOR ColorControlEdgeLighter;
COLOR ColorControlEdgeDark;
COLOR ColorControlEdgeDarker;

COLOR ColorControlFrameNormal;
COLOR ColorControlFrameDisabled;
COLOR ColorControlFrameFocused;
COLOR ColorControlFrameOnHover;

COLOR ColorControlSurfaceNormal;
COLOR ColorControlSurfaceDisabled;
COLOR ColorControlSurfaceFocused;
COLOR ColorControlSurfaceOnHover;

COLOR ColorControlTextNormal;
COLOR ColorControlTextDisabled;
COLOR ColorControlTextFocused;
COLOR ColorControlTextOnHover;

// Metrics
UINT32 MetricFrameSize;
UINT32 MetricBorderSize;
UINT32 MetricEdgeSize;

UINT32 MetricWindowTitleBarSize;
UINT32 MetricWindowCloseButtonWidth;
UINT32 MetricWindowCloseButtonHeight;
UINT32 MetricWindowCloseButtonMarginX;
UINT32 MetricWindowCloseButtonMarginY;
UINT32 MetricWindowButtonWidth;
UINT32 MetricWindowButtonHeight;
UINT32 MetricWindowButtonMarginX;
UINT32 MetricWindowButtonMarginY;

UINT32 MetricMenuBarHeight;
UINT32 MetricMenuItemHeight;

UINT32 MetricScrollbarSize;
UINT32 MetricCheckboxSize;
UINT32 MetricRadiobuttonSize;

VOID   SystemLookAndFeelInit();
COLOR  GetSystemColor(UINT32 index);
UINT32 GetSystemMetric(UINT32 index);

#endif // __SYSTEMLOOKANDFEEL_H