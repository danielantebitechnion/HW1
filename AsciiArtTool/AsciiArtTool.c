#include "AsciiArtTool.h"
#define NULL_CHAR '\0'
#define SPACE_CHAR ' '
#define AT_SIGN '@'

RLEList asciiArtRead (FILE *in_stream) {
    RLEList res = RLEListCreate();
    if (res == NULL) {
        printf("Malloc Error!\n");
        return 0;
    }
    char symbol;
    while ((symbol = fgetc(in_stream)) != EOF) {
        RLEListAppend(res, symbol);
    }
    return res;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream) {
    if (list->next != NULL){
        list = list->next;
    }
    int symbol;
    while (list != NULL) {
        if (list->symbol == NULL_CHAR) {
            return RLE_LIST_NULL_ARGUMENT;
        }
        symbol = list->symbol;
        for (int i=0 ; i<list->repetitions ; i++) {
            fputc(symbol,out_stream);
        }
        list=list->next;
    }
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded (RLEList list, FILE *out_stream) {
    char *zipped_rlelist = RLEListExportToString(list, 0);
    if (zipped_rlelist == NULL) {
        printf("Malloc Error!\n");
        return RLE_LIST_OUT_OF_MEMORY;
    }
    if (*zipped_rlelist == RLE_LIST_NULL_ARGUMENT) {
        return RLE_LIST_NULL_ARGUMENT;
    }
    fprintf(out_stream, "%s", zipped_rlelist);
    free(zipped_rlelist);
    return RLE_LIST_SUCCESS;
}

static char InvertMapAsciiArt(char c){
    if(c == SPACE_CHAR){
        return AT_SIGN;
    }
    else if(c == AT_SIGN){
        return SPACE_CHAR;
    }
    return c;
}
