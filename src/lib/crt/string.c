//C language runtime String implementation


/* INCLUDES */
#include <kldr.h>

char* strcat(char* destination, const char* source)
{
	size_t i = 0;
	size_t length = strlen(destination);
		
	while(source[i] != 0)
	{
		destination[length + i] = source[i];
		i++;
	};
	destination[length + i] = 0;
	
	return destination;
}

size_t strlen(const char* str)
{
	size_t i = 0;
	
	while(str[i] != 0)
	{
		i++;
	}

	return i;	
}

char* strchr(char * str, int character)
{
	size_t length = strlen(str);
	
	for(size_t i = 0; i < length; i++)
	{
		if (str[i] == (char) character) return &str[i]; 
	}
	
	return NULL;	
}

char* strstr(char* str1, const char *str2)
{
	size_t length = strlen(str2);
	const char* ref = str2;
	
	if (!str1) return NULL;
	if (!str2) return NULL;
	
	while(str1[0] != '\0' && ref[0] != '\0')
	{
		if (str1[0] == ref[0])
		{			
			ref++;
			if (length == (ref - str2))
			{
				return str1 + 1 - length;
			}
		}
		else
		{
			ref = str2;
		}
		str1++;
	}
	return NULL;
}

char* strcpy(char *destination, const char *source)
{
	size_t i = 0;
	
	while(source[i] != '\0')
	{	
		destination[i] = source[i];
		i++;
	}
	
	return destination;
}

char* strncpy(char *destination, const char *source, size_t num)
{
	for(size_t i = 0; i < num; i++)
	{
		destination[i] = source[i];
	}

	destination[num] = '\0';

	return destination;
}

char* strupr(char *str)
{
	size_t i = 0;
	
	while(str[i] != 0)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] += 'A' - 'a'; 
		}
		i++;
	}
	return str;
}

// Returns 0 if the two strings are equal
int strcmp(const char* str1, const char* str2)
{
	size_t length;
	size_t str1Length = strlen(str1);
	size_t str2Length = strlen(str2);

	if (str1Length >= str2Length)
	{
		length = str1Length;
	}
	else
	{
		length = str2Length;
	}

	for(size_t i = 0; i < length; i++)
	{
		int check = str1[i] - str2[i];

		if (check > 0 || check < 0)
		{
			return check;
		}
	}

	return 0;
}

void* memset(void* str, int c, size_t n)
{
	char* next = str;
	char value = (char)c;

	for(size_t counter = 0; counter < n; next++, counter++)
	{
		*next = value;
	}

	return str;
}

void *memcpy(void* dest, const void* src, size_t n)
{
	const char* nextSrc = src;
	char* nextDest = dest;

	for(size_t i = 0; i < n; i++, nextSrc++, nextDest++)
	{
		nextDest[0] = nextSrc[0];
	}
}







