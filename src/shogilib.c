#include "shogilib.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stacklib.h"
// #include "linkedlist/linkedlist.h"
Attr pieceAttr[14];
Board bannmenn;
UserInput input;
Stack kifuStack;
LinkedList kifuList;

char *token;
// initialize the project;
void initialize() {
    pieceAttr[FU].sfen = 'p';
    pieceAttr[KYO].sfen = 'l';
    pieceAttr[KEI].sfen = 'n';
    pieceAttr[GIN].sfen = 's';
    pieceAttr[KIN].sfen = 'g';
    pieceAttr[KAKU].sfen = 'b';
    pieceAttr[HI].sfen = 'r';
    pieceAttr[GYOKU].sfen = 'k';
    pieceAttr[FU].displayChar[0] = "歩";
    pieceAttr[KYO].displayChar[0] = "香";
    pieceAttr[KEI].displayChar[0] = "桂";
    pieceAttr[GIN].displayChar[0] = "銀";
    pieceAttr[KIN].displayChar[0] = "金";
    pieceAttr[KAKU].displayChar[0] = "角";
    pieceAttr[HI].displayChar[0] = "飛";
    pieceAttr[GYOKU].displayChar[0] = "玉";
    pieceAttr[FU].displayChar[1] = "と";
    pieceAttr[KYO].displayChar[1] = "杏";
    pieceAttr[KEI].displayChar[1] = "圭";
    pieceAttr[GIN].displayChar[1] = "全";
    pieceAttr[KAKU].displayChar[1] = "馬";
    pieceAttr[HI].displayChar[1] = "竜";
    pieceAttr[FU].name[0] = "fu";
    pieceAttr[KYO].name[0] = "kyo";
    pieceAttr[KEI].name[0] = "kei";
    pieceAttr[GIN].name[0] = "gin";
    pieceAttr[KIN].name[0] = "kin";
    pieceAttr[KAKU].name[0] = "kaku";
    pieceAttr[HI].name[0] = "hi";
    pieceAttr[GYOKU].name[0] = "gyoku";
    pieceAttr[FU].name[1] = "to";
    pieceAttr[KYO].name[1] = "narikyo";
    pieceAttr[KEI].name[1] = "narikei";
    pieceAttr[GIN].name[1] = "gin";
    pieceAttr[KIN].name[1] = "kin";
    pieceAttr[KAKU].name[1] = "uma";
    pieceAttr[HI].name[1] = "ryu";
    pieceAttr[GYOKU].name[1] = "gyoku";
    initializeList(&kifuList);
}
// reads KIF file into the program.
void readKifu(FILE *file) {
    
    if (file != NULL) {
        char *buffer = (char *)malloc(150 * sizeof(char));
        while(fgets(buffer, 150, file) != NULL){
            pushBack(&kifuList, createNode(buffer));
            buffer = (char*)malloc(150 * sizeof(char));
        }
        print(kifuList);
        // if (fgets(buffer, 150, file) != NULL) kifuList = initializeList(buffer);
        // buffer = (char *)malloc(150 * sizeof(char));
        // while (fgets(buffer, 150, file) != NULL) {
        //     append(buffer, kifuList);
        //     buffer = (char *)malloc(150 * sizeof(char));
        // }
        // printList(*kifuList);
    }
}
// stores the current sfen into the input string pointer.
void exportToSFEN(char *str) {
    // char str[150]  = "";
    char numstr[4] = "";
    int blanks = 0;
    int i, j;
    // bannmenn recording. Note that spaces are not processed.
    for (i = 0; i < 9; i++) {
        blanks = 0;
        for (j = 0; j < 9; j++) {
            if (bannmenn.pieces[i][j].type == ' ') {
                blanks++;
                continue;
            }
            if (blanks != 0) {
                append_str(str, blanks + '0');
                blanks = 0;
            }
            if (bannmenn.pieces[i][j].promoted) {
                append_str(str, '+');
                // continue;
            }

            append_str(str, bannmenn.pieces[i][j].type);
        }
        if (j == 9 && blanks != 0) append_str(str, blanks + '0');
        if (i != 8) append_str(str, '/');
    }
    // teban recording
    append_str(str, ' ');
    append_str(str, bannmenn.turn ? 'w' : 'b');
    // mochikoma recording
    bool mochikomaEmpty = true;
    append_str(str, ' ');
    for (int i = GYOKU; i >= FU; i--) {
        if (bannmenn.senteKomadai.komaList[i] != 0) {
            if (bannmenn.senteKomadai.komaList[i] > 1)
                append_str(str, bannmenn.senteKomadai.komaList[i] + '0');
            append_str(str, toupper(pieceAttr[i].sfen));
            mochikomaEmpty = false;
        }
    }
    for (int i = GYOKU; i >= FU; i--) {
        if (bannmenn.goteKomadai.komaList[i] != 0) {
            if (bannmenn.goteKomadai.komaList[i] > 1)
                append_str(str, bannmenn.goteKomadai.komaList[i] + '0');
            append_str(str, tolower(pieceAttr[i].sfen));
            mochikomaEmpty = false;
        }
    }
    if (mochikomaEmpty) append_str(str, '-');
    sprintf(numstr, " %d", bannmenn.moveNumber);
    strcat(str, numstr);

    // puts(str);
    // return str;
}
bool SFENParse(char *sfen) {
    SfenData_t sfenData;
    char *temp = (char *)malloc(150 * sizeof(char));
    strcpy(temp, sfen);
    const char delim[2] = " ";
    char *token;
    char *rawSfenBoard;
    if (Regex(temp, SFEN_REGEX) == 0) {
        token = strtok(temp, delim);
        rawSfenBoard = token;
        token = strtok(NULL, delim);
        sfenData.turn = *token;
        token = strtok(NULL, delim);
        sfenData.mochiKomaList = token;
        token = strtok(NULL, delim);
        sfenData.moveNumber = atoi(token);

        token = strtok(rawSfenBoard, "/");
        for (int i = 0; i < 9; i++) {
            sfenData.matrix[i] = token;
            token = strtok(NULL, "/");
        }
    } else
        return 1;
    SFENLoad(sfenData);
    free(temp);
    return 0;
}
// shows the board based on current database
void SFENLoad(SfenData_t data) {
    /* turn and move number parsing */
    bannmenn.turn = (data.turn == 'w') ? GOTE : SENTE;
    bannmenn.moveNumber = data.moveNumber;
    /* main board parsing */
    for (int i = 0; i < 9; i++) {
        int currentIndex = 0;
        for (int it_char = 0; it_char < strlen(data.matrix[i]);
             it_char++) {  // each row

            switch (data.matrix[i][it_char])  // each character
            {
                    // merge uppercase with lowercase
                case 'p':
                case 'l':
                case 'n':
                case 's':
                case 'g':
                case 'b':
                case 'r':
                case 'k':
                    bannmenn.pieces[i][currentIndex].type = data.matrix[i][it_char];

                    currentIndex++;
                    break;
                case 'P':
                case 'L':
                case 'N':
                case 'S':
                case 'G':
                case 'B':
                case 'R':
                case 'K':
                    bannmenn.pieces[i][currentIndex].type = data.matrix[i][it_char];

                    currentIndex++;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':  // numbers (spaces)
                    for (int k = 0; k < (data.matrix[i][it_char] - '0'); k++) {
                        bannmenn.pieces[i][currentIndex].type = ' ';
                        bannmenn.pieces[i][currentIndex].promoted = 0;
                        currentIndex++;
                    }
                    break;
                case '+':
                    bannmenn.pieces[i][currentIndex].promoted = true;
                    break;
                default:
                    break;
            }
        }
    }
    /* Mochikoma Parsing */
    int komaNumber = 1;
    for (int it_char = 0; it_char < strlen(data.mochiKomaList); it_char++) {
        char objectChar = data.mochiKomaList[it_char];
        if (isdigit(objectChar))
            komaNumber = ctoi(objectChar);
        else if (isupper(objectChar)) {
            bannmenn.senteKomadai.komaList[getPieceNumber(objectChar)] += komaNumber;
            komaNumber = 1;
        } else if (islower(objectChar)) {
            bannmenn.goteKomadai.komaList[getPieceNumber(objectChar)] += komaNumber;
            komaNumber = 1;
        }
    }
    // for(int i = 0; i < 8; i++) {
    //     printf("%d %d %d\n",i,
    //     bannmenn.senteKomadai.komaList[i],bannmenn.goteKomadai.komaList[i]);
    // }
}
int getPieceNumber(char c) {
    if (!isalpha(c)) printf("!!!");
    c = tolower(c);
    // printf("%c",c);
    for (int i = FU; i <= GYOKU; i++) {
        if (pieceAttr[i].sfen == c) return i;
        // printf("OK");
    }
    return -1;
}
void renderBoard() {
    // printf("\033c"); // clear screen
    // displays data
    puts("------------------");

    printf("%d手目、", bannmenn.moveNumber);
    (bannmenn.turn) ? printColor(RED,"後手番です\n") : printColor(BLUE, "先手番です\n");
    puts("------------------");
    // displays Board
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (bannmenn.pieces[i][j].type == ' ') {
                printf("  ");
            } else {
                if (!owner(bannmenn.pieces[i][j].type))
                    printColor(BLUE, getPieceName(bannmenn.pieces[i][j]));
                else
                    printColor(RED, getPieceName(bannmenn.pieces[i][j]));
            }
        }
        puts("");
    }
    puts("------持  駒------");

    // displays mochiKoma
    bool komaEmpty = 1;
    printColor(BLUE, "先手  ");
    for (int i = GYOKU; i >= FU; i--) {
        if (bannmenn.senteKomadai.komaList[i] != 0) komaEmpty = 0;
        for (int j = 1; j <= bannmenn.senteKomadai.komaList[i]; j++)
            printColor(BLUE, pieceAttr[i].displayChar[0]);
    }
    if (komaEmpty) printColor(BLUE, "なし");

    komaEmpty = 1;
    printColor(RED, "\n後手  ");
    for (int i = GYOKU; i >= FU; i--) {
        if (bannmenn.goteKomadai.komaList[i] != 0) komaEmpty = 0;
        for (int j = 1; j <= bannmenn.goteKomadai.komaList[i]; j++)
            printColor(RED, pieceAttr[i].displayChar[0]);
    }
    if (komaEmpty) printColor(RED, "なし");

    puts("\n------------------");
}
// let the user browse through shogi moves
enum { PREV,
       NEXT };
