#include "customWarning.h"
#include "parser.h"
#include "basics.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static const size_t EXPAND_FACTOR = 2;

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Options *initializeOptions() {
    Options *options = (Options *)calloc(1, sizeof(Options));
    customWarning(options, NULL);

    memset(options->flags, 0, sizeof(options->flags));

    options->elbrusVersions      = {};
    options->elbrusVersionsCount = {};

    options->nonOptions         = {};
    options->nonOptionsCount    = {};

    options->errorMsg           = {};

    return options;
}

optionsError destroyOptions(Options *options) {
    customWarning(options, OPTIONS_BAD_POINTER);

    if (options->elbrusVersions) {
        FREE_(options->elbrusVersions);
    }

    if (options->nonOptions) {
        for (size_t i = 0; i < options->nonOptionsCount; i++) {
            FREE_(options->nonOptions[i]);
        }

        FREE_(options->nonOptions);
    }

    if (options->errorMsg) {
        FREE_(options->errorMsg);
    }

    FREE_(options);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

elbrusType getElbrusType(const char *name) {
    customWarning(name, ELBRUS_INVALID);

    for (size_t i = 0; elbrusMap[i].name; i++) {
        if (!strcmp(name, elbrusMap[i].name)) {
            return elbrusMap[i].type;
        }
    }

    return ELBRUS_INVALID;
}

optionsError setElbrusType(Options *options, const char *name) {
    customWarning(options, OPTIONS_BAD_POINTER);
    customWarning(name,    NAME_BAD_POINTER);

    elbrusType type = getElbrusType(name);
    
    if (type == ELBRUS_INVALID) {
        if (options->errorMsg) {
            FREE_(options->errorMsg);
        }

        options->errorMsg = (char *)calloc(ERROR_MESSAGE_SIZE, sizeof(char));
        customWarning(options->errorMsg, ALLOCATION_ERROR);

        snprintf(options->errorMsg, ERROR_MESSAGE_SIZE, "--elbrus=%s", name);
        return BAD_ELBRUS_TYPE;
    }

    size_t capacity = (options->elbrusVersionsCount == 0) ? 1 : options->elbrusVersionsCount * EXPAND_FACTOR;

    options->elbrusVersions = (elbrusType *)realloc(options->elbrusVersions, capacity * sizeof(elbrusType));
    customWarning(options->elbrusVersions, ALLOCATION_ERROR);

    options->elbrusVersions[options->elbrusVersionsCount++] = type;

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

optionsError addNonOption(Options *options, const char *value) {
    customWarning(options, OPTIONS_BAD_POINTER);
    customWarning(value,   VALUE_BAD_POINTER);

    size_t capacity = (options->nonOptionsCount == 0) ? 1 : options->nonOptionsCount * EXPAND_FACTOR;
    options->nonOptions = (char **)realloc(options->nonOptions, capacity * sizeof(char *));
    customWarning(options->nonOptions, ALLOCATION_ERROR);

    size_t valueSize = strlen(value);

    options->nonOptions[options->nonOptionsCount] = (char *)calloc(valueSize + 1, sizeof(char));
    customWarning(options->nonOptions[options->nonOptionsCount], ALLOCATION_ERROR);

    strncpy(options->nonOptions[options->nonOptionsCount], value, valueSize);
    options->nonOptionsCount++;

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

optionsError parseOptions(Options *options, int argc, char *argv[]) {
    customWarning(options, OPTIONS_BAD_POINTER);

    static struct option longOptions[] = {
        {"elbrus", required_argument, 0, 0},
        {0,        0,                 0, 0}
    };

    int opt         = {};
    int optionIndex = {};

    while ((opt = getopt_long(argc, argv, "mcst", longOptions, &optionIndex)) != -1) {
        switch (opt) {

            case 'm': {
                options->flags[OPT_M] = 1;
                break;
            }

            case 'c': {
                options->flags[OPT_C] = 1;
                break;
            }

            case 's': {
                options->flags[OPT_S] = 1;
                break;
            }

            case 't': {
                options->flags[OPT_T] = 1;
                break;
            }

            case 0: {
                if (setElbrusType(options, optarg)) {
                    return BAD_ELBRUS_TYPE;
                }
                
                break;
            }

            case '?': {
                if (options->errorMsg) {
                    FREE_(options->errorMsg);
                }

                options->errorMsg = (char *)calloc(ERROR_MESSAGE_SIZE, sizeof(char));
                customWarning(options->errorMsg, ALLOCATION_ERROR);

                if (optopt) {
                    snprintf(options->errorMsg, ERROR_MESSAGE_SIZE, "-%c", optopt);
                } else {
                    snprintf(options->errorMsg, ERROR_MESSAGE_SIZE, "%s", argv[optind - 1]);
                }

                return BAD_OPTION;
            }

            default: {
                return BAD_OPTION;
            }
        }
    }

    for (size_t i = optind; i < (size_t)argc; i++) {
        if (argv[i][0] == '-') {
            if (options->errorMsg) {
                FREE_(options->errorMsg);
            }

            options->errorMsg = (char *)calloc(ERROR_MESSAGE_SIZE, sizeof(char));
            customWarning(options->errorMsg, ALLOCATION_ERROR);

            snprintf(options->errorMsg, ERROR_MESSAGE_SIZE, "%s", argv[i]);
            return BAD_NON_OPTION;
        }

        addNonOption(options, argv[i]);
    }

    return NO_ERRORS;
}

optionsError printOptions(const Options *options) {
    customWarning(options, OPTIONS_BAD_POINTER);

    if (options->errorMsg) {
        customPrint(red, bold, bgDefault, "Options are incorrect: ");
        fprintf(stderr, "%s\n", options->errorMsg);

        return NO_ERRORS;
    }

    customPrint(green, bold, bgDefault, "Options are correct:\n");
    
    int flag = 1;

    for (size_t i = 0; i < OPT_COUNT; i++) {
        if (options->flags[i]) {
            if (flag) {
                fprintf(stderr, "%s", optionTypeNames[i]);
            } else {
                fprintf(stderr, ", %s", optionTypeNames[i]);
            }

            flag = 0;
        }
    }

    for (size_t i = 0; i < options->elbrusVersionsCount; i++) {
        fprintf(stderr, "%selbrus=%s", flag ? "" : ", ", elbrusMap[options->elbrusVersions[i]].name);
        flag = 0;
    }

    if (options->nonOptionsCount > 0) {
        fprintf(stderr, ", non-options: ");

        for (size_t i = 0; i < options->nonOptionsCount; i++) {
            fprintf(stderr, "%s%s", i ? ", " : "", options->nonOptions[i]);
        }
    }

    fprintf(stderr, "\n");

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //