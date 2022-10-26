#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist/linkedlist.h"
#include "shogilib.h"
#include "stacklib.h"
#include "utilities.h"

extern Attr_t pieceAttr[14];
extern Board_t bannmenn;
extern UserInput_t input;
extern Stack_t kifuStack;
extern List_t *kifuList;
int main(int argc, char **argv) {
    FILE *fptr;
    char initSfen[100] = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
    char sfenBuffer[150] = "";
    initialize();
    initializeStack(&kifuStack);

    if (argc >= 3) {
        if ((strcmp(argv[1], "--readsfen") == 0)) {
            fptr = fopen(argv[2], "r");
            fgets(sfenBuffer, 150, fptr);
            push(sfenBuffer, &kifuStack);
            if (!SFENParse(sfenBuffer))
                renderBoard();
        } else if (strcmp(argv[1], "--loadsfenlist") == 0) {
            fptr = fopen(argv[2], "r");
            readKifu(fptr);
            if (!SFENParse(kifuList->head->data))
                renderBoard();
            else
                puts("SFEN parsing failed\n");

            Node_t currentState = *(kifuList->head);
            char arg;
            while (arg = getc(stdin)) {
                if (arg == 'f')
                    scrollKifu(1, &currentState);
                else if (arg == 'b')
                    scrollKifu(0, &currentState);
                else if (arg == 'q')
                    break;
            }
            return 0;
        } else {
            printf("file opening failed. turn to initial board. \n");
            push(initSfen, &kifuStack);
            if (!SFENParse(initSfen))
                renderBoard();
        }
    } else {
        push(initSfen, &kifuStack);
        if (!SFENParse(initSfen))
            renderBoard();
    }

    int inputcode;
    while ((inputcode = userInput()) != -1) {
        if (inputcode == 1) {
            revert();
        } else if (inputcode == 2)
            puts("stack viewing complete.");
        else if (inputcode == 3) {
            puts("SFEN: ");
            sfenBuffer[0] = '\0';
            exportToSFEN(sfenBuffer);
            puts(sfenBuffer);
        } else if (validMove(input.init, input.final)) {
            sfenBuffer[0] = '\0';

            if (getPieceBycoord(input.init)->promoted == true || forcePromote()) {
                makeMove(input.init, input.final, 1);

            } else if (canPromote()) {
                int promote;
                char buffer[10];

                printf("<<成りますか？>>(naru, narazu)\n>>");
                while (fgets(buffer, 10, stdin)) {
                    if (strcmp(buffer, "naru\n") == 0) {
                        makeMove(input.init, input.final, 1);
                        break;
                    } else if (strcmp(buffer, "narazu\n") == 0) {
                        makeMove(input.init, input.final, 0);
                        break;
                    } else
                        printf("Your promotion argument is not OK.\n>>");
                }

            } else {
                makeMove(input.init, input.final, 0);
            }

            renderBoard();
            exportToSFEN(sfenBuffer);
            push(sfenBuffer, &kifuStack);
        }

        else
            printf("Invalid move.\n");
    }
    reverse(&kifuStack);
    generateKifu(&kifuStack);
    return 0;
}
