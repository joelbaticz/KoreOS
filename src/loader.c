#include <kldr.h> //KLoader System Headers (including K Standard Types)

//__attribute__ ((dllexport))
VOID __cdecl KLoaderMain(IN PCHAR pCommandLineOptions)
{
	IsWindowingInitialized = 0;

	UiInit(FALSE);
	DebugInit(DEBUG_SETTING_NONE);
	MachineInit();
	UiInit(TRUE);

	DateTimeInit();
	MemoryManagerInit();
	ResourceManagerInit();

	PitDriverInit();
	PicDriverInit();
	VgaDriverInit();
	KeyboardDriverInit();
	MouseDriverInit();

	DefaultFontInit();
	LogoBitmapInit();
	LogoIconInit();

	GraphicalInterfaceManagerInit();
	SystemLookAndFeelInit();

	WindowingInit();
	WindowingStart();

	//MemoryDisplayFreeList();
	//MemoryDisplayUsedList();

	IoManagerInit();
	MessagingInit();

	MachineEnableInterrupts();

	//GiTest();
	RegisterLauncherWindow();
	RegisterWelcomeWindow();
	RegisterAboutWindow();
	RegisterSystemPropertiesWindow();
	RegisterPaletteWindow();
	RegisterMemoryMapWindow();
	RegisterUiTestWindow();
	// RegisterMystifyWindow();
	// RegisterClockWindow();
	
	CreateDesktop();
	CreateLauncherWindow();	
	CreateUiTestWindow();	
	// CreateWelcomeWindow();
	// CreateAboutWindow();
	// CreateSystemPropertiesWindow();
	// CreatePaletteWindow();
	// CreateMystifyWindow();

    SetMousePosition(160, 100);
    ShowMouse();

 	MessageLoop();

	HALT;
}
