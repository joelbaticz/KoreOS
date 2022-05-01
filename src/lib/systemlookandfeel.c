#include "kldr.h"

VOID KoreOSColorsInit();
VOID Windows2012ServerColorsInit();
VOID Windows2012ServerMetricsInit();

VOID Windows31ColorsInit();
VOID Windows31MetricsInit();

VOID Windows95ColorsInit();
VOID Windows95MetricsInit();

VOID SystemLookAndFeelInit()
{
    UiDrawText("SystemLookAndFeelInit()");
    //KoreOSColorsInit();
    //Windows2012ServerColorsInit();
    //Windows2012ServerMetricsInit();
    //Windows31ColorsInit();
    //Windows31MetricsInit();
    Windows95ColorsInit();
    Windows95MetricsInit();

}

VOID KoreOSColorsInit()
{
    // Desktop
    ColorDesktop                        = 0xFFA0A0A0; //0xFF142350;
    ColorDesktopText                    = 0xFFFFFFFF;

    // Window
    ColorWindowActiveBorder             = 0xFF42AAE7;
    ColorWindowActiveFrame              = 0xFF114A82;
    ColorWindowActiveTitleBar           = 0xFF42AAE7;
    ColorWindowActiveText               = 0xFFFFFFFF;
    ColorWindowInactiveBorder           = 0xFFFFFFFF;
    ColorWindowInactiveFrame            = 0xFF000000;
    ColorWindowInactiveTitleBar         = 0xFFFFFFFF;
    ColorWindowInactiveText             = 0xFF000000;

    // Control
    ColorControlContentArea             = GetColorFromComponents(255, 255, 255);
    ColorControlContentAreaDisabled     = GetColorFromComponents(190, 190, 190);
    ColorControlContentAreaSelected     = GetColorFromComponents(0, 0, 128);
    ColorControlContentAreaText         = GetColorFromComponents(0, 0, 0);
    ColorControlContentAreaTextDisabled = GetColorFromComponents(128, 128, 128);
    ColorControlContentAreaTextSelected = GetColorFromComponents(255, 255, 255);


    ColorControlEdgeLighter             = 0xFFFFFFFF;
    ColorControlEdgeLight               = 0xFFF0F0F0;
    ColorControlEdgeDark                = 0xFF808080;
    ColorControlEdgeDarker              = 0xFF000000;

    ColorControlFrameNormal             = 0xFF000000;
    ColorControlFrameDisabled           = 0xFF000000;
    ColorControlFrameFocused            = 0xFF000000;
    ColorControlFrameOnHover            = 0xFF000000;

    ColorControlSurfaceNormal           = 0xFFFCFCFC;
    ColorControlSurfaceDisabled         = 0xFF000000;
    ColorControlSurfaceFocused          = 0xFF000000;
    ColorControlSurfaceOnHover          = 0xFF000000;

    ColorControlTextNormal              = 0xFF000000;
    ColorControlTextDisabled            = 0xFF000000;
    ColorControlTextFocused             = 0xFF000000;
    ColorControlTextOnHover             = 0xFF0000FF;
}

