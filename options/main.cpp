#include "parser.h"
#include "basics.h"

int main(int argc, char *argv[]) {
    Options *options = initializeOptions();

    if (parseOptions(options, argc, argv)) {
        printOptions(options);
        destroyOptions(options);

        return EXIT_FAILURE;
    }

    printOptions(options);
    destroyOptions(options);

    return EXIT_SUCCESS;
}