/*******************************************************************************
 * Size: 6 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_FONT_HARMONYOS_SANS_SC_REGULAR_6
#define LV_FONT_HARMONYOS_SANS_SC_REGULAR_6 1
#endif

#if LV_FONT_HARMONYOS_SANS_SC_REGULAR_6

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x80, 0x70, 0x60, 0x50,

    /* U+0022 "\"" */
    0x0, 0x66, 0x11,

    /* U+0023 "#" */
    0x7, 0x25, 0x3b, 0xa6, 0x89, 0xb2, 0x61, 0x70,

    /* U+0024 "$" */
    0x3, 0x0, 0x4b, 0x70, 0x89, 0x10, 0xa, 0x90,
    0x7a, 0x81, 0x8, 0x10,

    /* U+0025 "%" */
    0x89, 0x17, 0x5, 0x78, 0x0, 0x4, 0x67, 0x20,
    0x84, 0x75,

    /* U+0026 "&" */
    0x7, 0x60, 0x1, 0x79, 0x0, 0xc, 0x63, 0x9,
    0x19, 0x80, 0x56, 0x79, 0x0,

    /* U+0027 "'" */
    0x6, 0x10,

    /* U+0028 "(" */
    0x7, 0x62, 0x80, 0x62, 0x7,

    /* U+0029 ")" */
    0x71, 0x17, 0x8, 0x17, 0x71,

    /* U+002A "*" */
    0x49, 0x14, 0xa1, 0x0, 0x0,

    /* U+002B "+" */
    0x4, 0x0, 0x4b, 0x70, 0x7, 0x0,

    /* U+002C "," */
    0x40, 0x30,

    /* U+002D "-" */
    0x46, 0x30,

    /* U+002E "." */
    0x40,

    /* U+002F "/" */
    0x7, 0x10, 0x80, 0x43, 0x8, 0x0,

    /* U+0030 "0" */
    0x48, 0x80, 0x90, 0x71, 0x90, 0x71, 0x48, 0x80,

    /* U+0031 "1" */
    0x2a, 0x40, 0x44, 0x4, 0x40, 0x44,

    /* U+0032 "2" */
    0x57, 0x80, 0x0, 0x90, 0x7, 0x30, 0x7a, 0x70,

    /* U+0033 "3" */
    0x47, 0x90, 0x5, 0x90, 0x10, 0x80, 0x67, 0x80,

    /* U+0034 "4" */
    0x6, 0x20, 0x36, 0x40, 0x76, 0xb2, 0x0, 0x80,

    /* U+0035 "5" */
    0x48, 0x60, 0x67, 0x50, 0x0, 0x90, 0x47, 0x80,

    /* U+0036 "6" */
    0x6, 0x20, 0x2a, 0x60, 0x90, 0x81, 0x58, 0x90,

    /* U+0037 "7" */
    0x47, 0xb0, 0x1, 0x70, 0x7, 0x10, 0x8, 0x0,

    /* U+0038 "8" */
    0x57, 0x90, 0x4a, 0x90, 0x90, 0x80, 0x58, 0x80,

    /* U+0039 "9" */
    0x58, 0x90, 0x90, 0x80, 0x39, 0x80, 0x8, 0x0,

    /* U+003A ":" */
    0x40, 0x0, 0x40,

    /* U+003B ";" */
    0x40, 0x0, 0x41, 0x30,

    /* U+003C "<" */
    0x0, 0x20, 0x47, 0x50, 0x37, 0x60, 0x0, 0x10,

    /* U+003D "=" */
    0x47, 0x70, 0x46, 0x60,

    /* U+003E ">" */
    0x20, 0x0, 0x27, 0x60, 0x37, 0x60, 0x10, 0x0,

    /* U+003F "?" */
    0x5, 0x72, 0x2, 0x27, 0x0, 0x80, 0x0, 0x40,
    0x0, 0x40,

    /* U+0040 "@" */
    0x3, 0x66, 0x40, 0x35, 0x0, 0x53, 0x70, 0x77,
    0x27, 0x74, 0x36, 0x37, 0x46, 0x75, 0x72, 0x4,
    0x66, 0x10,

    /* U+0041 "A" */
    0x6, 0x60, 0x7, 0x80, 0x58, 0x94, 0x80, 0x8,

    /* U+0042 "B" */
    0x77, 0x83, 0x77, 0xa3, 0x71, 0x8, 0x77, 0x75,

    /* U+0043 "C" */
    0x18, 0x75, 0x90, 0x0, 0x90, 0x0, 0x18, 0x75,

    /* U+0044 "D" */
    0x77, 0x73, 0x71, 0x9, 0x71, 0x9, 0x77, 0x74,

    /* U+0045 "E" */
    0x77, 0x71, 0x77, 0x60, 0x71, 0x0, 0x77, 0x72,

    /* U+0046 "F" */
    0x77, 0x71, 0x77, 0x60, 0x71, 0x0, 0x71, 0x0,

    /* U+0047 "G" */
    0x18, 0x75, 0x90, 0x38, 0x90, 0x8, 0x18, 0x78,

    /* U+0048 "H" */
    0x71, 0x8, 0x77, 0x7b, 0x71, 0x8, 0x71, 0x8,

    /* U+0049 "I" */
    0x71, 0x71, 0x71, 0x71,

    /* U+004A "J" */
    0x4, 0x40, 0x44, 0x4, 0x47, 0x91,

    /* U+004B "K" */
    0x71, 0x45, 0x7, 0x66, 0x0, 0x76, 0x90, 0x7,
    0x12, 0x80,

    /* U+004C "L" */
    0x71, 0x0, 0x71, 0x0, 0x71, 0x0, 0x77, 0x71,

    /* U+004D "M" */
    0x75, 0x2, 0x97, 0x81, 0x88, 0x71, 0xb2, 0x87,
    0x0, 0x8,

    /* U+004E "N" */
    0x75, 0x8, 0x77, 0x28, 0x70, 0x89, 0x70, 0xc,

    /* U+004F "O" */
    0x18, 0x77, 0x8, 0x0, 0x62, 0x80, 0x6, 0x22,
    0x87, 0x70,

    /* U+0050 "P" */
    0x77, 0x81, 0x71, 0x35, 0x77, 0x70, 0x71, 0x0,

    /* U+0051 "Q" */
    0x18, 0x77, 0x8, 0x0, 0x62, 0x80, 0x6, 0x22,
    0x88, 0x80, 0x0, 0x9, 0x20,

    /* U+0052 "R" */
    0x77, 0x91, 0x71, 0x35, 0x77, 0xb0, 0x71, 0x45,

    /* U+0053 "S" */
    0x67, 0x80, 0x66, 0x10, 0x11, 0x82, 0x78, 0x91,

    /* U+0054 "T" */
    0x6b, 0x72, 0x8, 0x0, 0x8, 0x0, 0x8, 0x0,

    /* U+0055 "U" */
    0x80, 0x8, 0x80, 0x8, 0x70, 0x8, 0x28, 0x75,

    /* U+0056 "V" */
    0x90, 0x8, 0x44, 0x34, 0x8, 0x70, 0x6, 0x60,

    /* U+0057 "W" */
    0x90, 0x75, 0x7, 0x62, 0x78, 0x33, 0x18, 0x57,
    0x70, 0xb, 0x13, 0x90,

    /* U+0058 "X" */
    0x73, 0x36, 0x9, 0x80, 0x8, 0x90, 0x72, 0x37,

    /* U+0059 "Y" */
    0x81, 0x35, 0x8, 0x80, 0x6, 0x20, 0x6, 0x20,

    /* U+005A "Z" */
    0x47, 0xc1, 0x4, 0x50, 0x18, 0x0, 0xa8, 0x72,

    /* U+005B "[" */
    0x56, 0x70, 0x70, 0x70, 0x70, 0x56,

    /* U+005C "\\" */
    0x80, 0x4, 0x30, 0x8, 0x0, 0x71,

    /* U+005D "]" */
    0x65, 0x8, 0x8, 0x8, 0x8, 0x65,

    /* U+005E "^" */
    0x1b, 0x7, 0x25,

    /* U+005F "_" */
    0x66, 0x30,

    /* U+0060 "`" */
    0x43,

    /* U+0061 "a" */
    0x37, 0x73, 0x5a, 0x76, 0xa0,

    /* U+0062 "b" */
    0x80, 0x0, 0x87, 0x81, 0x80, 0x44, 0x77, 0x81,

    /* U+0063 "c" */
    0x47, 0x59, 0x0, 0x47, 0x50,

    /* U+0064 "d" */
    0x0, 0x53, 0x47, 0xa3, 0x80, 0x53, 0x46, 0x93,

    /* U+0065 "e" */
    0x48, 0x80, 0x96, 0x80, 0x47, 0x60,

    /* U+0066 "f" */
    0x18, 0x15, 0x30, 0x88, 0x5, 0x20, 0x52, 0x0,

    /* U+0067 "g" */
    0x47, 0xa3, 0x80, 0x53, 0x47, 0xa2, 0x37, 0x80,

    /* U+0068 "h" */
    0x80, 0x0, 0x87, 0x90, 0x80, 0x70, 0x80, 0x71,

    /* U+0069 "i" */
    0x40, 0x80, 0x80, 0x80,

    /* U+006A "j" */
    0x4, 0x0, 0x80, 0x8, 0x0, 0x80, 0x48, 0x0,

    /* U+006B "k" */
    0x80, 0x0, 0x82, 0x70, 0x8b, 0x10, 0x80, 0x90,

    /* U+006C "l" */
    0x88, 0x88,

    /* U+006D "m" */
    0x77, 0xa6, 0x68, 0x8, 0x8, 0x80, 0x80, 0x80,

    /* U+006E "n" */
    0x76, 0x90, 0x80, 0x70, 0x80, 0x71,

    /* U+006F "o" */
    0x47, 0x80, 0x80, 0x53, 0x47, 0x80,

    /* U+0070 "p" */
    0x76, 0x81, 0x80, 0x44, 0x87, 0x81, 0x80, 0x0,

    /* U+0071 "q" */
    0x47, 0xa3, 0x80, 0x63, 0x47, 0xa3, 0x0, 0x53,

    /* U+0072 "r" */
    0x76, 0x8, 0x0, 0x80, 0x0,

    /* U+0073 "s" */
    0x77, 0x34, 0x73, 0x57, 0x50,

    /* U+0074 "t" */
    0x61, 0xa, 0x70, 0x61, 0x4, 0x80,

    /* U+0075 "u" */
    0x80, 0x70, 0x80, 0x70, 0x57, 0xb0,

    /* U+0076 "v" */
    0x90, 0x80, 0x45, 0x60, 0xb, 0x0,

    /* U+0077 "w" */
    0x80, 0xb0, 0x65, 0x77, 0x71, 0xa, 0xa, 0x0,

    /* U+0078 "x" */
    0x83, 0x70, 0xd0, 0x73, 0x70,

    /* U+0079 "y" */
    0x90, 0x80, 0x46, 0x50, 0xb, 0x0, 0x65, 0x0,

    /* U+007A "z" */
    0x49, 0x60, 0x80, 0x97, 0x40,

    /* U+007B "{" */
    0x7, 0x1, 0x60, 0x25, 0x8, 0x20, 0x26, 0x0,
    0x80,

    /* U+007C "|" */
    0x77, 0x77, 0x70,

    /* U+007D "}" */
    0x71, 0x35, 0x25, 0xa, 0x35, 0x72,

    /* U+007E "~" */
    0x35, 0x40, 0x42, 0x60,

    /* U+529F "功" */
    0x6, 0x71, 0x70, 0x0, 0x16, 0x6b, 0xb0, 0x1,
    0x60, 0x77, 0x0, 0x5a, 0x54, 0x70, 0x2, 0x19,
    0x39, 0x0, 0x0, 0x0, 0x0,

    /* U+538B "压" */
    0xb, 0x78, 0x75, 0x7, 0x8, 0x0, 0x9, 0x8c,
    0x84, 0x16, 0x8, 0x62, 0x66, 0x7b, 0x77, 0x10,
    0x0, 0x0,

    /* U+6D41 "流" */
    0x0, 0x1, 0x0, 0x45, 0x7c, 0x74, 0x50, 0x63,
    0x60, 0x15, 0x99, 0x93, 0x34, 0x66, 0x72, 0x74,
    0x46, 0x77, 0x0, 0x0, 0x0,

    /* U+7387 "率" */
    0x0, 0x0, 0x0, 0x57, 0xb7, 0x72, 0x54, 0x96,
    0x60, 0x16, 0x86, 0x60, 0x44, 0xa6, 0x32, 0x67,
    0xa7, 0x73,

    /* U+7535 "电" */
    0x0, 0x70, 0x0, 0x77, 0xb7, 0xa0, 0x76, 0xa6,
    0x90, 0x67, 0xb7, 0x91, 0x0, 0x77, 0x75, 0x0,
    0x0, 0x0,

    /* U+8868 "表" */
    0x46, 0xa6, 0x61, 0x26, 0xa6, 0x50, 0x56, 0xb7,
    0x62, 0x9, 0x38, 0x70, 0x68, 0x54, 0x82, 0x3,
    0x10, 0x1,

    /* U+F001 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x49, 0xec, 0x0,
    0xbf, 0xea, 0xc0, 0xb, 0x30, 0xc, 0x0, 0xb0,
    0x5, 0xc0, 0x8e, 0x5, 0xfa, 0x1b, 0xa0, 0x0,
    0x0,

    /* U+F008 "" */
    0x47, 0x88, 0x74, 0x8a, 0x11, 0xa8, 0x9c, 0x88,
    0xc9, 0xaa, 0x0, 0xaa, 0x9d, 0x99, 0xd9,

    /* U+F00B "" */
    0xeb, 0xcf, 0xfe, 0x86, 0x79, 0x98, 0xfb, 0xdf,
    0xff, 0x86, 0x79, 0x98, 0xfb, 0xdf, 0xff, 0x65,
    0x57, 0x76,

    /* U+F00C "" */
    0x0, 0x0, 0x4, 0x0, 0x0, 0xcc, 0xc5, 0xc,
    0xc1, 0x6f, 0xdc, 0x10, 0x5, 0xc1, 0x0,

    /* U+F00D "" */
    0x20, 0x2, 0xd, 0x86, 0xe0, 0x2e, 0xf3, 0x6,
    0xee, 0x80, 0xb3, 0x2b, 0x0,

    /* U+F011 "" */
    0x0, 0x2, 0x0, 0x0, 0x53, 0xa2, 0x20, 0x89,
    0x4b, 0x3d, 0xd, 0x13, 0xa0, 0xb4, 0xc3, 0x1,
    0xc, 0x35, 0xd4, 0x39, 0xb0, 0x4, 0xbb, 0x70,
    0x0,

    /* U+F013 "" */
    0x0, 0x11, 0x0, 0x1, 0xdd, 0x10, 0x8f, 0xee,
    0xf8, 0x5f, 0x22, 0xf5, 0x8f, 0x88, 0xf8, 0x39,
    0xff, 0x94, 0x0, 0x77, 0x0,

    /* U+F015 "" */
    0x0, 0x6d, 0x6a, 0x0, 0x9a, 0x8c, 0xb0, 0xb9,
    0xdf, 0xba, 0x71, 0xdf, 0xdf, 0x91, 0xe, 0xd1,
    0xfa, 0x0, 0x43, 0x4, 0x20,

    /* U+F019 "" */
    0x0, 0x6, 0x30, 0x0, 0x0, 0xe9, 0x0, 0x0,
    0xe, 0x90, 0x0, 0xd, 0xff, 0x80, 0x4, 0x5c,
    0x94, 0x32, 0xff, 0x9d, 0xfd, 0x1b, 0xbb, 0xb7,
    0x80,

    /* U+F01C "" */
    0x7, 0xbb, 0xb5, 0x4, 0xa0, 0x0, 0xc1, 0xd5,
    0x10, 0x28, 0x9f, 0xfc, 0x8e, 0xfc, 0xef, 0xff,
    0xff, 0xa0,

    /* U+F021 "" */
    0x0, 0x0, 0x3, 0x8, 0xdd, 0x8d, 0x88, 0x3,
    0xdf, 0x50, 0x4, 0x66, 0xdd, 0x70, 0x1a, 0xfc,
    0x22, 0xc5, 0x95, 0xcc, 0x50,

    /* U+F026 "" */
    0x0, 0x34, 0x6f, 0xff, 0xfe, 0xff, 0x1, 0xd0,
    0x0,

    /* U+F027 "" */
    0x0, 0x30, 0x4, 0x6f, 0x0, 0xff, 0xf4, 0x6e,
    0xff, 0x32, 0x1, 0xd0, 0x0, 0x0, 0x0,

    /* U+F028 "" */
    0x0, 0x0, 0x0, 0x0, 0x3, 0x1, 0xa0, 0x46,
    0xf0, 0x76, 0x7f, 0xff, 0x36, 0x89, 0xef, 0xf3,
    0x57, 0x90, 0x1d, 0x5, 0x83, 0x0, 0x0, 0x14,
    0x0,

    /* U+F03E "" */
    0x9a, 0xaa, 0xa8, 0xc3, 0xff, 0xff, 0xfa, 0xf7,
    0x3f, 0xe1, 0x30, 0xc, 0xdc, 0xcc, 0xcd,

    /* U+F043 "" */
    0x0, 0x0, 0x0, 0x79, 0x0, 0x1e, 0xf2, 0x9,
    0xff, 0xb0, 0xdf, 0xff, 0x1b, 0x8e, 0xe0, 0x1a,
    0xa2, 0x0,

    /* U+F048 "" */
    0x0, 0x0, 0x0, 0xc0, 0x59, 0xd, 0x6f, 0xb0,
    0xff, 0xfb, 0xd, 0xbf, 0xb0, 0xd0, 0xab, 0x3,
    0x0, 0x20,

    /* U+F04B "" */
    0x20, 0x0, 0x0, 0xfc, 0x30, 0x0, 0xff, 0xf9,
    0x10, 0xff, 0xff, 0xe2, 0xff, 0xff, 0x80, 0xff,
    0xb2, 0x0, 0xa4, 0x0, 0x0,

    /* U+F04C "" */
    0xde, 0x2d, 0xe2, 0xff, 0x4f, 0xf4, 0xff, 0x4f,
    0xf4, 0xff, 0x4f, 0xf4, 0xff, 0x3f, 0xf3, 0x34,
    0x3, 0x40,

    /* U+F04D "" */
    0xdf, 0xff, 0xe2, 0xff, 0xff, 0xf4, 0xff, 0xff,
    0xf4, 0xff, 0xff, 0xf4, 0xff, 0xff, 0xf3, 0x34,
    0x44, 0x40,

    /* U+F051 "" */
    0x0, 0x0, 0x0, 0xc2, 0x1a, 0xf, 0xe4, 0xb0,
    0xff, 0xfb, 0xf, 0xf9, 0xb0, 0xf5, 0x2b, 0x2,
    0x0, 0x30,

    /* U+F052 "" */
    0x0, 0x1a, 0x30, 0x0, 0xc, 0xfe, 0x20, 0xb,
    0xff, 0xfd, 0x10, 0x8a, 0xaa, 0x91, 0xf, 0xff,
    0xff, 0x30, 0x67, 0x77, 0x71,

    /* U+F053 "" */
    0x0, 0x83, 0x9, 0xb0, 0x7c, 0x0, 0x1d, 0x50,
    0x1, 0xd3, 0x0, 0x10,

    /* U+F054 "" */
    0x75, 0x0, 0x1d, 0x50, 0x1, 0xf3, 0x9, 0xb0,
    0x7b, 0x0, 0x10, 0x0,

    /* U+F067 "" */
    0x0, 0xc2, 0x0, 0x0, 0xf3, 0x0, 0xcd, 0xfd,
    0xc2, 0x45, 0xf7, 0x40, 0x0, 0xf3, 0x0, 0x0,
    0x40, 0x0,

    /* U+F068 "" */
    0xef, 0xff, 0xf3, 0x22, 0x22, 0x20,

    /* U+F06E "" */
    0x1, 0x68, 0x50, 0x3, 0xe4, 0x58, 0xc1, 0xdb,
    0x5f, 0x7f, 0x98, 0xe6, 0xd5, 0xf4, 0x6, 0xb9,
    0xb4, 0x0,

    /* U+F070 "" */
    0x20, 0x0, 0x0, 0x0, 0xa8, 0x48, 0x71, 0x0,
    0x6, 0xe5, 0x5e, 0x40, 0x79, 0x3e, 0xc9, 0xf0,
    0x3f, 0x51, 0xbe, 0xa0, 0x3, 0xb9, 0x39, 0x90,
    0x0, 0x0, 0x0, 0x54,

    /* U+F071 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xf1, 0x0,
    0x0, 0xd, 0xfa, 0x0, 0x0, 0x7e, 0x2f, 0x30,
    0x1, 0xff, 0x6f, 0xd0, 0xa, 0xff, 0x3f, 0xf6,
    0xb, 0xcc, 0xcc, 0xc8,

    /* U+F074 "" */
    0x0, 0x0, 0x53, 0xed, 0x1b, 0xfd, 0x4, 0xa9,
    0x42, 0x18, 0xb8, 0x64, 0xdb, 0x8, 0xed, 0x0,
    0x0, 0x41,

    /* U+F077 "" */
    0x0, 0xa2, 0x0, 0xb, 0xad, 0x20, 0xb8, 0x4,
    0xd1, 0x20, 0x0, 0x20,

    /* U+F078 "" */
    0x92, 0x0, 0xa1, 0x4d, 0x2b, 0x80, 0x5, 0xf8,
    0x0, 0x0, 0x20, 0x0,

    /* U+F079 "" */
    0x4, 0x0, 0x0, 0x0, 0x8f, 0x88, 0xcc, 0x90,
    0x4c, 0x40, 0x2, 0x90, 0xc, 0x0, 0x19, 0xb6,
    0xa, 0xcc, 0x49, 0xe2, 0x0, 0x0, 0x0, 0x0,

    /* U+F07B "" */
    0x68, 0x50, 0x0, 0xff, 0xfc, 0xca, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xfe,

    /* U+F093 "" */
    0x0, 0x22, 0x0, 0x2, 0xee, 0x20, 0xa, 0xff,
    0xa0, 0x0, 0xcc, 0x0, 0x44, 0xbb, 0x44, 0xff,
    0x77, 0xff, 0xbb, 0xbb, 0x89,

    /* U+F095 "" */
    0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0xfe, 0x0,
    0x0, 0x2f, 0xd0, 0x0, 0x0, 0xc8, 0x2, 0x70,
    0x9d, 0x10, 0xef, 0xfd, 0x20, 0x8, 0xa6, 0x0,
    0x0,

    /* U+F0C4 "" */
    0x0, 0x0, 0x0, 0xbc, 0x14, 0xd1, 0xcd, 0x9f,
    0x40, 0xa, 0xf8, 0x0, 0xbd, 0xaf, 0x40, 0xcc,
    0x15, 0xe1, 0x1, 0x0, 0x0,

    /* U+F0C5 "" */
    0x1, 0x21, 0x0, 0x18, 0xfb, 0xb0, 0xfa, 0xff,
    0xc2, 0xfa, 0xff, 0xf3, 0xfa, 0xff, 0xf3, 0xf8,
    0xaa, 0x91, 0xbc, 0xc8, 0x0,

    /* U+F0C7 "" */
    0xcc, 0xcc, 0x70, 0xc0, 0x1, 0xf5, 0xfc, 0xcc,
    0xf6, 0xff, 0x1a, 0xf6, 0xff, 0xbe, 0xf6, 0x12,
    0x22, 0x20,

    /* U+F0C9 "" */
    0xcc, 0xcc, 0xc2, 0x11, 0x11, 0x10, 0xdd, 0xdd,
    0xd3, 0x1, 0x11, 0x10, 0xdd, 0xdd, 0xd3,

    /* U+F0E0 "" */
    0x68, 0x88, 0x86, 0xcf, 0xff, 0xfc, 0xba, 0xff,
    0xab, 0xfe, 0x99, 0xef, 0xef, 0xff, 0xfe,

    /* U+F0E7 "" */
    0x2, 0x42, 0x0, 0xbf, 0x70, 0xd, 0xf8, 0x40,
    0xff, 0xf7, 0x1, 0x8d, 0x0, 0xa, 0x40, 0x0,
    0x70, 0x0,

    /* U+F0EA "" */
    0x0, 0x0, 0x0, 0xaa, 0xb7, 0x0, 0xfd, 0xa7,
    0x10, 0xfa, 0xfb, 0x90, 0xfa, 0xff, 0xf3, 0x86,
    0xff, 0xf3, 0x1, 0xaa, 0xa1,

    /* U+F0F3 "" */
    0x0, 0x10, 0x0, 0x4, 0xe6, 0x0, 0x1f, 0xff,
    0x40, 0x4f, 0xff, 0x80, 0x9f, 0xff, 0xc0, 0x9a,
    0xaa, 0x91, 0x0, 0xa2, 0x0,

    /* U+F11C "" */
    0x68, 0x88, 0x88, 0x4d, 0x98, 0x86, 0x7b, 0xf9,
    0xa9, 0xab, 0xce, 0xa6, 0x68, 0x9c, 0xdd, 0xcc,
    0xcd, 0xa0,

    /* U+F124 "" */
    0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x6d, 0xe0,
    0x1, 0x7e, 0xff, 0x80, 0xd, 0xff, 0xff, 0x10,
    0x1, 0x27, 0xf9, 0x0, 0x0, 0x6, 0xf2, 0x0,
    0x0, 0x3, 0x80, 0x0,

    /* U+F15B "" */
    0x0, 0x0, 0xf, 0xfa, 0xb0, 0xff, 0xba, 0x4f,
    0xff, 0xf8, 0xff, 0xff, 0x8f, 0xff, 0xf8, 0xef,
    0xff, 0x70,

    /* U+F1EB "" */
    0x0, 0x0, 0x0, 0x0, 0x5, 0xbd, 0xc8, 0x20,
    0xba, 0x41, 0x26, 0xd4, 0x13, 0xbe, 0xd7, 0x10,
    0x4, 0x40, 0x17, 0x0, 0x0, 0xe, 0x60, 0x0,
    0x0, 0x3, 0x10, 0x0,

    /* U+F240 "" */
    0xdc, 0xcc, 0xcc, 0xd4, 0xcc, 0xff, 0xff, 0x6b,
    0xc7, 0x99, 0x99, 0x69, 0xac, 0xcc, 0xcc, 0xb0,

    /* U+F241 "" */
    0xdc, 0xcc, 0xcc, 0xd4, 0xcc, 0xff, 0xf0, 0x3b,
    0xc7, 0x99, 0x90, 0x59, 0xac, 0xcc, 0xcc, 0xb0,

    /* U+F242 "" */
    0xdc, 0xcc, 0xcc, 0xd4, 0xcc, 0xfc, 0x0, 0x3b,
    0xc7, 0x97, 0x0, 0x59, 0xac, 0xcc, 0xcc, 0xb0,

    /* U+F243 "" */
    0xdc, 0xcc, 0xcc, 0xd4, 0xcc, 0xa0, 0x0, 0x3b,
    0xc7, 0x60, 0x0, 0x59, 0xac, 0xcc, 0xcc, 0xb0,

    /* U+F244 "" */
    0xdc, 0xcc, 0xcc, 0xc2, 0xc0, 0x0, 0x0, 0x3b,
    0xc0, 0x0, 0x0, 0x5a, 0xac, 0xcc, 0xcc, 0xb0,

    /* U+F287 "" */
    0x0, 0x0, 0x40, 0x0, 0x0, 0x38, 0xb0, 0x0,
    0xda, 0xa7, 0x66, 0xb3, 0x63, 0x7, 0x12, 0x30,
    0x0, 0x2, 0xad, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+F293 "" */
    0x0, 0x10, 0x3, 0xec, 0xd1, 0xac, 0x6a, 0x7d,
    0xb2, 0xca, 0xc9, 0x3a, 0x99, 0xe6, 0xb7, 0x1a,
    0xba, 0x0,

    /* U+F2ED "" */
    0x0, 0x41, 0x0, 0xbc, 0xed, 0xc2, 0x7c, 0xcc,
    0xa0, 0xaa, 0xaa, 0xe0, 0xaa, 0xaa, 0xe0, 0xaa,
    0xaa, 0xe0, 0x6c, 0xcc, 0x90,

    /* U+F304 "" */
    0x0, 0x0, 0x1, 0x0, 0x0, 0x2, 0xfa, 0x0,
    0x4, 0xd9, 0xb0, 0x4, 0xff, 0xa0, 0x4, 0xff,
    0xb0, 0x0, 0xef, 0xb0, 0x0, 0xb, 0x80, 0x0,
    0x0,

    /* U+F55A "" */
    0x1, 0x78, 0x88, 0x71, 0x1d, 0xfa, 0xea, 0xf7,
    0xcf, 0xfa, 0xa, 0xf8, 0x6f, 0xf6, 0x76, 0xf8,
    0x6, 0xff, 0xff, 0xf5,

    /* U+F7C2 "" */
    0x0, 0x0, 0x2, 0xbd, 0xd8, 0xe7, 0x78, 0xbf,
    0xff, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xfb, 0xcf,
    0xff, 0x70,

    /* U+F8A2 "" */
    0x0, 0x0, 0x8, 0x4, 0xd2, 0x22, 0xe0, 0xcf,
    0xdd, 0xdc, 0x1, 0x90, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 26, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 23, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 33, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 7, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 55, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 27, .adv_w = 74, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 68, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 18, .box_w = 1, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 52, .adv_w = 33, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 57, .adv_w = 33, .box_w = 2, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 62, .adv_w = 41, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 67, .adv_w = 55, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 73, .adv_w = 23, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 75, .adv_w = 47, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 77, .adv_w = 22, .box_w = 2, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 37, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 55, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 24, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 25, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 169, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 55, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 181, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 41, .box_w = 4, .box_h = 5, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 94, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 217, .adv_w = 64, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 69, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 57, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 54, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 67, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 25, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 44, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 65, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 301, .adv_w = 54, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 83, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 319, .adv_w = 71, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 73, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 57, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 73, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 358, .adv_w = 62, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 366, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 70, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 390, .adv_w = 63, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 93, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 410, .adv_w = 63, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 59, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 426, .adv_w = 55, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 33, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 440, .adv_w = 37, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 33, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 452, .adv_w = 45, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 455, .adv_w = 40, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 457, .adv_w = 29, .box_w = 2, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 458, .adv_w = 52, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 471, .adv_w = 47, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 484, .adv_w = 53, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 33, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 506, .adv_w = 56, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 514, .adv_w = 23, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 23, .box_w = 3, .box_h = 5, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 526, .adv_w = 50, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 534, .adv_w = 23, .box_w = 1, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 82, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 544, .adv_w = 56, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 550, .adv_w = 57, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 556, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 564, .adv_w = 58, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 572, .adv_w = 36, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 44, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 35, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 588, .adv_w = 56, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 594, .adv_w = 49, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 600, .adv_w = 75, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 608, .adv_w = 48, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 50, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 621, .adv_w = 46, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 626, .adv_w = 35, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 635, .adv_w = 18, .box_w = 1, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 638, .adv_w = 35, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 644, .adv_w = 55, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 648, .adv_w = 96, .box_w = 7, .box_h = 6, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 669, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 687, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 708, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 726, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 744, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 762, .adv_w = 96, .box_w = 7, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 787, .adv_w = 96, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 802, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 820, .adv_w = 96, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 835, .adv_w = 66, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 848, .adv_w = 96, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 873, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 894, .adv_w = 108, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 915, .adv_w = 96, .box_w = 7, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 940, .adv_w = 108, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 958, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 979, .adv_w = 48, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 988, .adv_w = 72, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1003, .adv_w = 108, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1028, .adv_w = 96, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1043, .adv_w = 66, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1061, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1079, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1100, .adv_w = 84, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1118, .adv_w = 84, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1136, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1154, .adv_w = 84, .box_w = 7, .box_h = 6, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1175, .adv_w = 60, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1187, .adv_w = 60, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1199, .adv_w = 84, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1217, .adv_w = 84, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1223, .adv_w = 108, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1241, .adv_w = 120, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1269, .adv_w = 108, .box_w = 8, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1297, .adv_w = 96, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1315, .adv_w = 84, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1327, .adv_w = 84, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1339, .adv_w = 120, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1363, .adv_w = 96, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1378, .adv_w = 96, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1399, .adv_w = 96, .box_w = 7, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1424, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1445, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1466, .adv_w = 84, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1484, .adv_w = 84, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1499, .adv_w = 96, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1514, .adv_w = 60, .box_w = 5, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1532, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1553, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1574, .adv_w = 108, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1592, .adv_w = 96, .box_w = 8, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1620, .adv_w = 72, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1638, .adv_w = 120, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1666, .adv_w = 120, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1682, .adv_w = 120, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1698, .adv_w = 120, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1714, .adv_w = 120, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1730, .adv_w = 120, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1746, .adv_w = 120, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1770, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1788, .adv_w = 84, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1809, .adv_w = 96, .box_w = 7, .box_h = 7, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1834, .adv_w = 120, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1854, .adv_w = 72, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1872, .adv_w = 97, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0xec, 0x1aa2, 0x20e8, 0x2296, 0x35c9, 0x9d62, 0x9d69,
    0x9d6c, 0x9d6d, 0x9d6e, 0x9d72, 0x9d74, 0x9d76, 0x9d7a, 0x9d7d,
    0x9d82, 0x9d87, 0x9d88, 0x9d89, 0x9d9f, 0x9da4, 0x9da9, 0x9dac,
    0x9dad, 0x9dae, 0x9db2, 0x9db3, 0x9db4, 0x9db5, 0x9dc8, 0x9dc9,
    0x9dcf, 0x9dd1, 0x9dd2, 0x9dd5, 0x9dd8, 0x9dd9, 0x9dda, 0x9ddc,
    0x9df4, 0x9df6, 0x9e25, 0x9e26, 0x9e28, 0x9e2a, 0x9e41, 0x9e48,
    0x9e4b, 0x9e54, 0x9e7d, 0x9e85, 0x9ebc, 0x9f4c, 0x9fa1, 0x9fa2,
    0x9fa3, 0x9fa4, 0x9fa5, 0x9fe8, 0x9ff4, 0xa04e, 0xa065, 0xa2bb,
    0xa523, 0xa603
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 21151, .range_length = 42500, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 66, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 0,
    1, 2, 3, 0, 4, 0, 4, 0,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
    6, 7, 8, 9, 10, 7, 11, 12,
    13, 14, 14, 15, 16, 17, 14, 14,
    7, 18, 0, 19, 20, 21, 15, 5,
    22, 23, 24, 25, 2, 8, 3, 0,
    0, 0, 26, 27, 28, 29, 30, 31,
    32, 26, 0, 33, 34, 29, 26, 26,
    27, 27, 0, 35, 36, 37, 32, 38,
    38, 39, 38, 40, 2, 0, 3, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 1, 0, 2, 0, 0, 0, 0,
    2, 0, 3, 0, 4, 5, 4, 5,
    6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0,
    7, 8, 6, 9, 8, 9, 9, 9,
    8, 9, 9, 10, 9, 9, 9, 9,
    8, 9, 8, 9, 11, 12, 13, 14,
    15, 16, 17, 18, 0, 14, 3, 0,
    5, 0, 19, 20, 21, 21, 21, 22,
    21, 20, 0, 23, 20, 20, 24, 24,
    21, 0, 21, 24, 25, 26, 27, 28,
    28, 29, 28, 30, 0, 0, 3, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 2, 2, 1,
    0, 1, 0, 0, 6, 0, 0, 2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 2, 2, -4, -13, -8, 2, -3,
    0, -11, -1, 2, 0, 0, 0, 0,
    0, 0, -7, 0, -7, -2, 0, -4,
    -5, 0, -4, -4, -5, -5, 0, 0,
    0, -3, -9, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    -4, -3, 0, 0, 0, -4, 0, -3,
    0, -3, -2, -3, -5, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, -11, 0, -6, 3, 0,
    -6, -3, 0, 0, 0, -8, -1, -8,
    -6, 0, -10, 2, 0, 0, -1, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, -1, 0, 0, 0, -1, 0, 0,
    0, 1, 0, -3, 0, -4, -1, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, -3, 2, 0, 3, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    1, 0, 1, -1, 0, 1, 0, 0,
    0, -1, 0, 0, -1, 0, -1, 0,
    -1, -2, 0, 0, -1, -1, -1, -2,
    -1, 0, -1, 3, 0, 1, -13, -5,
    4, 0, 0, -14, 0, 2, 0, 0,
    0, 0, 0, 0, -4, 0, -3, -1,
    0, -2, 0, -1, 0, -2, -4, -3,
    0, 0, 0, 0, 2, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, -1, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, 0, -11, 2, 0,
    0, -5, -1, 0, -1, 0, -2, 0,
    0, 0, 0, 0, -2, 0, -3, -4,
    0, -1, -1, -3, -4, -6, -3, 0,
    -5, -10, 0, -9, 3, 0, -7, -5,
    0, 2, -1, -12, -4, -14, -10, 0,
    -17, 0, -1, 0, -2, -2, 0, 0,
    0, -2, -3, -9, 0, 0, -1, 1,
    0, 1, -14, -8, 1, 0, 0, -15,
    0, 0, 0, 0, 0, -2, 0, -3,
    -3, 0, -3, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, -1, 0, -1,
    4, 0, 0, -1, 0, 0, 1, -1,
    -1, -2, -1, 0, -4, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 2, 0, 0, -1,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, 3, 0, -8, -11, -8, 3, -3,
    0, -13, 0, 2, 0, 2, 2, 0,
    0, 0, -11, 0, -11, -5, 0, -9,
    -11, -3, -8, -10, -10, -8, -1, 2,
    0, -2, -7, -6, 0, -2, 0, -6,
    0, 2, 0, 0, 0, 0, 0, 0,
    -7, 0, -6, -1, 0, -4, -4, 0,
    -3, -2, -3, -3, 0, 0, 2, -9,
    1, 0, 1, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -3, 0, 0, -2, -2, -2, -3, -6,
    0, 0, -3, 1, 2, -7, -13, -10,
    1, -5, 0, -13, -2, 0, 0, 0,
    0, 0, 0, 0, -10, 0, -10, -5,
    0, -8, -9, -3, -7, -7, -6, -7,
    0, 0, 1, -5, 2, 0, 1, -3,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -1, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, -4,
    0, 0, 0, 0, -3, 0, 0, 0,
    0, -8, 0, -7, -6, -1, -9, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, -5, 0, -1,
    -3, 0, -4, 0, 0, 0, 0, -11,
    0, -7, -6, -3, -10, 0, -1, 0,
    0, -1, 0, 0, 0, 0, 0, -2,
    -2, -2, 0, 0, 0, 2, 2, 0,
    -1, 0, 0, 0, 0, -7, 0, -4,
    -3, 2, -7, 0, 0, 0, 0, 1,
    0, 0, 0, 2, 0, 0, 1, 1,
    0, 0, 1, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, -2, 0, 0, 0,
    0, -7, 0, -5, -4, -1, -8, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 4, 0, -1,
    -7, 0, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -2, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, -9, 0, -4,
    -4, 0, -8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 3, 0, -4, 0, 0, 0,
    0, -9, 0, -5, -5, 0, -8, 0,
    -3, 0, -2, 0, 0, 0, -1, 0,
    -1, 0, 0, 0, 0, 2, 0, 1,
    -10, -3, -3, 0, 0, -10, 0, 0,
    0, -3, 0, -4, -6, 0, -4, 0,
    -3, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    -3, 0, 0, 0, 0, -10, 0, -6,
    -5, 0, -10, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 1,
    0, -1, 1, 0, 3, 0, -2, 0,
    0, 0, 0, -7, 0, -4, 0, 0,
    -6, 0, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -9, -4, -2, 0, 0, -7,
    0, -10, 0, -4, -2, -6, -7, 0,
    -2, 0, -2, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    1, 0, -4, 0, 0, 0, 0, -10,
    0, -5, -3, 0, -6, 0, -1, 0,
    -2, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    -3, 0, 0, 0, 0, -10, 0, -5,
    -3, 0, -7, 0, -2, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 40,
    .right_class_cnt     = 30,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_HarmonyOS_Sans_SC_Regular_6 = {
#else
lv_font_t lv_font_HarmonyOS_Sans_SC_Regular_6 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 6,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_HARMONYOS_SANS_SC_REGULAR_6*/