VOID Windows2012ServerColorsInit()
{
    // Desktop
    ColorDesktop                        = GetColorFromComponents(55, 55, 55);
    ColorDesktopText                    = GetColorFromComponents(255, 255, 255);

    // Window
    ColorWindowActiveBorder             = GetColorFromComponents(102, 203, 234); // GetColorFromComponents(154, 214, 230); //GetColorFromComponents(194, 214, 220);
    ColorWindowActiveFrame              = GetColorFromComponents(0, 0, 0);
    ColorWindowActiveTitleBar           = GetColorFromComponents(102, 203, 234); // GetColorFromComponents(154, 214, 230); //GetColorFromComponents(194, 214, 220);
    ColorWindowActiveTitleBarGradient   = GetColorFromComponents(102, 203, 234); // GetColorFromComponents(0, 0, 128);
    ColorWindowActiveText               = GetColorFromComponents(0, 0, 0);

    ColorWindowInactiveBorder           = GetColorFromComponents(255, 255, 255);
    ColorWindowInactiveFrame            = GetColorFromComponents(93, 96, 102);
    ColorWindowInactiveTitleBar         = GetColorFromComponents(255, 255, 255);
    ColorWindowInactiveTitleBarGradient = GetColorFromComponents(255, 255, 255); // GetColorFromComponents(0, 0, 128);
    ColorWindowInactiveText             = GetColorFromComponents(0, 0, 0);

    // Control
    ColorControlContentArea             = GetColorFromComponents(255, 255, 255);
    ColorControlContentAreaDisabled     = GetColorFromComponents(190, 190, 190);
    ColorControlContentAreaSelected     = GetColorFromComponents(0, 0, 128);
    ColorControlContentAreaText         = GetColorFromComponents(0, 0, 0);
    ColorControlContentAreaTextDisabled = GetColorFromComponents(128, 128, 128);
    ColorControlContentAreaTextSelected = GetColorFromComponents(255, 255, 255);

    ColorControlEdgeLighter             = GetColorFromComponents(112, 112, 112);
    ColorControlEdgeLight               = GetColorFromComponents(112, 112, 112);
    ColorControlEdgeDark                = GetColorFromComponents(112, 112, 112);
    ColorControlEdgeDarker              = GetColorFromComponents(112, 112, 112);

    ColorControlFrameNormal             = GetColorFromComponents(112, 112, 112);
    ColorControlFrameDisabled           = GetColorFromComponents(173, 178, 181);
    ColorControlFrameFocused            = GetColorFromComponents(60, 127, 177);
    ColorControlFrameOnHover            = GetColorFromComponents(111, 138, 157);

    ColorControlSurfaceNormal           = GetColorFromComponents(221, 221, 221);
    ColorControlSurfaceDisabled         = GetColorFromComponents(244, 244, 244);
    ColorControlSurfaceFocused          = GetColorFromComponents(221, 221, 221);
    ColorControlSurfaceOnHover          = GetColorFromComponents(175, 210, 232);
    
    ColorControlTextNormal              = GetColorFromComponents(0, 0, 0);
    ColorControlTextDisabled            = GetColorFromComponents(131, 131, 131);
    ColorControlTextFocused             = GetColorFromComponents(0, 0, 0);
    ColorControlTextOnHover             = GetColorFromComponents(0, 0, 128);
}

VOID Windows31ColorsInit()
{
    // Desktop
    ColorDesktop                        = GetColorFromComponents(190, 190, 190);
    ColorDesktopText                    = GetColorFromComponents(255, 255, 255);

    // Window
    ColorWindowActiveBorder             = GetColorFromComponents(166, 203, 243);  // GetColorFromComponents(0, 0, 128);
    ColorWindowActiveFrame              = GetColorFromComponents(0, 0, 0);
    ColorWindowActiveTitleBar           = GetColorFromComponents(166, 203, 243);  // GetColorFromComponents(0, 0, 128);
    ColorWindowActiveText               = GetColorFromComponents(0, 0, 0);
    ColorWindowInactiveBorder           = GetColorFromComponents(255, 255, 255);
    ColorWindowInactiveFrame            = GetColorFromComponents(93, 96, 102);
    ColorWindowInactiveTitleBar         = GetColorFromComponents(255, 255, 255);
    ColorWindowInactiveText             = GetColorFromComponents(0, 0, 0);

    // Control
    ColorControlContentArea             = GetColorFromComponents(255, 255, 255);
    ColorControlContentAreaDisabled     = GetColorFromComponents(190, 190, 190);
    ColorControlContentAreaSelected     = GetColorFromComponents(0, 0, 128);
    ColorControlContentAreaText         = GetColorFromComponents(0, 0, 0);
    ColorControlContentAreaTextDisabled = GetColorFromComponents(128, 128, 128);
    ColorControlContentAreaTextSelected = GetColorFromComponents(255, 255, 255);

    ColorControlEdgeLighter             = GetColorFromComponents(255, 255, 255);
    ColorControlEdgeLight               = GetColorFromComponents(205, 205, 205);
    ColorControlEdgeDark                = GetColorFromComponents(128, 128, 128);
    ColorControlEdgeDarker              = GetColorFromComponents(0, 0, 0);

    ColorControlFrameNormal             = GetColorFromComponents(112, 112, 112);
    ColorControlFrameDisabled           = GetColorFromComponents(173, 178, 181);
    ColorControlFrameFocused            = GetColorFromComponents(0, 0, 0);
    ColorControlFrameOnHover            = GetColorFromComponents(60, 107, 157);

    ColorControlSurfaceNormal           = GetColorFromComponents(190, 190, 190); // GetColorFromComponents(221, 221, 221);
    ColorControlSurfaceDisabled         = GetColorFromComponents(244, 244, 244);
    ColorControlSurfaceFocused          = GetColorFromComponents(190, 190, 190);
    ColorControlSurfaceOnHover          = GetColorFromComponents(230, 230, 230);

    ColorControlTextNormal              = GetColorFromComponents(0, 0, 0);
    ColorControlTextDisabled            = GetColorFromComponents(131, 131, 131);
    ColorControlTextFocused             = GetColorFromComponents(60, 60, 60);
    ColorControlTextOnHover             = GetColorFromComponents(0, 0, 128);
}

