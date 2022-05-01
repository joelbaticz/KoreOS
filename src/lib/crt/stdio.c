//C language runtime Standard IO implementation
//#include <string.h>
//#include <ktypes.h>
#include <kldr.h>

/* Not standard C string functions */
int vsnprintf(char* buffer, size_t size, const char* format, va_list args)
{
	/* Sends formatted output to a buffer using an argument list. */

	size_t count;	
	char* ptr = (char*) format;
	char tempBuffer[32];
	char* paramBuffer = (char*) tempBuffer;
	char paramLength;
	_Bool endOfBuffer;
		
	/* Make space for the last character */
	//size--;
	
	count = 0;
	while(*ptr != '\0')// && count <= size)
	{
		
		
		if (*ptr == '%')
		{
			/* Handle parameter */
			
			/* Skip % char */
			ptr++;
			
			if (*ptr == 'c')
			{
				char c = va_arg(args, char);
				*paramBuffer = c;
				paramLength = sizeof(c);				
			}
			if (*ptr == 's')
			{
				char* c = va_arg(args, char*);
				paramBuffer = c;
				paramLength = strlen(paramBuffer);
			}
			if (*ptr == 'd' || *ptr == 'i')
			{
				int c = va_arg(args, int);
				itoa(c, paramBuffer, 10);
				paramLength = strlen(paramBuffer);
			}
			if (*ptr == 'u')
			{
				int c = va_arg(args, int);
				uitoa(c, paramBuffer, 10);
				paramLength = strlen(paramBuffer);
			}
			if (*ptr == 'x' || *ptr == 'X')
			{
				unsigned int c = va_arg(args, unsigned int);
				uitoa(c, paramBuffer, 16);
				paramLength = strlen(paramBuffer);
			}
			if (*ptr == 'h')
			{
				unsigned char c = va_arg(args, unsigned char);
				uitoa(c, paramBuffer, 16);
				paramLength = strlen(paramBuffer);
			}
			
			/* Skip formatting char */
			ptr++;
		}
		else
		{
			/* If normal character */
			*paramBuffer = *ptr;
			ptr++;
			paramLength = 1;
		}



		if (count + paramLength <= size) // <= ?
		{
			for(int i = 0; i < paramLength; i++)
			{
				*buffer = *paramBuffer;
				buffer++;
				paramBuffer++;
			}			
		}
		else
		{
			if (endOfBuffer == FALSE)
			{
				buffer--;
		//		*buffer = '\0';
				endOfBuffer = TRUE;
			}
		}

		count+=paramLength;		
		paramBuffer=(char*) tempBuffer;
	}
	
	//buffer--;
	//*buffer = '\0';
	buffer[0] = '\0';
	
	return count;
	
}






