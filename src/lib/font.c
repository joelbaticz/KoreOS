#include <kldr.h>

static CHAR defaultFont[4032] = {
    0x00, 0x02, 0xC0, 0x0F, 0x00, 0x00, 0x28, 0x63, 0x29, 0x20, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69,
    0x67, 0x68, 0x74, 0x20, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F,
    0x72, 0x70, 0x2E, 0x20, 0x31, 0x39, 0x38, 0x37, 0x2E, 0x20, 0x41, 0x6C, 0x6C, 0x20, 0x72, 0x69,
    0x67, 0x68, 0x74, 0x73, 0x20, 0x72, 0x65, 0x73, 0x65, 0x72, 0x76, 0x65, 0x64, 0x2E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x60, 0x00, 0x60, 0x00, 0x0B, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x21, 0x07, 0x00, 0x0B, 0x00, 0x20,
    0xFF, 0x60, 0x00, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x0D, 0x04, 0x05, 0x00,
    0x1A, 0x04, 0x07, 0x00, 0x27, 0x04, 0x06, 0x00, 0x34, 0x04, 0x08, 0x00, 0x41, 0x04, 0x06, 0x00,
    0x4E, 0x04, 0x02, 0x00, 0x5B, 0x04, 0x03, 0x00, 0x68, 0x04, 0x03, 0x00, 0x75, 0x04, 0x04, 0x00,
    0x82, 0x04, 0x06, 0x00, 0x8F, 0x04, 0x03, 0x00, 0x9C, 0x04, 0x03, 0x00, 0xA9, 0x04, 0x03, 0x00,
    0xB6, 0x04, 0x05, 0x00, 0xC3, 0x04, 0x06, 0x00, 0xD0, 0x04, 0x06, 0x00, 0xDD, 0x04, 0x06, 0x00,
    0xEA, 0x04, 0x06, 0x00, 0xF7, 0x04, 0x06, 0x00, 0x04, 0x05, 0x06, 0x00, 0x11, 0x05, 0x06, 0x00,
    0x1E, 0x05, 0x06, 0x00, 0x2B, 0x05, 0x06, 0x00, 0x38, 0x05, 0x06, 0x00, 0x45, 0x05, 0x03, 0x00,
    0x52, 0x05, 0x03, 0x00, 0x5F, 0x05, 0x06, 0x00, 0x6C, 0x05, 0x06, 0x00, 0x79, 0x05, 0x06, 0x00,
    0x86, 0x05, 0x06, 0x00, 0x93, 0x05, 0x0B, 0x00, 0xA0, 0x05, 0x07, 0x00, 0xBA, 0x05, 0x07, 0x00,
    0xC7, 0x05, 0x07, 0x00, 0xD4, 0x05, 0x08, 0x00, 0xE1, 0x05, 0x07, 0x00, 0xEE, 0x05, 0x06, 0x00,
    0xFB, 0x05, 0x08, 0x00, 0x08, 0x06, 0x08, 0x00, 0x15, 0x06, 0x03, 0x00, 0x22, 0x06, 0x05, 0x00,
    0x2F, 0x06, 0x07, 0x00, 0x3C, 0x06, 0x06, 0x00, 0x49, 0x06, 0x09, 0x00, 0x56, 0x06, 0x08, 0x00,
    0x70, 0x06, 0x08, 0x00, 0x7D, 0x06, 0x07, 0x00, 0x8A, 0x06, 0x08, 0x00, 0x97, 0x06, 0x08, 0x00,
    0xA4, 0x06, 0x07, 0x00, 0xB1, 0x06, 0x07, 0x00, 0xBE, 0x06, 0x08, 0x00, 0xCB, 0x06, 0x07, 0x00,
    0xD8, 0x06, 0x0B, 0x00, 0xE5, 0x06, 0x07, 0x00, 0xFF, 0x06, 0x07, 0x00, 0x0C, 0x07, 0x07, 0x00,
    0x19, 0x07, 0x03, 0x00, 0x26, 0x07, 0x05, 0x00, 0x33, 0x07, 0x03, 0x00, 0x40, 0x07, 0x06, 0x00,
    0x4D, 0x07, 0x06, 0x00, 0x5A, 0x07, 0x03, 0x00, 0x67, 0x07, 0x06, 0x00, 0x74, 0x07, 0x06, 0x00,
    0x81, 0x07, 0x06, 0x00, 0x8E, 0x07, 0x06, 0x00, 0x9B, 0x07, 0x06, 0x00, 0xA8, 0x07, 0x03, 0x00,
    0xB5, 0x07, 0x06, 0x00, 0xC2, 0x07, 0x06, 0x00, 0xCF, 0x07, 0x02, 0x00, 0xDC, 0x07, 0x02, 0x00,
    0xE9, 0x07, 0x06, 0x00, 0xF6, 0x07, 0x02, 0x00, 0x03, 0x08, 0x08, 0x00, 0x10, 0x08, 0x06, 0x00,
    0x1D, 0x08, 0x06, 0x00, 0x2A, 0x08, 0x06, 0x00, 0x37, 0x08, 0x06, 0x00, 0x44, 0x08, 0x03, 0x00,
    0x51, 0x08, 0x05, 0x00, 0x5E, 0x08, 0x03, 0x00, 0x6B, 0x08, 0x06, 0x00, 0x78, 0x08, 0x06, 0x00,
    0x85, 0x08, 0x08, 0x00, 0x92, 0x08, 0x05, 0x00, 0x9F, 0x08, 0x05, 0x00, 0xAC, 0x08, 0x05, 0x00,
    0xB9, 0x08, 0x04, 0x00, 0xC6, 0x08, 0x02, 0x00, 0xD3, 0x08, 0x04, 0x00, 0xE0, 0x08, 0x07, 0x00,
    0xED, 0x08, 0x03, 0x00, 0xFA, 0x08, 0x03, 0x00, 0x07, 0x09, 0x03, 0x00, 0x14, 0x09, 0x03, 0x00,
    0x21, 0x09, 0x03, 0x00, 0x2E, 0x09, 0x03, 0x00, 0x3B, 0x09, 0x03, 0x00, 0x48, 0x09, 0x03, 0x00,
    0x55, 0x09, 0x03, 0x00, 0x62, 0x09, 0x03, 0x00, 0x6F, 0x09, 0x03, 0x00, 0x7C, 0x09, 0x03, 0x00,
    0x89, 0x09, 0x03, 0x00, 0x96, 0x09, 0x03, 0x00, 0xA3, 0x09, 0x03, 0x00, 0xB0, 0x09, 0x03, 0x00,
    0xBD, 0x09, 0x03, 0x00, 0xCA, 0x09, 0x03, 0x00, 0xD7, 0x09, 0x03, 0x00, 0xE4, 0x09, 0x03, 0x00,
    0xF1, 0x09, 0x03, 0x00, 0xFE, 0x09, 0x03, 0x00, 0x0B, 0x0A, 0x03, 0x00, 0x18, 0x0A, 0x03, 0x00,
    0x25, 0x0A, 0x03, 0x00, 0x32, 0x0A, 0x03, 0x00, 0x3F, 0x0A, 0x03, 0x00, 0x4C, 0x0A, 0x03, 0x00,
    0x59, 0x0A, 0x03, 0x00, 0x66, 0x0A, 0x03, 0x00, 0x73, 0x0A, 0x03, 0x00, 0x80, 0x0A, 0x03, 0x00,
    0x8D, 0x0A, 0x03, 0x00, 0x9A, 0x0A, 0x03, 0x00, 0xA7, 0x0A, 0x03, 0x00, 0xB4, 0x0A, 0x06, 0x00,
    0xC1, 0x0A, 0x06, 0x00, 0xCE, 0x0A, 0x06, 0x00, 0xDB, 0x0A, 0x06, 0x00, 0xE8, 0x0A, 0x02, 0x00,
    0xF5, 0x0A, 0x06, 0x00, 0x02, 0x0B, 0x03, 0x00, 0x0F, 0x0B, 0x09, 0x00, 0x1C, 0x0B, 0x04, 0x00,
    0x36, 0x0B, 0x06, 0x00, 0x43, 0x0B, 0x06, 0x00, 0x50, 0x0B, 0x03, 0x00, 0x5D, 0x0B, 0x08, 0x00,
    0x6A, 0x0B, 0x06, 0x00, 0x77, 0x0B, 0x04, 0x00, 0x84, 0x0B, 0x06, 0x00, 0x91, 0x0B, 0x03, 0x00,
    0x9E, 0x0B, 0x03, 0x00, 0xAB, 0x0B, 0x03, 0x00, 0xB8, 0x0B, 0x06, 0x00, 0xC5, 0x0B, 0x06, 0x00,
    0xD2, 0x0B, 0x03, 0x00, 0xDF, 0x0B, 0x03, 0x00, 0xEC, 0x0B, 0x03, 0x00, 0xF9, 0x0B, 0x04, 0x00,
    0x06, 0x0C, 0x06, 0x00, 0x13, 0x0C, 0x08, 0x00, 0x20, 0x0C, 0x08, 0x00, 0x2D, 0x0C, 0x08, 0x00,
    0x3A, 0x0C, 0x06, 0x00, 0x47, 0x0C, 0x07, 0x00, 0x54, 0x0C, 0x07, 0x00, 0x61, 0x0C, 0x07, 0x00,
    0x6E, 0x0C, 0x07, 0x00, 0x7B, 0x0C, 0x07, 0x00, 0x88, 0x0C, 0x07, 0x00, 0x95, 0x0C, 0x0A, 0x00,
    0xA2, 0x0C, 0x07, 0x00, 0xBC, 0x0C, 0x07, 0x00, 0xC9, 0x0C, 0x07, 0x00, 0xD6, 0x0C, 0x07, 0x00,
    0xE3, 0x0C, 0x07, 0x00, 0xF0, 0x0C, 0x03, 0x00, 0xFD, 0x0C, 0x03, 0x00, 0x0A, 0x0D, 0x03, 0x00,
    0x17, 0x0D, 0x03, 0x00, 0x24, 0x0D, 0x08, 0x00, 0x31, 0x0D, 0x08, 0x00, 0x3E, 0x0D, 0x08, 0x00,
    0x4B, 0x0D, 0x08, 0x00, 0x58, 0x0D, 0x08, 0x00, 0x65, 0x0D, 0x08, 0x00, 0x72, 0x0D, 0x08, 0x00,
    0x7F, 0x0D, 0x06, 0x00, 0x8C, 0x0D, 0x08, 0x00, 0x99, 0x0D, 0x08, 0x00, 0xA6, 0x0D, 0x08, 0x00,
    0xB3, 0x0D, 0x08, 0x00, 0xC0, 0x0D, 0x08, 0x00, 0xCD, 0x0D, 0x07, 0x00, 0xDA, 0x0D, 0x07, 0x00,
    0xE7, 0x0D, 0x06, 0x00, 0xF4, 0x0D, 0x06, 0x00, 0x01, 0x0E, 0x06, 0x00, 0x0E, 0x0E, 0x06, 0x00,
    0x1B, 0x0E, 0x06, 0x00, 0x28, 0x0E, 0x06, 0x00, 0x35, 0x0E, 0x06, 0x00, 0x42, 0x0E, 0x0A, 0x00,
    0x4F, 0x0E, 0x06, 0x00, 0x69, 0x0E, 0x06, 0x00, 0x76, 0x0E, 0x06, 0x00, 0x83, 0x0E, 0x06, 0x00,
    0x90, 0x0E, 0x06, 0x00, 0x9D, 0x0E, 0x02, 0x00, 0xAA, 0x0E, 0x04, 0x00, 0xB7, 0x0E, 0x04, 0x00,
    0xC4, 0x0E, 0x04, 0x00, 0xD1, 0x0E, 0x06, 0x00, 0xDE, 0x0E, 0x06, 0x00, 0xEB, 0x0E, 0x06, 0x00,
    0xF8, 0x0E, 0x06, 0x00, 0x05, 0x0F, 0x06, 0x00, 0x12, 0x0F, 0x06, 0x00, 0x1F, 0x0F, 0x06, 0x00,
    0x2C, 0x0F, 0x06, 0x00, 0x39, 0x0F, 0x06, 0x00, 0x46, 0x0F, 0x06, 0x00, 0x53, 0x0F, 0x06, 0x00,
    0x60, 0x0F, 0x06, 0x00, 0x6D, 0x0F, 0x06, 0x00, 0x7A, 0x0F, 0x05, 0x00, 0x87, 0x0F, 0x06, 0x00,
    0x94, 0x0F, 0x05, 0x00, 0xA1, 0x0F, 0x08, 0x00, 0xAE, 0x0F, 0x48, 0x65, 0x6C, 0x76, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x48, 0x48, 0x48, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x7E,
    0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x54, 0x50, 0x30, 0x18, 0x14, 0x54, 0x38, 0x10,
    0x00, 0x00, 0x00, 0x30, 0x49, 0x32, 0x04, 0x08, 0x10, 0x26, 0x49, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x50, 0x50, 0x20, 0x20, 0x54, 0x48, 0x48, 0x34, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x20, 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x40, 0x00, 0x00, 0x00, 0x50, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x18, 0x04, 0x04,
    0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x18, 0x28, 0x28, 0x48, 0x7C, 0x08, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x40, 0x40, 0x78, 0x44, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00,
    0x38, 0x44, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x04, 0x08,
    0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x44,
    0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x44, 0x38,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C,
    0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x10, 0x20,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x0F, 0x30, 0x20, 0x47, 0x49, 0x49, 0x46, 0x20, 0x30, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0x40, 0x20, 0x20, 0x20, 0xE0, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x10, 0x10, 0x28, 0x28,
    0x44, 0x44, 0x7C, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x78, 0x44, 0x44,
    0x44, 0x78, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00, 0x00,
    0x7C, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x40, 0x40,
    0x40, 0x78, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x4E, 0x42,
    0x42, 0x46, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x90, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x44, 0x48,
    0x50, 0x60, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x63, 0x63, 0x55, 0x55, 0x49, 0x49,
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x42, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x46, 0x46, 0x42, 0x00, 0x00, 0x00, 0x00, 0x3C,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42,
    0x7C, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x4A,
    0x46, 0x3C, 0x02, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x40, 0x38, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00,
    0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x82, 0x82, 0x44, 0x44, 0x44, 0x28,
    0x28, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x44, 0x44, 0x44, 0x2A, 0x2A, 0x11, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x40, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x82, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x82, 0x82,
    0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x60, 0x00, 0x00, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x00, 0x10, 0x28,
    0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04,
    0x3C, 0x44, 0x44, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7C,
    0x40, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x60, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x78, 0x00,
    0x00, 0x40, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40, 0x40, 0x48, 0x50, 0x60, 0x50, 0x48,
    0x44, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x49, 0x49, 0x49, 0x49, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44,
    0x44, 0x78, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x44, 0x44, 0x3C, 0x04,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x48, 0x20, 0x10, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40,
    0x60, 0x40, 0x40, 0x40, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44,
    0x44, 0x4C, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x49, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x48, 0x48, 0x30, 0x30, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x48, 0x48, 0x48, 0x48, 0x30, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x08,
    0x10, 0x20, 0x40, 0x78, 0x00, 0x00, 0x00, 0x10, 0x20, 0x20, 0x20, 0x20, 0x40, 0x20, 0x20, 0x20,
    0x20, 0x10, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x00, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x08, 0x38, 0x44, 0x40, 0x40, 0x44, 0x38, 0x20, 0x00, 0x00, 0x00,
    0x30, 0x48, 0x40, 0x20, 0x70, 0x20, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x44, 0x38, 0x44,
    0x44, 0x44, 0x38, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x7C, 0x10,
    0x7C, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x00, 0x00, 0x38, 0x44, 0x20, 0x50, 0x48, 0x24, 0x14, 0x08, 0x44, 0x38, 0x00, 0x00,
    0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x21,
    0x4C, 0x50, 0x4C, 0x21, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x50, 0x70, 0x00, 0x70, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x28, 0x50, 0x28, 0x14, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x5D, 0x59,
    0x55, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0x20, 0x40, 0x80, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x40,
    0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x48, 0x48, 0x48, 0x48, 0x74,
    0x40, 0x40, 0x00, 0x00, 0x3C, 0x7C, 0x7C, 0x7C, 0x3C, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x00, 0x00, 0x20, 0x60, 0x20, 0x20, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x50, 0x50, 0x70, 0x00, 0x70, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x28, 0x14, 0x28, 0x50, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x61, 0x22, 0x24, 0x29, 0x13, 0x25, 0x47, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20,
    0x61, 0x22, 0x24, 0x2E, 0x11, 0x22, 0x44, 0x07, 0x00, 0x00, 0x00, 0x00, 0x60, 0x11, 0x22, 0x14,
    0x69, 0x13, 0x25, 0x47, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x10, 0x20,
    0x40, 0x40, 0x44, 0x38, 0x20, 0x10, 0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x7C, 0x82, 0x82, 0x00,
    0x00, 0x08, 0x10, 0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x7C, 0x82, 0x82, 0x00, 0x00, 0x10, 0x28,
    0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x7C, 0x82, 0x82, 0x00, 0x00, 0x34, 0x58, 0x10, 0x10, 0x28,
    0x28, 0x44, 0x44, 0x7C, 0x82, 0x82, 0x00, 0x00, 0x44, 0x00, 0x10, 0x10, 0x28, 0x28, 0x44, 0x44,
    0x7C, 0x82, 0x82, 0x00, 0x00, 0x10, 0x28, 0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x7C, 0x82, 0x82,
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x28, 0x28, 0x4F, 0x78, 0x48, 0x88, 0x8F, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3C, 0x08, 0x18, 0x20, 0x10, 0x7C, 0x40, 0x40, 0x40, 0x78,
    0x40, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x08, 0x10, 0x7C, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40,
    0x7C, 0x00, 0x00, 0x10, 0x28, 0x7C, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x7C, 0x00, 0x00,
    0x44, 0x00, 0x7C, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x80, 0x40, 0x00,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x20, 0x40, 0x00, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x00, 0x00, 0xA0, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0xF2, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x32, 0x4C,
    0x42, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x46, 0x46, 0x42, 0x00, 0x00, 0x10, 0x08, 0x3C, 0x42, 0x42,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x08, 0x10, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x42, 0x42, 0x3C, 0x00, 0x00, 0x18, 0x24, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C,
    0x00, 0x00, 0x32, 0x4C, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x24,
    0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x46, 0x4A, 0x4A, 0x52,
    0x52, 0x62, 0x62, 0x7C, 0x00, 0x00, 0x10, 0x08, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x3C, 0x00, 0x00, 0x08, 0x10, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x24, 0x00, 0x42,
    0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x08, 0x10, 0x82, 0x82, 0x44, 0x28,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x7C,
    0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x48, 0x48, 0x58, 0x44, 0x44, 0x44, 0x58, 0x00,
    0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x10, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x00,
    0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x34, 0x58, 0x00, 0x38, 0x04, 0x3C,
    0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C,
    0x00, 0x00, 0x00, 0x10, 0x28, 0x10, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3B, 0x04, 0x3F, 0x44, 0x44, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x40, 0xC0, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44,
    0x40, 0x40, 0x44, 0x38, 0x10, 0x30, 0x00, 0x00, 0x20, 0x10, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x44,
    0x38, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x44, 0x38, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x28, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x44,
    0x00, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x28, 0x10, 0x28, 0x08, 0x3C, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x34, 0x58, 0x00,
    0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x38, 0x44, 0x44,
    0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x38,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00,
    0x00, 0x34, 0x58, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00,
    0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x7C, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x4C, 0x54, 0x54, 0x64,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x44, 0x44, 0x44, 0x44, 0x4C, 0x34, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x10, 0x00, 0x44, 0x44, 0x44, 0x44, 0x4C, 0x34, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x28, 0x00, 0x44, 0x44, 0x44, 0x44, 0x4C, 0x34, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x44,
    0x44, 0x44, 0x44, 0x4C, 0x34, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00, 0x48, 0x48, 0x48, 0x48,
    0x30, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x40, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x44, 0x78, 0x40,
    0x40, 0x00, 0x00, 0x48, 0x00, 0x00, 0x48, 0x48, 0x48, 0x48, 0x30, 0x20, 0x20, 0xC0, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

VOID DefaultFontInit()
{
    UiDrawText("DefaultFontInit()");
    pDefaultFont = &defaultFont[0];
}