/*
  - Date: 2018-01-22
  - Author: Joel
  - Purpose: x86 Loader definitions
*/


/* Addresses and Offsets */
#define X86_BOOT_START_OFFSET			            			0x7c00

#define X86_STACK16						                			0x9c00
#define X86_STACK32						                			0xa000
//#define X86_STACK16						                			0xC000    // EXPERIMENTAL - Needs to be a 16bit value
//#define X86_STACK32						                			0xE000    // EXPERIMENTAL
//#define X86_STACK16						                			0x4000    // EXPERIMENTAL - Needs to be a 16bit value
//#define X86_STACK32						                			0x6000    // EXPERIMENTAL
//#define X86_STACK32						                			0x800000    // EXPERIMENTAL 8MB

#define X86_LOADER_BASE				              				0xfa00 
#define X86_LOADER_MZ_BASE					            		0xfe00
#define X86_LOADER_PE_BASE						            	0x10000  

// #define X86_REAL_MODE_BUFFER_ADDRESS			        	0x30000 /* Assume that the Loader will grow to 128k in size */
// #define X86_REAL_MODE_BUFFER_SIZE				          	0x10000 /* 64KB */   /* Used to be 4096 One page */
// #define X86_FREELIST_BASE                           0x40000
// #define X86_FREELIST_SIZE                           0x10000 /* 64KB */
// #define X86_USEDLIST_BASE                           0x50000
// #define X86_USEDLIST_SIZE                           0x10000 /* 64KB */

// Bumped up addresses (EXPERIMENTAL)
#define X86_REAL_MODE_BUFFER_ADDRESS			        	0x50000 /* Assume that the Loader will grow to 128k in size */
#define X86_REAL_MODE_BUFFER_SIZE			  	          0x10000 /* 64KB */   /* Used to be 4096 One page */
#define X86_FREELIST_BASE                          0x60000
#define X86_FREELIST_SIZE                          0x10000 /* 64KB */
#define X86_USEDLIST_BASE                          0x70000
#define X86_USEDLIST_SIZE                          0x10000 /* 64KB */

/* Loader Memory Footprint */
#define X86_LOADER_START                            X86_BOOT_START_OFFSET
#define X86_LOADER_END                              X86_USEDLIST_BASE + X86_USEDLIST_SIZE
//#define X86_LOADER_END                              0x100000  // EXPERIMENTAL (above 1MB)

/* Signatures and Flags */
#define X86_LOADER_SIGNATURE				            		0x4c4b	/* "KL" - Kore Loader */
#define X86_LOADER_MZ_SIGNATURE			          			0x5a4d	/* "MZ" - MS-DOS Header */
#define X86_LOADER_PE_SIGNATURE				          		0x4550	/* "PE" - Portable Executable */
#define X86_LOADER_OPTIONAL_HEADER_SIGNATURE	    	0x010b	/* PE Optional Header */
#define X86_LOADER_MACHINE_FLAG_I386		        		0x014c

/* Bios */
#define X86_BIOS_VIDEO_FUNCTION				          		0x10
#define X86_BIOS_VIDEO_FUNCION_TELETYPE_OUTPUT	  	0x0e
#define X86_BIOS_DISK_FUNCTION				          		0x13
#define X86_BIOS_DISK_FUNCTION_RESET_DISK_SYSTEM  	0x00
#define X86_BIOS_DISK_FUNCTION_GET_DRIVE_PARAMETERS	0x08
#define X86_BIOS_DISK_FUNCTION_READ_SECTOR	    		0x02
#define X86_BIOS_MAX_DRIVE_SECTOR_COUNT			      	0x3f

/* Video Text */
#define X86_TEXT_COLOR_ATTRIBUTE_GRAY_ON_BLACK	    0x07

/* Partition Info */
#define X86_ACTIVE_PARTITION_FLAG				          	0x80
#define X86_INACTIVE_PARTITION_FLAG			        		0x00
#define X86_PARTITION_TYPE_FAT32LARGE		        		0x0c
#define X86_PARTITION_TYPE_FAT32SMALL			        	0x0b

/* Boot Sector */
#define X86_BOOT_SECTOR_SIGNATURE_OFFSET		      	0x7dfe
#define X86_BOOT_SECTOR_SIGNATURE			          		0xaa55

/* Selectors */
#define X86_NULL_DESCRIPTOR						            	0x00
#define X86_CODE32_SELECTOR		 			            		0x08
#define X86_DATA32_SELECTOR		 				            	0x10
#define X86_CODE16_SELECTOR	 				            		0x18
#define X86_DATA16_SELECTOR		 				            	0x20

/* Protected Mode */
#define X86_CR0_PE_SET							              	0x00000001
#define X86_CR0_PE_CLEAR						              	0xfffffffe

/* x86 Info Block */
#define X86_INFO_BLOCK_START				            		0x0800
#define X86_BOOT_DRIVE_ID					              		X86_INFO_BLOCK_START
#define X86_BOOT_PARTITION_ID				            		X86_INFO_BLOCK_START + 4
#define X86_REAL_MODE_HANDLER_ADDRESS		        		X86_INFO_BLOCK_START + 8
#define X86_PROTECTED_MODE_LOADER_ENTRY_POINT	    	X86_INFO_BLOCK_START + 12
#define X86_PROTECTED_MODE_CONTINUATION_ADDRESS	  	X86_INFO_BLOCK_START + 16
#define X86_STACK16_STORED					            		X86_INFO_BLOCK_START + 20
#define X86_STACK32_STORED					            		X86_INFO_BLOCK_START + 24
#define X86_FUNCTION_NUMBER						            	X86_INFO_BLOCK_START + 28
#define X86_INTERRUPT_NUMBER_ADDRESS		        		X86_INFO_BLOCK_START + 32

/* Real Mode Function registers */
#define X86_REGISTER_EAX					              		X86_INFO_BLOCK_START + 36
#define X86_REGISTER_EBX						              	X86_INFO_BLOCK_START + 40
#define X86_REGISTER_ECX					              		X86_INFO_BLOCK_START + 44
#define X86_REGISTER_EDX					              		X86_INFO_BLOCK_START + 48
#define X86_REGISTER_ESI					              		X86_INFO_BLOCK_START + 52
#define X86_REGISTER_EDI					              		X86_INFO_BLOCK_START + 56
#define X86_REGISTER_EBP					              		X86_INFO_BLOCK_START + 60
#define X86_REGISTER_DS						              		X86_INFO_BLOCK_START + 64
#define X86_REGISTER_ES						              		X86_INFO_BLOCK_START + 68
#define X86_REGISTER_FS						              		X86_INFO_BLOCK_START + 72
#define X86_REGISTER_GS						              		X86_INFO_BLOCK_START + 76
#define X86_REGISTER_EFLAGS				            			X86_INFO_BLOCK_START + 80
