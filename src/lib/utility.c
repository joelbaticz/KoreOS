
#include "kldr.h"

static CHAR buffer[256]= {0};

PCHAR GetFormattedText(PCHAR pDestination, char* Format, ...)
{
	va_list ap;
	size_t length;
	
	
	PCHAR pBuffer;
	
	for (int i = 0; i < sizeof(buffer) - 1; i++)
	{
		buffer[i] = '/';
 	}
	
	pBuffer = (char*) buffer;
	
	va_start(ap, Format);
	length = vsnprintf(buffer, sizeof(buffer), Format, ap);
	va_end(ap);
	
    strcpy(&pDestination[0], (const char*) pBuffer);
    length = strlen(pBuffer);
    pDestination[length] = '\0';

    return pDestination;
}