VOID Windows95ColorsInit()
{
    // Desktop
    //ColorDesktop                        = GetColorFromComponents(85, 170, 170); // GetColorFromComponents(0, 128, 128);
    ColorDesktop                        = GetColorFromComponents(0, 128, 128);
    //ColorDesktop                        = GetColorFromComponents(30, 30, 30); // GetColorFromComponents(0, 128, 128);
    ColorDesktopText                    = GetColorFromComponents(255, 255, 255);

    // Window
    ColorWindowActiveBorder             = GetColorFromComponents(0, 0, 128);
    ColorWindowActiveFrame              = GetColorFromComponents(0, 0, 0);
    //ColorWindowActiveTitleBar           = GetColorFromComponents(0, 0, 20);    // GetColorFromComponents(0, 0, 128);
    //ColorWindowActiveTitleBarGradient   = GetColorFromComponents(0, 0, 255); // GetColorFromComponents(15, 136, 217);
    ColorWindowActiveTitleBar           = GetColorFromComponents(0, 0, 128);
    ColorWindowActiveTitleBarGradient   = GetColorFromComponents(16, 132, 208); // GetColorFromComponents(0, 0, 128);

    ColorWindowActiveText               = GetColorFromComponents(255, 255, 255);
    ColorWindowInactiveBorder           = GetColorFromComponents(128, 128, 128);
    ColorWindowInactiveFrame            = GetColorFromComponents(0, 0, 0);
    ColorWindowInactiveTitleBar         = GetColorFromComponents(128, 128, 128);
    ColorWindowInactiveTitleBarGradient = GetColorFromComponents(192, 192, 192);
    ColorWindowInactiveText             = GetColorFromComponents(192, 192, 192);

    // Control
    ColorControlContentArea             = GetColorFromComponents(255, 255, 255);
    ColorControlContentAreaDisabled     = GetColorFromComponents(190, 190, 190);
    ColorControlContentAreaSelected     = GetColorFromComponents(0, 0, 128);
    ColorControlContentAreaText         = GetColorFromComponents(0, 0, 0);
    ColorControlContentAreaTextDisabled = GetColorFromComponents(128, 128, 128);
    ColorControlContentAreaTextSelected = GetColorFromComponents(255, 255, 255);

    ColorControlEdgeLighter             = GetColorFromComponents(255, 255, 255);
    ColorControlEdgeLight               = GetColorFromComponents(205, 205, 205);
    ColorControlEdgeDark                = GetColorFromComponents(128, 128, 128);
    ColorControlEdgeDarker              = GetColorFromComponents(0, 0, 0);

    ColorControlFrameNormal             = GetColorFromComponents(112, 112, 112);
    ColorControlFrameDisabled           = GetColorFromComponents(173, 178, 181);
    ColorControlFrameFocused            = GetColorFromComponents(0, 0, 0);
    ColorControlFrameOnHover            = GetColorFromComponents(60, 107, 157);

    ColorControlSurfaceNormal           = GetColorFromComponents(192, 192, 192);
    ColorControlSurfaceDisabled         = GetColorFromComponents(244, 244, 244);
    ColorControlSurfaceFocused          = GetColorFromComponents(192, 192, 192);
    ColorControlSurfaceOnHover          = GetColorFromComponents(230, 230, 230);

    ColorControlTextNormal              = GetColorFromComponents(0, 0, 0);
    ColorControlTextDisabled            = GetColorFromComponents(128, 128, 128);
    ColorControlTextFocused             = GetColorFromComponents(60, 60, 60);
    ColorControlTextOnHover             = GetColorFromComponents(0, 0, 128);
}