void scrollKifu(bool function, Node *current) {
    if (function == NEXT) {
        if (current->next != NULL) {
            initializeBoard();
            // SFENParse May corrupt the data. needs to fix.
            SFENParse(current->next->data);
            renderBoard();
            *current = *(current->next);
        }
        else
            puts("reached the last element of kifulist.");
    } else {
        Node ptr = *(*kifuList.head);
        if (current->data == (*kifuList.head)->data) {
            puts("reached the first element of kifulist.");
        } else {
            while (ptr.next->data != current->data) {
                ptr = *(ptr.next);
            }
            *current = ptr;
            initializeBoard();
            SFENParse(current->data);
            renderBoard();
        }
    }
}
// let user enter the move to interact with the shogi board
int userInput() {
    char rawInput[20];

    printf(">>");
    fgets(rawInput, 20, stdin);

    while (Regex(rawInput, INPUT_REGEX_FULL) == 1) {
        if (strcmp(rawInput, "quit\n") == 0)
            return -1;
        else if (strcmp(rawInput, "revert\n") == 0)
            return 1;
        else if (strcmp(rawInput, "stack\n") == 0) {
            inspectStack(&kifuStack);
            return 2;
        } else if (strcmp(rawInput, "sfen\n") == 0) {
            return 3;
        } else if (strcmp(rawInput, "\n") == 0) {
            printf(">>");
            fgets(rawInput, 20, stdin);
        } else {
            printf("Format Error. \nPlease enter the correct format.\n>>");
            fgets(rawInput, 20, stdin);
        }
    }
    sscanf(rawInput, "%1d%1d %1d%1d %s", &input.init.X, &input.init.Y,
           &input.final.X, &input.final.Y, input.type);
    return 0;

    // printf("%d%d, %d%d , %s", input.init.X, input.init.Y,  input.final.X,
    // input.final.Y,input.type);
}
// back to the origin from user inputs
void returnToOrigin() {}

