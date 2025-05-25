#ifndef PARSER_H_
#define PARSER_H_

#include "basics.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum optionsError {
    NO_ERRORS = 0,
    OPTIONS_BAD_POINTER = 1,
    ALLOCATION_ERROR = 2,
    BAD_ELBRUS_TYPE = 3,
    VALUE_BAD_POINTER = 4,
    NAME_BAD_POINTER = 5,
    BAD_OPTION = 6,
    BAD_NON_OPTION = 7
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Options     *initializeOptions();
optionsError destroyOptions   (Options *options);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

elbrusType   getElbrusType(const char *name);
optionsError setElbrusType(Options *options, const char *name);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

optionsError addNonOption(Options *options, const char *value);
// -------------------------------------------------------------------------------------------------------------------------------------------------- //

optionsError parseOptions(      Options *options, int argc, char *argv[]);
optionsError printOptions(const Options *options);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // PARSER_H_