#ifndef INFO_H
#define INFO_H

#include "config.h"

typedef struct {
    char prompt[PROMPT_BUFFER];

    unsigned long long mem_total;
    unsigned long long mem_available;
    char mem[MEMORY_BUFFER];

    char kernel[KERNEL_BUFFER];

    double uptime_seconds;
    char uptime[UPTIME_BUFFER];

    char os[OS_BUFFER];

    char shell[SHELL_BUFFER];
} SystemInfo;

SystemInfo get_info();

#endif
