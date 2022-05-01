// This file containes no longer used parts

//loader.c

PCHAR ptrVideo = (PCHAR)0xb8000;

VOID MyPutChar(char chr)
{
	if (chr == 0) chr='|';
	
	//if (chr)
	{
		*ptrVideo = chr;
		ptrVideo+=2;
	}
}

void MyPrintf(char* str)
{
	if (str)
	{
		while(str[0] != '\0')
		{
			*ptrVideo = *str;
			ptrVideo+=2;
			str++;
		}
	}
}

void clrscr()
{
	for (size_t i = 0; i < 80 * 25; i++)
	{
		*ptrVideo = ' ';
		ptrVideo++;
		*ptrVideo = 0x5e;
		ptrVideo++;
	}	
	ptrVideo = (PCHAR) 0xb8000;
}

/*
__attribute__ ((dllexport)) uint32_t __cdecl GetSystemTime()
{	
}

__attribute__ ((dllexport)) VOID __cdecl SetSystemTime(uint32_t NewTime)
{	
}
*/


//loader.c KLoaderMain
	//MENU
	// UiDrawBox(25, 7, 55, 17, 0x1f, LINE_STYLE_DOUBLE, "  Loader Menu  ");
	// UiDrawTextCenter(20, 10, 60, 14, COLOR_DO_NOT_CARE, "Please wait...");
	// UiDrawStatusBar("Use the up and down arrow keys to select an option from the menu.", "            ");
	
	// UiDrawButton(5, 10, 25, 10, "Disabled", FALSE, FALSE, FALSE, FALSE);
	// UiDrawButton(30, 10, 55, 10, "Enabled", TRUE, FALSE, FALSE, FALSE);
	// UiDrawButton(5, 15, 25, 15, "Default", TRUE, TRUE, FALSE, FALSE);
	// UiDrawButton(30, 15, 55, 15, "Selected", TRUE, FALSE, TRUE, FALSE);
	// UiDrawButton(5, 20, 25, 20, "Disabled", FALSE, TRUE, TRUE, TRUE);
	// UiDrawButton(30, 20, 55, 20, "Pressed", TRUE, FALSE, FALSE, TRUE);
	
	
	
	//UiDrawMessageBox(5, 5, 25, 15, " MessageBox ");
	//UiDrawInfoBox(35, 5, 55, 15, " InfoBox ");
	//UiDrawCriticalMessageBox(5, 17, 40, 21, " CriticalBox ");
	
	//TRACE("YEEEEEEEEEEEE");
	//LOADER OPTIONS AREA - END	
	
	//InitializeMemMan();
	//MemMan is available. Entire pages, aligned blocks from heap are possible to be allocated. Uses Bios 820h to query memory;
	
	//InitializeIoMan();
	//Estabilishes boot path, queries bios devices, gets ready the file system
	
	//Return to REAL MODE
	//Actually never return since it would halt anyways...





//kldr.h
VOID MyPutChar(char chr);
void MyPrintf(char* str);