char coordTransfer(char axis, char input) {
    if (axis != 'X' && axis != 'Y')
        return -1;
    else if (axis == 'X')
        return 9 - input;
    else
        return input - 1;
}

Piece_t *getPieceBycoord(Location_t loc) {
    return &bannmenn.pieces[coordTransfer('Y', loc.Y)][coordTransfer('X', loc.X)];
}

char *getPieceName(Piece_t piece) {
    return pieceAttr[getPieceNumber(piece.type)].displayChar[piece.promoted];
}
void makeMove(Location_t init, Location_t final, bool promote) {
    if (init.X == 0 && init.Y == 0) {
        if (bannmenn.turn == SENTE) {
            bannmenn.senteKomadai.komaList[getPieceNumByName(input.type)]--;
            getPieceBycoord(final)->type =
                toupper(pieceAttr[getPieceNumByName(input.type)].sfen);
        } else {
            bannmenn.goteKomadai.komaList[getPieceNumByName(input.type)]--;
            getPieceBycoord(final)->type =
                tolower(pieceAttr[getPieceNumByName(input.type)].sfen);
        }

        getPieceBycoord(final)->promoted = 0;
        bannmenn.moveNumber++;
        bannmenn.turn = !bannmenn.turn;
        return;
    }
    if (getPieceBycoord(final)->type != ' ') {
        if (owner(getPieceBycoord(final)->type) == SENTE)

            bannmenn.goteKomadai
                .komaList[getPieceNumber(getPieceBycoord(final)->type)]++;

        else

            bannmenn.senteKomadai
                .komaList[getPieceNumber(getPieceBycoord(final)->type)]++;
    }

    getPieceBycoord(final)->promoted = promote;
    getPieceBycoord(final)->type = getPieceBycoord(init)->type;
    getPieceBycoord(init)->type = ' ';
    bannmenn.moveNumber++;
    bannmenn.turn = !bannmenn.turn;
}

