#pragma once

struct _font {
    int size;
    int width;
    void (*normal);
    void (*bold);
};

struct _fonts {
    struct _font font12;
    struct _font font14;
    struct _font font16;
    struct _font font18;
    struct _font font20;
    struct _font font22;
    struct _font font24;
    struct _font font28;
    struct _font font32;
};

extern const struct _fonts Fonts;