VOID Windows95MetricsInit()
{
    MetricFrameSize                     = 1;
    MetricBorderSize                    = 3;  // 7
    MetricEdgeSize                      = 3;
    MetricWindowTitleBarSize            = 18; //28 // 18 // 24
    MetricWindowCloseButtonWidth        = 16;
    MetricWindowCloseButtonHeight       = 14;
    MetricWindowCloseButtonMarginX      = 5;
    MetricWindowCloseButtonMarginY      = 5;
    MetricWindowButtonWidth             = 16;
    MetricWindowButtonHeight            = 14;
    MetricWindowButtonMarginX           = 2;
    MetricWindowButtonMarginY           = 5;
    MetricMenuBarHeight                 = 20;
    MetricMenuItemHeight                = 20;
    MetricScrollbarSize                 = 24;
    MetricCheckboxSize                  = 13;
    MetricRadiobuttonSize               = 13;
}

VOID Windows2012ServerMetricsInit()
{
    MetricFrameSize                     = 1;
    MetricBorderSize                    = 5; // 7
    MetricEdgeSize                      = 1;
    MetricWindowTitleBarSize            = 26; // 18 // 24
    MetricWindowCloseButtonWidth        = 16;
    MetricWindowCloseButtonHeight       = 14;
    MetricWindowCloseButtonMarginX      = 5;
    MetricWindowCloseButtonMarginY      = 5;
    MetricWindowButtonWidth             = 16;
    MetricWindowButtonHeight            = 14;
    MetricWindowButtonMarginX           = 2;
    MetricWindowButtonMarginY           = 5;
    MetricMenuBarHeight                 = 20;
    MetricMenuItemHeight                = 20;
    MetricScrollbarSize                 = 26;
    MetricCheckboxSize                  = 16;
    MetricRadiobuttonSize               = 16;
}

VOID Windows31MetricsInit()
{
    MetricFrameSize                     = 1;
    MetricBorderSize                    = 5; // 7
    MetricEdgeSize                      = 3;
    MetricWindowTitleBarSize            = 22; // 18 // 24
    MetricWindowCloseButtonWidth        = 16;
    MetricWindowCloseButtonHeight       = 14;
    MetricWindowCloseButtonMarginX      = 5;
    MetricWindowCloseButtonMarginY      = 5;
    MetricWindowButtonWidth             = 16;
    MetricWindowButtonHeight            = 14;
    MetricWindowButtonMarginX           = 2;
    MetricWindowButtonMarginY           = 5;
    MetricMenuBarHeight                 = 20;
    MetricMenuItemHeight                = 20;
    MetricScrollbarSize                 = 24;
    MetricCheckboxSize                  = 13;
    MetricRadiobuttonSize               = 13;
}