char owner(char pieceType) {
    if (pieceType == ' ')
        return -1;
    else if (isupper(pieceType))
        return SENTE;
    else
        return GOTE;
}
bool validMove(Location_t init, Location_t final) {
    Location_t diff;
    // need to combine with Makemove
    if (init.X == 0 && init.Y == 0) {
        if (getPieceBycoord(final)->type != ' ') {
            printf("You're putting your own pieces on a existing piece. \n");
            return false;
        } else if(strcmp(input.type, "fu" ) == 0){
            Location_t tempLoc;
            tempLoc.X = final.X;
            for (tempLoc.Y = 1; tempLoc.Y <= 9; tempLoc.Y++ ){
                if(tolower( getPieceBycoord(tempLoc)->type )== 'p' && getPieceBycoord(tempLoc)->promoted == false ){
                    printf("Nifu desu. \n");
                    return false;
                }       
            }
        } 
        else if (bannmenn.turn == SENTE) {
            if (bannmenn.senteKomadai.komaList[getPieceNumByName(input.type)] == 0) {
                printf("You have no sufficient piece to put on the board. \n");
                return false;
            }
        } else {
            if (bannmenn.goteKomadai.komaList[getPieceNumByName(input.type)] == 0) {
                printf("You have no sufficient piece to put on the board. \n");
                return false;
            }
        }
        return true;
    }
    Piece_t *piece = getPieceBycoord(init);

    // prevent accessing empty spaces
    if (piece->type == ' ') {
        printf("You have accessed an empty space.\n");
        return false;
    }
    // checks if entered piece name is correct
    if (strcmp(input.type,
               pieceAttr[getPieceNumber(piece->type)].name[piece->promoted]) !=
        0) {
        printf("your piece name is not valid.\n");
        return false;
    }

    // prevent taking owned pieces
    if (owner(getPieceBycoord(final)->type) ==
        owner(getPieceBycoord(init)->type)) {
        printf("You're taking your own pieces.\n");
        return false;
    }
    // prevent accessing opponent's pieces
    if (owner(getPieceBycoord(init)->type) != bannmenn.turn) {
        printf("Do not move your opponent's pieces.\n");
        return false;
    }
    // mochikoma detection

    else {
        // calculating position differences
        diff.X = final.X - init.X;
        diff.Y = final.Y - init.Y;
        switch (tolower(piece->type)) {
            case 'p':  // FU
                if (piece->promoted)
                    return kinDetection(piece->type, diff);
                else {
                    if (owner(piece->type) == SENTE) {
                        if (diff.Y == -1 && diff.X == 0) return true;
                    } else if (diff.Y == 1 && diff.X == 0)
                        return true;
                }
                break;

            case 'l':  // KYO
                if (piece->promoted)
                    return kinDetection(piece->type, diff);
                else {
                    if (owner(piece->type) == SENTE) {
                        if (diff.Y < 0 && diff.X == 0) return true;
                    } else if (diff.Y > 0 && diff.X == 0)
                        return true;
                }
                break;
            case 'n':  // KEI
                if (piece->promoted)
                    return kinDetection(piece->type, diff);
                else {
                    if (owner(piece->type) == SENTE) {
                        if (diff.Y == -2 && abs(diff.X) == 1) return true;
                    } else if (diff.Y == 2 && abs(diff.X) == 1)
                        return true;
                }
                break;
            case 's':  // GIN
                if (piece->promoted)
                    return kinDetection(piece->type, diff);
                else {
                    if (owner(piece->type) == SENTE) {
                        if (abs(diff.Y) == 1 && abs(diff.X) == 1 ||
                            (diff.Y == -1 && diff.X == 0))
                            return true;
                    } else if (abs(diff.Y) == 1 && abs(diff.X) == 1 ||
                               (diff.Y == 1 && diff.X == 0))
                        return true;
                }
                break;
            case 'g':  // KIN
                return kinDetection(piece->type, diff);
                break;
            case 'b':  // KAKU
                if (piece->promoted)
                    return (kakuMove(diff, init) || gyokuMove(diff));
                else {
                    return kakuMove(diff, init);
                }
                break;
            case 'r':  // HI
                if (piece->promoted)
                    return (hisyaMove(diff, init) || gyokuMove(diff));
                else
                    return hisyaMove(diff, init);
                break;
            case 'k':  // GYOKU
                return gyokuMove(diff);
                break;
        }
        return false;
    }
}

