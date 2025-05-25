#include <stdio.h>

#include "customWarning.h"
#include "sort.h"
#include "textStruct.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

int main(int argc, char *argv[]) {
    if (argc < 3) {
        customPrint(red,    bold, bgDefault, "Usage: %s <file> <sortType>\n", argv[0]);
        customPrint(yellow, bold, bgDefault, "Sort types: -f (forward), -b (backward), -l (length), -w (word count), -v (vowel count)\n");

        return EXIT_FAILURE;
    }

    textData text = {};
    initializeTextData(&text, argv[argc - 2]);

    if (argv[argc - 1][0] == '-') {
        switch (argv[argc - 1][1]) {
            case 'f':
                sortTextLines(&text, forwardStrcmp);
                break;

            case 'b':
                sortTextLines(&text, backwardStrcmp);
                break;

            case 'l':
                sortTextLines(&text, lengthStrcmp);
                break;

            case 'w':
                sortTextLines(&text, wordCountStrcmp);
                break;

            case 'v':
                sortTextLines(&text, vowelCountStrcmp);
                break;
            
            default:
                customPrint(red, bold, bgDefault, "Unknown sort type: %s\n", argv[argc - 1]);

                return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < text.lineCount; i++) {
        fwrite(text.lineArray[i].linePointer, sizeof(char), text.lineArray[i].lineSize, stdout);
        fputc('\n', stdout);
    }

    destroyTextData(&text);

    return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //