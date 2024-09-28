/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/skelig/SquareLine/assets/fonts/SourceHanSansCN-Medium.otf -o C:/Users/skelig/SquareLine/assets/fonts\ui_font_FontLast.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONTLAST
#define UI_FONT_FONTLAST 1
#endif

#if UI_FONT_FONTLAST

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0x3,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xbd, 0x80,

    /* U+0023 "#" */
    0x22, 0x22, 0x22, 0xff, 0x26, 0x64, 0x64, 0xff,
    0x44, 0x44, 0x44, 0x4c,

    /* U+0024 "$" */
    0x30, 0x61, 0xf6, 0x2c, 0x18, 0x1c, 0x1e, 0xe,
    0xc, 0x1c, 0x3f, 0xc6, 0xc, 0x0,

    /* U+0025 "%" */
    0x78, 0x43, 0x31, 0xc, 0xc8, 0x33, 0x20, 0xcd,
    0x3, 0x35, 0xe7, 0xac, 0xc0, 0xb3, 0x4, 0xcc,
    0x13, 0x30, 0x8c, 0xc4, 0x1e,

    /* U+0026 "&" */
    0x3c, 0xc, 0xc1, 0x98, 0x33, 0x2, 0xc0, 0x70,
    0x1e, 0x36, 0xc6, 0xcf, 0x98, 0xf3, 0x9f, 0x1f,
    0x60,

    /* U+0027 "'" */
    0xff, 0xc0,

    /* U+0028 "(" */
    0x32, 0x66, 0xcc, 0xcc, 0xcc, 0xcc, 0x66, 0x23,

    /* U+0029 ")" */
    0xc4, 0x66, 0x33, 0x33, 0x33, 0x33, 0x66, 0x4c,

    /* U+002A "*" */
    0x20, 0xcf, 0xcc, 0x79, 0x0,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18,

    /* U+002C "," */
    0xf3, 0x7c,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0xc, 0x20, 0x82, 0x18, 0x41, 0xc, 0x20, 0x82,
    0x18, 0x41, 0xc, 0x20,

    /* U+0030 "0" */
    0x3c, 0x66, 0x42, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x42, 0x66, 0x3c,

    /* U+0031 "1" */
    0x31, 0xe0, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc7, 0xf0,

    /* U+0032 "2" */
    0x78, 0xcc, 0x6, 0x6, 0x6, 0xe, 0xc, 0x1c,
    0x38, 0x30, 0x60, 0xff,

    /* U+0033 "3" */
    0x7d, 0x8c, 0x18, 0x30, 0xc7, 0x3, 0x3, 0x7,
    0xe, 0x33, 0xc0,

    /* U+0034 "4" */
    0xe, 0xe, 0x1e, 0x16, 0x36, 0x66, 0x66, 0xc6,
    0xff, 0x6, 0x6, 0x6,

    /* U+0035 "5" */
    0x7e, 0x60, 0x60, 0x60, 0x7c, 0x46, 0x3, 0x3,
    0x3, 0x3, 0xc6, 0x7c,

    /* U+0036 "6" */
    0x1e, 0x62, 0x60, 0xc0, 0xc0, 0xde, 0xe7, 0xc3,
    0xc3, 0x43, 0x66, 0x3c,

    /* U+0037 "7" */
    0xff, 0x6, 0xe, 0xc, 0x18, 0x18, 0x18, 0x30,
    0x30, 0x30, 0x30, 0x30,

    /* U+0038 "8" */
    0x7d, 0x8f, 0x1e, 0x36, 0x47, 0x1f, 0x67, 0xc7,
    0x8f, 0x99, 0xe0,

    /* U+0039 "9" */
    0x3c, 0x66, 0xc2, 0xc3, 0xc3, 0xe7, 0x7b, 0x3,
    0x3, 0x6, 0x46, 0x78,

    /* U+003A ":" */
    0xc0, 0x0, 0xc0,

    /* U+003B ";" */
    0xc0, 0x0, 0x0, 0xef, 0xe0,

    /* U+003C "<" */
    0x1, 0x7, 0x3c, 0xe0, 0xe0, 0x3c, 0x7, 0x1,

    /* U+003D "=" */
    0xff, 0x0, 0x0, 0x0, 0xff,

    /* U+003E ">" */
    0x80, 0xe0, 0x3c, 0x7, 0x7, 0x3c, 0xe0, 0x80,

    /* U+003F "?" */
    0x7c, 0x8c, 0x18, 0x30, 0xe1, 0x86, 0x18, 0x30,
    0x0, 0x1, 0x80,

    /* U+0040 "@" */
    0x7, 0xc0, 0x70, 0xc3, 0x0, 0x98, 0x3, 0x67,
    0xc7, 0x33, 0x1d, 0x8c, 0x76, 0x31, 0xd8, 0x8f,
    0x67, 0x26, 0xe7, 0x18, 0x0, 0x38, 0x0, 0x3f,
    0x0,

    /* U+0041 "A" */
    0xc, 0x7, 0x81, 0xe0, 0x58, 0x33, 0xc, 0xc2,
    0x31, 0xfe, 0x61, 0x98, 0x6c, 0xb, 0x3,

    /* U+0042 "B" */
    0xfe, 0x63, 0xb0, 0xd8, 0x6c, 0x67, 0xe3, 0xd,
    0x83, 0xc1, 0xe0, 0xf0, 0xdf, 0xc0,

    /* U+0043 "C" */
    0x1f, 0x18, 0x98, 0x18, 0xc, 0x6, 0x3, 0x1,
    0x80, 0xc0, 0x30, 0xc, 0x63, 0xe0,

    /* U+0044 "D" */
    0xfc, 0x63, 0x30, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xe1, 0xb1, 0x9f, 0x80,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0xb0, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0x60, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0047 "G" */
    0x1f, 0x18, 0x98, 0x18, 0xc, 0x6, 0x3, 0x1f,
    0x83, 0xc1, 0xb0, 0xcc, 0x63, 0xe0,

    /* U+0048 "H" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1f, 0xff, 0x7,
    0x83, 0xc1, 0xe0, 0xf0, 0x78, 0x30,

    /* U+0049 "I" */
    0xff, 0xff, 0xff,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0xf, 0x33, 0xc0,

    /* U+004B "K" */
    0xc3, 0x63, 0x31, 0x99, 0x8d, 0x87, 0xc3, 0xf1,
    0xcc, 0xc6, 0x61, 0xb0, 0xd8, 0x30,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+004D "M" */
    0xe1, 0xf8, 0x7e, 0x1f, 0x8f, 0xf2, 0xf4, 0xbd,
    0x2f, 0x73, 0xcc, 0xf3, 0x3c, 0xcf, 0x3,

    /* U+004E "N" */
    0xc1, 0xf0, 0xfc, 0x7e, 0x3d, 0x9e, 0xcf, 0x37,
    0x9b, 0xc7, 0xe3, 0xf0, 0xf8, 0x30,

    /* U+004F "O" */
    0x1e, 0x1c, 0xe6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xd8, 0x67, 0x38, 0x78,

    /* U+0050 "P" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x1e, 0x18, 0xe6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x61, 0xd8, 0x63, 0xf0, 0x30, 0x6,
    0x0, 0xf0,

    /* U+0052 "R" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xcc,
    0xcc, 0xc6, 0xc7, 0xc3,

    /* U+0053 "S" */
    0x1f, 0x10, 0x98, 0xc, 0x7, 0x81, 0xf0, 0x7c,
    0xf, 0x1, 0x80, 0xd8, 0xc7, 0xc0,

    /* U+0054 "T" */
    0xff, 0x8c, 0x6, 0x3, 0x1, 0x80, 0xc0, 0x60,
    0x30, 0x18, 0xc, 0x6, 0x3, 0x0,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xe0, 0xd8, 0xc7, 0xc0,

    /* U+0056 "V" */
    0xc1, 0xb0, 0x66, 0x19, 0x84, 0x63, 0x8, 0xc3,
    0x30, 0xc8, 0x16, 0x7, 0x81, 0xc0, 0x70,

    /* U+0057 "W" */
    0xc3, 0xd, 0x8c, 0x36, 0x38, 0x98, 0xe2, 0x66,
    0x99, 0x9a, 0x62, 0x4d, 0x8d, 0x34, 0x3c, 0xd0,
    0xf1, 0x43, 0x87, 0x6, 0x1c,

    /* U+0058 "X" */
    0x61, 0xb1, 0x8c, 0xc6, 0xc1, 0xe0, 0xe0, 0x70,
    0x2c, 0x36, 0x11, 0x98, 0xc8, 0x30,

    /* U+0059 "Y" */
    0x61, 0x98, 0x63, 0x30, 0xcc, 0x12, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0xff, 0x7, 0x6, 0xc, 0xc, 0x18, 0x18, 0x30,
    0x60, 0x60, 0xc0, 0xff,

    /* U+005B "[" */
    0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xf0,

    /* U+005C "\\" */
    0xc1, 0x4, 0x18, 0x20, 0x82, 0xc, 0x10, 0x41,
    0x82, 0x8, 0x20, 0xc1,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xf0,

    /* U+005E "^" */
    0x18, 0x70, 0xa3, 0x66, 0x48, 0xf1, 0x80,

    /* U+005F "_" */
    0xff, 0x80,

    /* U+0060 "`" */
    0x46, 0x30,

    /* U+0061 "a" */
    0x7c, 0x8c, 0x18, 0xf7, 0xf8, 0xf1, 0xe7, 0x7e,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xe6, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc6, 0xfc,

    /* U+0063 "c" */
    0x3e, 0xc3, 0x6, 0xc, 0x18, 0x30, 0x31, 0x3e,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3, 0x3f, 0x63, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3, 0x67, 0x3f,

    /* U+0065 "e" */
    0x3c, 0x66, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0x62,
    0x3e,

    /* U+0066 "f" */
    0x3d, 0x86, 0x18, 0xf9, 0x86, 0x18, 0x61, 0x86,
    0x18, 0x60,

    /* U+0067 "g" */
    0x7f, 0xc6, 0xc6, 0xc6, 0x4e, 0x3c, 0xc0, 0xc0,
    0x7e, 0xc3, 0xc3, 0xc6, 0x7c,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xd, 0xdc, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x3c, 0x60,

    /* U+0069 "i" */
    0xc0, 0xff, 0xff, 0xc0,

    /* U+006A "j" */
    0x30, 0x0, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0xe0,

    /* U+006B "k" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc6, 0xcc, 0xd8, 0xd8,
    0xf8, 0xec, 0xcc, 0xc6, 0xc3,

    /* U+006C "l" */
    0xdb, 0x6d, 0xb6, 0xdb, 0x66,

    /* U+006D "m" */
    0xdc, 0xee, 0x73, 0xc6, 0x3c, 0x63, 0xc6, 0x3c,
    0x63, 0xc6, 0x3c, 0x63, 0xc6, 0x30,

    /* U+006E "n" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,

    /* U+006F "o" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66,
    0x3c,

    /* U+0070 "p" */
    0xfc, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc6,
    0xfc, 0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0071 "q" */
    0x3f, 0x63, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x67,
    0x3f, 0x3, 0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xdf, 0x31, 0x8c, 0x63, 0x18, 0xc0,

    /* U+0073 "s" */
    0x7b, 0x2c, 0x3c, 0x78, 0x70, 0xe3, 0xf8,

    /* U+0074 "t" */
    0x63, 0x19, 0xf6, 0x31, 0x8c, 0x63, 0x18, 0x70,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe7, 0x76,

    /* U+0076 "v" */
    0xc1, 0x31, 0x98, 0xcc, 0x42, 0x61, 0xb0, 0xd0,
    0x38, 0x1c, 0x0,

    /* U+0077 "w" */
    0x43, 0x1b, 0x38, 0x99, 0x44, 0xca, 0x62, 0x5b,
    0x1e, 0xd0, 0xf2, 0x87, 0x1c, 0x18, 0xe0,

    /* U+0078 "x" */
    0x63, 0x66, 0x36, 0x1c, 0x18, 0x3c, 0x26, 0x66,
    0x43,

    /* U+0079 "y" */
    0xc1, 0x31, 0x98, 0xcc, 0x43, 0x61, 0xb0, 0x50,
    0x28, 0x1c, 0x4, 0x6, 0x2, 0x6, 0x0,

    /* U+007A "z" */
    0x7e, 0x1c, 0x30, 0xc3, 0x86, 0x1c, 0x30, 0xfe,

    /* U+007B "{" */
    0x1c, 0xc3, 0xc, 0x30, 0xc3, 0x38, 0x30, 0xc3,
    0xc, 0x30, 0xc1, 0xc0,

    /* U+007C "|" */
    0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xe1, 0x8c, 0x63, 0x18, 0xc3, 0x31, 0x8c, 0x63,
    0x1b, 0x80,

    /* U+007E "~" */
    0x62, 0x92, 0x8c
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 58, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 88, .box_w = 2, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 133, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 8, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 146, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 34, .adv_w = 240, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 180, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 76, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 74, .adv_w = 91, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 82, .adv_w = 91, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 90, .adv_w = 124, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 95, .adv_w = 146, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 103, .adv_w = 76, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 105, .adv_w = 91, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 106, .adv_w = 76, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 100, .box_w = 6, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 119, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 146, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 146, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 76, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 76, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 244, .adv_w = 146, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 252, .adv_w = 146, .box_w = 8, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 257, .adv_w = 146, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 265, .adv_w = 126, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 276, .adv_w = 249, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 301, .adv_w = 159, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 171, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 330, .adv_w = 165, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 179, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 145, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 380, .adv_w = 179, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 189, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 79, .box_w = 2, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 422, .adv_w = 170, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 143, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 212, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 188, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 193, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 166, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 503, .adv_w = 193, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 521, .adv_w = 168, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 156, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 156, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 188, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 152, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 229, .box_w = 14, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 153, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 142, .box_w = 10, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 155, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 652, .adv_w = 91, .box_w = 4, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 660, .adv_w = 100, .box_w = 6, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 672, .adv_w = 91, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 680, .adv_w = 146, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 687, .adv_w = 144, .box_w = 9, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 689, .adv_w = 157, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 691, .adv_w = 147, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 699, .adv_w = 161, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 712, .adv_w = 132, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 720, .adv_w = 161, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 733, .adv_w = 145, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 88, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 752, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 765, .adv_w = 159, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 777, .adv_w = 73, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 781, .adv_w = 74, .box_w = 4, .box_h = 17, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 790, .adv_w = 147, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 803, .adv_w = 76, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 808, .adv_w = 241, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 822, .adv_w = 160, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 839, .adv_w = 161, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 852, .adv_w = 161, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 865, .adv_w = 105, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 871, .adv_w = 123, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 878, .adv_w = 101, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 886, .adv_w = 159, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 894, .adv_w = 140, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 905, .adv_w = 212, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 920, .adv_w = 135, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 929, .adv_w = 139, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 944, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 952, .adv_w = 91, .box_w = 6, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 964, .adv_w = 72, .box_w = 1, .box_h = 18, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 967, .adv_w = 91, .box_w = 5, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 977, .adv_w = 146, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 5}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 3, 4, 3,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 6, 6, 0, 0, 0,
    0, 0, 7, 8, 9, 10, 11, 12,
    13, 0, 0, 14, 15, 16, 0, 0,
    10, 17, 10, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 2, 27, 0, 0,
    0, 0, 28, 29, 30, 0, 31, 32,
    33, 34, 0, 0, 35, 36, 34, 34,
    29, 29, 37, 38, 39, 40, 37, 41,
    42, 43, 44, 45, 2, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 0, 0, 0,
    2, 0, 3, 4, 0, 5, 6, 7,
    8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 9, 10, 0, 0, 0,
    11, 0, 12, 0, 13, 0, 0, 0,
    13, 0, 0, 14, 0, 0, 0, 0,
    13, 0, 13, 0, 15, 16, 17, 18,
    19, 20, 21, 22, 0, 23, 3, 0,
    0, 0, 24, 0, 25, 25, 25, 26,
    27, 0, 28, 29, 0, 0, 30, 30,
    25, 30, 25, 30, 31, 32, 33, 34,
    35, 36, 37, 38, 0, 0, 3, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, 0, -35, 0, -35, 0,
    0, 0, 0, -17, 0, -29, -3, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, -6, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 23, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -30, 0, -42,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -28, -6, -20, -10, 0,
    -28, 0, 0, 0, -3, 0, 0, 0,
    8, 0, 0, -15, 0, -12, -7, 0,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    -6, -14, 0, -5, -3, -9, -20, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, -3, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -12, -3, -23, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -7,
    -8, 0, -3, 6, 6, 0, 0, 1,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, -12, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -8, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -17, 0, -26,
    0, 0, 0, 0, 0, 0, -8, -2,
    -3, 0, 0, -17, -4, -4, 0, 0,
    -4, -2, -12, 6, 0, -3, 0, 0,
    0, 0, 6, -4, -2, -3, -2, -2,
    -3, 0, 0, 0, 0, -9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    -4, -7, 0, -2, -2, -2, -4, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -4, -3, -3, -4, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, -9, -3, -7, -5,
    -4, -2, -2, -2, -3, -3, 0, 0,
    0, 0, -6, 0, 0, 0, 0, -9,
    -3, -4, -3, 0, -4, 0, 0, 0,
    0, -9, 0, 0, 0, -5, 0, 0,
    0, -3, 0, -13, 0, -7, 0, -3,
    -2, -6, -6, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, 0, 0,
    0, 0, 0, -7, 0, -3, 0, -10,
    -3, 0, 0, 0, 0, 0, -23, 0,
    -23, -18, 0, 0, 0, -11, -3, -39,
    -6, 0, 0, 0, 0, -7, -1, -9,
    0, -10, -4, 0, -7, 0, 0, -6,
    -6, -3, -5, -7, -6, -8, -6, -10,
    0, 0, 0, -7, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, -6,
    0, -4, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -7, 0, -7, 0, 0, 0,
    0, 0, 0, -12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, 0, -12,
    0, -10, 0, 0, 0, 0, -3, -3,
    -6, 0, -4, -6, -4, -4, -3, 0,
    -6, 0, 0, 0, -3, 0, 0, 0,
    -3, 0, 0, -11, -4, -7, -6, -6,
    -7, -4, 0, -30, 0, -46, 0, -15,
    0, 0, 0, 0, -11, 0, -8, 0,
    -7, -35, -9, -22, -16, 0, -23, 0,
    -23, 0, -4, -4, -2, 0, 0, 0,
    0, -6, -3, -12, -10, 0, -12, 0,
    0, 0, 0, 0, -34, -9, -34, -19,
    0, 0, 0, -14, 0, -42, -3, -6,
    0, 0, 0, -7, -3, -20, 0, -12,
    -7, 0, -7, 0, 0, 0, -3, 0,
    0, 0, 0, -4, 0, -6, 0, 0,
    0, -3, 0, -9, 0, 0, 0, 0,
    0, -2, 0, -5, -4, -4, 0, 0,
    2, -2, -2, -3, 0, -2, -3, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -3, 0, 0, 0, -4,
    0, 4, 0, 0, 0, 0, 0, 0,
    0, -4, -4, -6, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, -31, -20,
    -31, -24, -6, -6, 0, -12, -7, -36,
    -10, 0, 0, 0, 0, -6, -4, -15,
    0, -20, -20, -5, -20, 0, 0, -13,
    -17, -5, -13, -9, -9, -10, -9, -21,
    0, 0, 0, 0, -6, 0, -6, -8,
    0, 0, 0, -4, 0, -14, -3, 0,
    0, -2, 0, -3, -4, 0, 0, -2,
    0, 0, -3, 0, 0, 0, -2, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    0, 0, -20, -5, -20, -12, 0, 0,
    0, -4, -3, -20, -3, 0, -3, 3,
    0, 0, 0, -5, 0, -7, -5, 0,
    -6, 0, 0, -6, -4, 0, -9, -3,
    -3, -5, -3, -7, 0, 0, 0, 0,
    -10, -3, -10, -7, 0, 0, 0, 0,
    -2, -17, -2, 0, 0, 0, 0, 0,
    0, -2, 0, -4, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -3, 0, -3, 0, -9,
    0, 0, 0, 0, 0, 0, -6, -2,
    -4, -6, -3, 0, 0, 0, 0, 0,
    0, -3, -3, -6, 0, 0, 0, 0,
    0, -6, -3, -6, -4, -3, -6, -4,
    0, 0, 0, 0, -28, -20, -28, -17,
    -8, -8, -3, -4, -4, -29, -5, -4,
    -3, 0, 0, 0, 0, -7, 0, -20,
    -13, 0, -17, 0, 0, -12, -13, -10,
    -10, -4, -7, -10, -4, -14, 0, 0,
    0, 0, 0, -9, 0, 0, 0, 0,
    0, -2, -6, -9, -9, 0, -3, -2,
    -2, 0, -4, -4, 0, -4, -6, -6,
    -4, 0, 0, 0, 0, -4, -6, -4,
    -4, -7, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -23, -7, -14, -7, 0,
    -20, 0, 0, 0, 0, 0, 9, 0,
    20, 0, 0, 0, 0, -6, -3, 0,
    3, 0, 0, 0, 0, -15, 0, 0,
    0, 0, 0, 0, -5, 0, 0, 0,
    0, -7, 0, -6, -2, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, -7, 0, -6, -3, 2, -3, 0,
    0, 0, -5, 0, 0, 0, 0, -17,
    0, -5, 0, -2, -14, 0, -8, -4,
    0, -1, 0, 0, 0, 0, -1, -6,
    0, -2, -2, -6, -2, -2, 0, 0,
    0, 0, 0, -7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -4,
    0, 0, -7, 0, 0, -3, -7, 0,
    -3, 0, 0, 0, 0, -3, 0, 2,
    2, 1, 2, 0, 0, 0, 0, -9,
    0, 3, 0, 0, 0, 0, -3, 0,
    0, -6, -6, -7, 0, -6, -3, 0,
    -8, 0, -7, -4, 0, -1, -3, 0,
    0, 0, 0, -3, 0, 0, 0, -3,
    0, 0, 3, 12, 13, 0, -16, -4,
    -16, -3, 0, 0, 7, 0, 0, 0,
    0, 13, 0, 19, 13, 9, 16, 0,
    15, -6, -3, 0, -4, 0, -3, 0,
    -2, 0, 0, 3, 0, -2, 0, -4,
    0, 0, 3, -9, 0, 0, 0, 12,
    0, 0, -11, 0, 0, 0, 0, -8,
    0, 0, 0, 0, -4, 0, 0, -5,
    -4, 0, 0, 0, 12, 0, 0, 0,
    0, -2, -2, 0, 3, -4, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    -3, 0, 0, 0, 0, -7, 0, -3,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    3, -16, 3, 0, 3, 3, -5, 0,
    0, 0, 0, -12, 0, 0, 0, 0,
    -4, 0, 0, -3, -6, 0, -3, 0,
    -3, 0, 0, -7, -4, 0, 0, -3,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, -4, 0, 0, -9, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -19, -7, -19, -9, 6, 6,
    0, -4, 0, -17, 0, 0, 0, 0,
    0, 0, 0, -3, 3, -7, -3, 0,
    -3, 0, 0, 0, -2, 0, 0, 6,
    4, 0, 6, -2, 0, 0, 0, -12,
    0, 3, 0, 0, 0, 0, -4, 0,
    0, 0, 0, -7, 0, -3, 0, 0,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 2, 3, 3, -8, 0, 0, 0,
    0, -4, 0, 0, 0, 0, -2, 0,
    0, -6, -4, 0, -3, 0, 0, 0,
    -3, -6, 0, 0, 0, -4, 0, 0,
    0, 0, 0, -3, -12, -3, -12, -6,
    0, 0, 0, -3, 0, -12, 0, -6,
    0, -3, 0, 0, -4, -3, 0, -6,
    -2, 0, 0, 0, -3, 0, 0, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, -3, -16, 0, -16, -2, 0, 0,
    0, -2, 0, -9, 0, -7, 0, -3,
    0, -4, -7, 0, 0, -3, -2, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, -6, -4, 0, 0, -6,
    3, -4, -3, 0, 0, 3, 0, 0,
    -3, 0, -2, -9, 0, -5, 0, -3,
    -10, 0, 0, -3, -6, 0, 0, 0,
    0, 0, 0, -7, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, -12, 0,
    -12, -3, 0, 0, 0, 0, 0, -12,
    0, -6, 0, -2, 0, -2, -3, 0,
    0, -6, -2, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, -4, 0, -7,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -7,
    0, 0, 0, 0, -7, 0, 0, -6,
    -3, 0, -2, 0, 0, 0, 0, 0,
    -3, -2, 0, 0, -2, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 45,
    .right_class_cnt     = 38,
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
    .cmap_num = 1,
    .bpp = 1,
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
const lv_font_t ui_font_FontLast = {
#else
lv_font_t ui_font_FontLast = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 18,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_FONTLAST*/

