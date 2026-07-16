#ifndef LOGOS_H
#define LOGOS_H

typedef struct {
    const char *name;
    const char *ascii;
} Logo;

extern const Logo logos[];
extern const int logo_count;

#endif