COLOR GetSystemColor(UINT32 index)
{
    // Desktop
    if (index == COLOR_DESKTOP) return ColorDesktop;
    if (index == COLOR_DESKTOP_TEXT) return ColorDesktopText;

    // Window
    if (index ==COLOR_WINDOW_ACTIVE_BORDER) return ColorWindowActiveBorder;
    if (index ==COLOR_WINDOW_ACTIVE_FRAME) return ColorWindowActiveFrame;
    if (index ==COLOR_WINDOW_ACTIVE_TITLE_BAR) return ColorWindowActiveTitleBar;
    if (index ==COLOR_WINDOW_ACTIVE_TITLE_BAR_GRADIENT) return ColorWindowActiveTitleBarGradient;
    if (index ==COLOR_WINDOW_ACTIVE_TEXT) return ColorWindowActiveText;
    if (index ==COLOR_WINDOW_INACTIVE_BORDER) return ColorWindowInactiveBorder;
    if (index ==COLOR_WINDOW_INACTIVE_FRAME) return ColorWindowInactiveFrame;
    if (index ==COLOR_WINDOW_INACTIVE_TITLE_BAR) return ColorWindowInactiveTitleBar;
    if (index ==COLOR_WINDOW_INACTIVE_TITLE_BAR_GRADIENT) return ColorWindowInactiveTitleBarGradient;
    if (index ==COLOR_WINDOW_INACTIVE_TEXT) return ColorWindowInactiveText;

    // Control
    if (index == COLOR_CONTROL_CONTENT_AREA) return ColorControlContentArea;
    if (index == COLOR_CONTROL_CONTENT_AREA_DISABLED) return ColorControlContentAreaDisabled;
    if (index == COLOR_CONTROL_CONTENT_AREA_SELECTED) return ColorControlContentAreaSelected;

    if (index == COLOR_CONTROL_CONTENT_AREA_TEXT) return ColorControlContentAreaText;
    if (index == COLOR_CONTROL_CONTENT_AREA_TEXT_DISABLED) return ColorControlContentAreaTextDisabled;
    if (index == COLOR_CONTROL_CONTENT_AREA_TEXT_SELECTED) return ColorControlContentAreaTextSelected;


    if (index == COLOR_CONTROL_EDGE_LIGHT) return ColorControlEdgeLight;
    if (index == COLOR_CONTROL_EDGE_LIGHTER) return ColorControlEdgeLighter;
    if (index == COLOR_CONTROL_EDGE_DARK) return ColorControlEdgeDark;
    if (index == COLOR_CONTROL_EDGE_DARKER) return ColorControlEdgeDarker;
    if (index == COLOR_CONTROL_FRAME_NORMAL) return ColorControlFrameNormal;
    if (index == COLOR_CONTROL_FRAME_DISABLED) return ColorControlFrameDisabled;
    if (index == COLOR_CONTROL_FRAME_FOCUSED) return ColorControlFrameFocused;
    if (index == COLOR_CONTROL_FRAME_ONHOVER) return ColorControlFrameOnHover;
    if (index == COLOR_CONTROL_SURFACE_NORMAL) return ColorControlSurfaceNormal;
    if (index == COLOR_CONTROL_SURFACE_DISABLED) return ColorControlSurfaceDisabled;
    if (index == COLOR_CONTROL_SURFACE_FOCUSED) return ColorControlSurfaceFocused;
    if (index == COLOR_CONTROL_SURFACE_ONHOVER) return ColorControlSurfaceOnHover;
    if (index == COLOR_CONTROL_TEXT_NORMAL) return ColorControlTextNormal;
    if (index == COLOR_CONTROL_TEXT_DISABLED) return ColorControlTextDisabled;
    if (index == COLOR_CONTROL_TEXT_FOCUSED) return ColorControlTextFocused;
    if (index == COLOR_CONTROL_TEXT_ONHOVER) return ColorControlTextOnHover;

    // Menu
    // if (index == COLOR_MENU_SURFACE) return 
    // if (index == COLOR_MENUBAR_SURFACE) return 
    // if (index == COLOR_MENU_TEXT) return 
    // if (index == COLOR_MENU_ITEM_NORMAL) return 
    // if (index == COLOR_MENU_ITEM_TEXT_NORMAL) return 
    // if (index == COLOR_MENU_ITEM_SELECTED) return 
    // if (index == COLOR_MENU_ITEM_TEXT_SELECTED) return 

    return 0;
}

UINT32 GetSystemMetric(UINT32 index)
{
    if (index == METRIC_FRAME_SIZE) return MetricFrameSize;
    if (index == METRIC_BORDER_SIZE) return MetricBorderSize;
    if (index == METRIC_EDGE_SIZE) return MetricEdgeSize;

    if (index == METRIC_WINDOW_TITLE_BAR_SIZE) return MetricWindowTitleBarSize;
    if (index == METRIC_WINDOW_CLOSE_BUTTON_WIDTH) return MetricWindowCloseButtonWidth;
    if (index == METRIC_WINDOW_CLOSE_BUTTON_HEIGHT) return MetricWindowCloseButtonHeight;
    if (index == METRIC_WINDOW_CLOSE_BUTTON_MARGIN_X) return MetricWindowCloseButtonMarginX;
    if (index == METRIC_WINDOW_CLOSE_BUTTON_MARGIN_Y) return MetricWindowCloseButtonMarginY;
    if (index == METRIC_WINDOW_BUTTON_WIDTH) return MetricWindowButtonWidth;
    if (index == METRIC_WINDOW_BUTTON_HEIGHT) return MetricWindowButtonHeight;
    if (index == METRIC_WINDOW_BUTTON_MARGIN_X) return MetricWindowButtonMarginX;
    if (index == METRIC_WINDOW_BUTTON_MARGIN_Y) return MetricWindowButtonMarginY;

    if (index == METRIC_MENU_BAR_HEIGHT) return MetricMenuBarHeight;
    if (index == METRIC_MENU_ITEM_HEIGHT) return MetricMenuItemHeight;

    if (index == METRIC_SCROLLBAR_SIZE) return MetricScrollbarSize;

    if (index == METRIC_CHECKBOX_SIZE) return MetricCheckboxSize;

    if (index == METRIC_RADIOBUTTON_SIZE) return MetricRadiobuttonSize;

    return 0;
}
