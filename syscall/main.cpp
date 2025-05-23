#include <cstdlib>
#include <stdio.h>
#include "customWarning.h"
#include "IO.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        customPrint(yellow, bold, bgDefault, "Usage: %s <files count> <file_name> <..>", argv[0]);
        return EXIT_FAILURE;
    }

    fileContainer *container = initializeFileContainer(&argv[2], argc - 2);
    customWarning(container, CONTAINER_BAD_POINTER);

    for (size_t i = 0; i < container->size; i++) {
        if (container->files[i]) {
            openFile(container->files[i]);
        }
    }

    destroyFileContainer(container);

    return EXIT_SUCCESS;
}