#ifndef BASICS_H_
#define BASICS_H_

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum elbrusType {
    ELBRUS_1C_PLUS,
    ELBRUS_2C_PLUS,
    ELBRUS_2C3,
    ELBRUS_4C,
    ELBRUS_8C,
    ELBRUS_16C,
    ELBRUS_INVALID
};

struct elbrusMapping {
    elbrusType type  = ELBRUS_INVALID;
    const char *name = {};
};

const elbrusMapping elbrusMap[] = {
    {ELBRUS_1C_PLUS, "1c+"},
    {ELBRUS_2C_PLUS, "2c+"},
    {ELBRUS_2C3,     "2c3"},
    {ELBRUS_4C,      "4c" },
    {ELBRUS_8C,      "8c" },
    {ELBRUS_16C,     "16c"},
    {ELBRUS_INVALID, NULL }
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum optionType {
    OPT_M     =  0,
    OPT_C     =  1,
    OPT_S     =  2,
    OPT_T     =  3,
    OPT_COUNT =  4
};

static const char *optionTypeNames[] = {"m", "c", "s", "t"};

struct Options {
    int flags[OPT_COUNT] = {};

    elbrusType *elbrusVersions      = {};
    size_t      elbrusVersionsCount = {};

    char **nonOptions      = {};
    size_t nonOptionsCount = {};

    char *errorMsg = {};
};

const size_t ERROR_MESSAGE_SIZE = 128;

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // BASICS_H_