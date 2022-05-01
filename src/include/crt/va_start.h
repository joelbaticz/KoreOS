/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
//Macro to set arg_ptr to beginning of list of optional arguments (UNIX version only)

#define	va_start(AP, LASTARG)	\
	(AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

	