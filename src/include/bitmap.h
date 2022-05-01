#ifndef __BITMAP_H
#define __BITMAP_H

#define BITMAPFILEHEADER_SIZE       14
#define BITMAPINFOHEADER_SIZE       40  // Windows NT, 3.1x or later
#define BITMAPINFOHEADERV2_SIZE     52  // Undocumented
#define BITMAPINFOHEADERV3_SIZE     56  // Adobe Photoshop, adds the alpha channel bitmask
#define BITMAPINFOHEADERV4_SIZE    108  // Windows NT 4.0, 95 or later, adds color space type and gamma correction
#define BITMAPINFOHEADERV5_SIZE    124  // Windows NT 5.0, 98 or later, adds ICC color profiles

#define BITMAP_SIGNATURE        0x4d42

typedef UINT32 PBITMAP;

typedef struct _BITMAPFILEHEADER
{
  UINT16  Signature;                  // The file type, must be "BM", 0x424d
  UINT32  Size;		                  // The size, in bytes, of the bitmap file.
  UINT32  Reserved;                   // Reserved; must be zero.
  UINT32  BitsOffset;                 // The offset, in bytes, from the beginning of the BITMAPFILEHEADER structure to the bitmap bits.

} __attribute__((packed)) BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER
{
  UINT32  HeaderSize;                 // The header size. Size = 40 if BITMAPINFOHEADER, different if other header version present
  INT32   Width;
  INT32   Height;
  UINT16  NumberOfPlanes;
  UINT16  NumberOfBitsPerPixel;
  UINT32  Compression;
  UINT32  ImageSize;                  // The size of the image (without headers) in bytes
  INT32   XPixelsPerMeter;
  INT32   YPixelsPerMeter;
  UINT32  NumberOfUsedColors;
  UINT32  NumberOfImportantColors;

} __attribute__((packed)) BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct _BITMAPINFOHEADERV4
{
  UINT32  HeaderSize;                 // The header size. Size = 40 if BITMAPINFOHEADER, different if other header version present
  INT32   Width;
  INT32   Height;
  UINT16  NumberOfPlanes;
  UINT16  NumberOfBitsPerPixel;
  UINT32  Compression;
  UINT32  ImageSize;                  // The image size in bytes
  INT32   XPixelsPerMeter;
  INT32   YPixelsPerMeter;
  UINT32  NumberOfUsedColors;
  UINT32  NumberOfImportantColors;
  UINT32  RedMask;
  UINT32  GreenMask;
  UINT32  BlueMask;
  UINT32  AlphaMask;
  UINT32  CsType;
  UINT32  Endpoints[9];               // see http://msdn2.microsoft.com/en-us/library/ms536569.aspx
  UINT32  RedGamma;
  UINT32  GreenGamma;
  UINT32  BlueGamma;

} __attribute__((packed)) BITMAPINFOHEADERV4, *PBITMAPINFOHEADERV4;

typedef struct _RGBQUAD
{
  BYTE Blue;
  BYTE Green;
  BYTE Red;
  BYTE Reserved;
} __attribute__((packed)) RGBQUAD, *PRGBQUAD;

PVOID pLogoBitmap;
PVOID pBarBitmap;

VOID LogoBitmapInit();

#endif /* __BITMAP_H */