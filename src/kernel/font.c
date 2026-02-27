#include "font.h"

extern void _binary_fonts_ter_u12n_sfn_start;
extern void _binary_fonts_ter_u12b_sfn_start;
const struct _font Font12 = {
    .size = 12,
    .width = 6,
    .normal = &_binary_fonts_ter_u12n_sfn_start,
    .bold = &_binary_fonts_ter_u12b_sfn_start
};

extern void _binary_fonts_ter_u14n_sfn_start;
extern void _binary_fonts_ter_u14b_sfn_start;
const struct _font Font14 = {
    .size = 14,
    .width = 8,
    .normal = &_binary_fonts_ter_u14n_sfn_start,
    .bold = &_binary_fonts_ter_u14b_sfn_start
};

extern void _binary_fonts_ter_u16n_sfn_start;
extern void _binary_fonts_ter_u16b_sfn_start;
const struct _font Font16 = {
    .size = 16,
    .width = 8,
    .normal = &_binary_fonts_ter_u16n_sfn_start,
    .bold = &_binary_fonts_ter_u16b_sfn_start
};

extern void _binary_fonts_ter_u18n_sfn_start;
extern void _binary_fonts_ter_u18b_sfn_start;
const struct _font Font18 = {
    .size = 18,
    .width = 10,
    .normal = &_binary_fonts_ter_u18n_sfn_start,
    .bold = &_binary_fonts_ter_u18b_sfn_start
};

extern void _binary_fonts_ter_u20n_sfn_start;
extern void _binary_fonts_ter_u20b_sfn_start;
const struct _font Font20 = {
    .size = 20,
    .width = 10,
    .normal = &_binary_fonts_ter_u20n_sfn_start,
    .bold = &_binary_fonts_ter_u20b_sfn_start
};

extern void _binary_fonts_ter_u22n_sfn_start;
extern void _binary_fonts_ter_u22b_sfn_start;
const struct _font Font22 = {
    .size = 22,
    .width = 11,
    .normal = &_binary_fonts_ter_u22n_sfn_start,
    .bold = &_binary_fonts_ter_u22b_sfn_start
};

extern void _binary_fonts_ter_u24n_sfn_start;
extern void _binary_fonts_ter_u24b_sfn_start;
const struct _font Font24 = {
    .size = 24,
    .width = 12,
    .normal = &_binary_fonts_ter_u24n_sfn_start,
    .bold = &_binary_fonts_ter_u24b_sfn_start
};

extern void _binary_fonts_ter_u28n_sfn_start;
extern void _binary_fonts_ter_u28b_sfn_start;
const struct _font Font28 = {
    .size = 28,
    .width = 14,
    .normal = &_binary_fonts_ter_u28n_sfn_start,
    .bold = &_binary_fonts_ter_u28b_sfn_start
};

extern void _binary_fonts_ter_u32n_sfn_start;
extern void _binary_fonts_ter_u32b_sfn_start;
const struct _font Font32 = {
    .size = 32,
    .width = 16,
    .normal = &_binary_fonts_ter_u32n_sfn_start,
    .bold = &_binary_fonts_ter_u32b_sfn_start
};

const struct _fonts Fonts = {
    .font12 = Font12,
    .font14 = Font14,
    .font16 = Font16,
    .font18 = Font18,
    .font20 = Font20,
    .font22 = Font22,
    .font24 = Font24,
    .font28 = Font28,
    .font32 = Font32
};
