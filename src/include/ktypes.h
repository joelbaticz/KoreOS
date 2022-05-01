/*
  - Date: 2018-01-01
  - Author: Joel
  - Purpose: Type definitions
*/

#ifndef __KTYPES_H
#define __KTYPES_H

#include <crt/_null.h>
#include <crt/stdint.h>
#include <crt/ctype.h>
#include <crt/size_t.h>
#include <crt/va_list.h>
#include <crt/va_start.h>
#include <crt/va_size.h>
#include <crt/va_arg.h>
#include <crt/va_end.h>

//K STANDARD TYPE DEFINITIONS

//CHAR
typedef char			        	CHAR;

//UNICODE CHAR
typedef short			        	WCHAR;

//INTxx
typedef signed char		    	INT8;
typedef signed short    		INT16;
typedef signed int		    	INT32;
typedef signed long long  	INT64;  

//UINTxx
typedef unsigned char	    	UINT8;
typedef unsigned short  		UINT16;
typedef unsigned int	    	UINT32;
typedef unsigned long long 	UINT64;

//X86 TYPES (Only be used in x86 specific contexts)
typedef UINT8		  		      BYTE;
typedef UINT16				      WORD;
typedef UINT32				      DWORD;
typedef WORD		  		      X86_SEGMENT;
typedef DWORD			        	X86_REGISTER;

//BOOL
typedef _Bool		  		      BOOL;
#define TRUE	  			      1
#define FALSE		  		      0

//SIZE
typedef UINT32				      SIZE;

//VOID
typedef void		  		      VOID;

/* ----- POINTERS ----- */

//Pointer to a CHAR
typedef char*		  		      PCHAR;

//Pointer to a WCHAR
typedef short*			      	PWCHAR;

//Pointer to a signed INTxx
typedef INT8*			        	PINT8;
typedef INT16*			      	PINT16;
typedef INT32*			      	PINT32;
typedef INT64*			      	PINT64;

//Pointer to an unsigned INTxx
typedef UINT8* 			      	PUINT8;
typedef UINT16* 		      	PUINT16;
typedef UINT32* 		      	PUINT32;
typedef UINT64* 		      	PUINT64;

//Pointer to a VOID
typedef void*			        	PVOID;

//Pointer to a X86 TYPES
typedef BYTE* 			      	PBYTE;
typedef WORD* 			      	PWORD;
typedef DWORD*			      	PDWORD;

//Pointer to BOOL
typedef BOOL* 			      	PBOOL;

//Pointer to ASCII STRING
typedef PCHAR			        	PCSTR;

//Pointer to UNICODE STRING (WIDE STRING)
typedef PWCHAR			      	PWSTR;

//Kernel objects
typedef UINT32              HANDLE;

#endif /* __KTYPES_H */