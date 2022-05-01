//C language runtime Standard Library implementation
//#include <string.h>
//#include <ktypes.h>
//#include <kldr.h>

/* Not standard C functions */


/* FIX ME! If Base is 16 and Value < 0 the result is not good */
char* itoa(int value, char* str, int base)
{
	
	char tempBuffer[32] = {0};
	char numberBuffer[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', };
	int length = 0;
	int pos = 0;
	int tempPos = 0;
	
	if (base > 16)
	{
		//We don't handle Number Bases bigger than hexadecimal
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	if (base == 10 && value == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	//If base = decimal and the value is negative
	if (base == 10 && value < 0)
	{
		str[pos] = '-';
		pos++;
		value *= -1;
	}
	//Add prefix if Hexadecimal
	if (base == 16)
	{
		str[pos] = '0';
		pos++;
		str[pos] = 'x';
		pos++;
		//if (value < 0) value=0 - value;
	}
	
	while (value != 0)
	{
		int index = value % base;
		value = value / base;
		tempBuffer[tempPos] = numberBuffer[index];
		tempPos++;
		length++;
	}	
	
	//Reverse order
	tempPos--;
	pos++;
	pos--;
	
	if (base == 16 && length<8)
	{
		//Fill with zeroes
		for (int i = 0; i < 8 - length; i++)
		{
			str[pos] = '0';
			pos++;
		}
	}

	for (int i = 0; i < length; i++)
	{
		str[pos] = tempBuffer[tempPos];
		pos++;
		tempPos--;
	}

	str[pos] = '\0';
	
	return str;
}

char* uitoa(unsigned int value, char* str, unsigned int base)
{
	
	char tempBuffer[32] = {0};
	char numberBuffer[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', };
	int length = 0;
	int pos = 0;
	int tempPos = 0;
	
	if (base > 16)
	{
		//We don't handle Number Bases bigger than hexadecimal
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	if (value == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	// //If base = decimal and the value is negative
	// if (base == 10 && value < 0)
	// {
		// str[pos] = '-';
		// pos++;
		// value *= -1;
	// }
	
	// //Add prefix if Hexadecimal
	if (base == 16)
	{
		str[pos] = '0';
		pos++;
		str[pos] = 'x';
		pos++;
		//if (value < 0) value=0 - value;
	}
	
	while (value != 0)
	{
		int index = value % base;
		value = value / base;
		tempBuffer[tempPos] = numberBuffer[index];
		tempPos++;
		length++;
	}	
	
	//Reverse order
	tempPos--;
	pos++;
	pos--;
	
	// if (base == 16 && length<8)
	// {
		// //Fill with zeroes
		// for (int i = 0; i < 8 - length; i++)
		// {
			// str[pos] = '0';
			// pos++;
		// }
	// }

	for (int i = 0; i < length; i++)
	{
		str[pos] = tempBuffer[tempPos];

		if (base == 10 && tempPos != 0 && tempPos % 3 == 0)
		{
			pos++;
			str[pos] = ',';
		}

		pos++;
		tempPos--;

	}

	str[pos] = '\0';
	
	return str;
}
