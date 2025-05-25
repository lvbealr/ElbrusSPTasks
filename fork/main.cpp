#include "IO.h"
#include "fork.h"
#include "customWarning.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        customPrint(red, bold, bgDefault, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return processFork(argv[1]);
}