bool kinMove(Location_t loc, bool owner) {
    if (owner == SENTE) {
        if (max(abs(loc.X), abs(loc.Y)) == 1 && !(loc.Y == 1 && abs(loc.X) == 1))
            return true;
    }

    else if (max(abs(loc.X), abs(loc.Y)) == 1 && !(loc.Y == -1 && abs(loc.X) == 1))
        return true;

    return false;
}

bool gyokuMove(Location_t loc) { return (max(abs(loc.X), abs(loc.Y)) == 1); }
int getPieceNumByName(char *str) {
    for (int i = FU; i <= GYOKU; i++) {
        for (int j = 0; j <= 1; j++)
            if (strcmp(pieceAttr[i].name[j], str) == 0) return i;
    }
    return -1;
}

bool kinDetection(char type, Location_t diff) {
    if (owner(type) == SENTE)
        return kinMove(diff, SENTE);
    else
        return kinMove(diff, GOTE);
}

bool kakuMove(Location_t diff, Location_t init) {
    Location_t testPos;

    if (diff.X == diff.Y) {
        testPos.X = init.X;
        testPos.Y = init.Y;
        for (int i = 1; i < abs(diff.X); i++) {
            testPos.X += abs(diff.X) / diff.X;
            testPos.Y += abs(diff.Y) / diff.Y;
            if (getPieceBycoord(testPos)->type != ' ') {
                printf("You're moving across pieces.\n");
                return false;
            }
        }
        return true;
    } else
        return false;
}
bool hisyaMove(Location_t diff, Location_t init) {
    Location_t testPos;
    if (diff.X * diff.Y == 0) {
        testPos.X = init.X;
        testPos.Y = init.Y;
        for (int i = 1; i < max(abs(diff.X), abs(diff.Y)); i++) {
            if (diff.X != 0) testPos.X += abs(diff.X) / diff.X;
            if (diff.Y != 0) testPos.Y += abs(diff.Y) / diff.Y;
            if (getPieceBycoord(testPos)->type != ' ') {
                printf("You're moving across pieces.\n");
                return false;
            }
        }
        return true;
    } else
        return false;
}
void generateKifu(LinkedList list) {
    FILE *fileptr;
    fileptr = fopen("./kifu.sfenlist", "w");
    if (fileptr == NULL) {
        printf("!!!!!\n");
    } else {
        Node* tracer = (*list.head);
        while(tracer){
            fprintf(fileptr, "%s\n", tracer->data);
            tracer = tracer->next;
        }
    }
    fclose(fileptr);
}
bool canPromote() {
    if (!(input.init.X == 0 || input.init.Y == 0)) {
        if (bannmenn.turn == SENTE) {
            if (input.final.Y <= 3 || input.init.Y <= 3) return true;
        } else {
            if (input.final.Y >= 7 || input.init.Y >= 7) return true;
        }
        return false;
    }
    return false;
}
bool revert() {
    char temp[150] = "";
    pop(&kifuStack);
    strcpy(temp, peek(&kifuStack));
    // push(temp,&kifuStack);
    initializeBoard();
    // puts("SFEN: ");
    // puts(temp);
    if (!SFENParse(temp)) {
        renderBoard();
        return 0;
    } else {
        printf("parsing from stack failed \n");
        return 1;
    }
}
void initializeBoard() {
    for (int i = FU; i <= GYOKU; i++) {
        bannmenn.goteKomadai.komaList[i] = 0;
        bannmenn.senteKomadai.komaList[i] = 0;
    }
    bannmenn.moveNumber = 0;
    for (int i = 0; i <= 8; i++)
        for (int j = 0; j <= 8; j++) {
            bannmenn.pieces[i][j].promoted = 0;
            bannmenn.pieces[i][j].type = ' ';
        }
    bannmenn.turn = 0;
}
bool forcePromote(){
    int invalidRowLimit;
    if(strcmp(input.type, "kei") == 0)
        invalidRowLimit = 2;
    else if (strcmp(input.type, "kyo") == 0 ||strcmp(input.type, "fu") == 0 )
        invalidRowLimit = 1;
    else return false;
        if (bannmenn.turn == SENTE) {
            if (input.final.Y <= invalidRowLimit || input.init.Y <= invalidRowLimit) return true;
        } else {
            if (input.final.Y >= 10 - invalidRowLimit || input.init.Y >= 10 - invalidRowLimit ) return true;
        }
    return false;
    
}