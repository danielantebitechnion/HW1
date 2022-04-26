#include "RLEList.h"


#define NULL_CHAR '\0'
#define INVALID -1

RLEList RLEListCreate(){
    RLEList ptr = malloc(sizeof(*ptr));
    if(!ptr){
        return NULL;
    }
    ptr->symbol = NULL_CHAR;// '\0'
    ptr->repetitions = 0;
    ptr->next = NULL;
    return ptr;
}

void RLEListDestroy (RLEList list) {
    while (list != NULL) {
        RLEList to_delete = list;
        list = list->next;
        free(to_delete);
    }
}

RLEListResult RLEListAppend(RLEList list, char value){
    if(value==NULL_CHAR || list==NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list->next != NULL){
        list = list->next;
    }
    if(list->symbol==value){
        list->repetitions++;
    }
    else{
        RLEList new_node = RLEListCreate();
        if(new_node==NULL){
            return RLE_LIST_OUT_OF_MEMORY;
        }
        else{
            new_node->symbol = value;
            new_node->repetitions = 1;
            new_node->next = NULL;
        }
        list->next = new_node;
        return RLE_LIST_SUCCESS;
    }
}

int RLEListSize (RLEList list){
    if(list==NULL){
        return INVALID;//-1
    }
    int num_of_characters = 0;
    if(list->next!=NULL){
        list = list->next;
        while (list != NULL) {
            num_of_characters+=list->repetitions;
            list = list->next;
        }
    }
    return num_of_characters;
}

RLEListResult RLEListRemove(RLEList list, int index){
    if(list==NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index > RLEListSize(list)-1){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    if(list->next==NULL){//empty list
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    //else
    RLEList index_node = RLEListCreate();
    if(index_node == NULL){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    index_node = list->next;
    while(index!=0){
        if(index >= index_node->repetitions){
            index-=index_node->repetitions;
            index_node = index_node->next;
            list = list->next;
        }
        if(index < index_node->repetitions){
            index = 0 ;
        }
    }
    if(index_node->repetitions == 1){
        list->next = index_node->next;
        if(list->next != NULL){
            if(list->symbol == index_node->symbol){
                RLEList merge_node = RLEListCreate();
                if(merge_node==NULL){
                    return RLE_LIST_OUT_OF_MEMORY;
                }
                merge_node = list->next;
                list->repetitions+=merge_node->repetitions;
                list->next = merge_node->next;
                free(merge_node);
            }
            free(index_node);
        }
    }
    else{//index_node->repetitions > 1
        index_node->repetitions--;
    }
    return RLE_LIST_SUCCESS;
}

char RLEListGet (RLEList list, int index, RLEListResult *result) {
    if (list == NULL) {
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if (index < 0 || index > (RLEListSize(list) - 1)) {
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    if(list->next==NULL){//empty list
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    int next_counter = 0;
    while (next_counter <= index) {
        list = list->next;
        next_counter+=list->repetitions;
    }
    result = RLE_LIST_SUCCESS;
    return list->symbol;
}

typedef char (*MapFunction)(char);
RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    if(list==NULL || list->next == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list!=NULL){
        list->symbol = map_function(list->symbol);
        list = list->next;
    }
    return RLE_LIST_SUCCESS;
}

static int digits (int num) {//return the amount of digits
    int counter =0;
    while(num!=0){
        counter++;
        num=num/10;
    }
    return counter;
}

static char *int_to_str (int num) {//convert int to string
    int num_of_digits = digits(num);
    char *res = malloc((num_of_digits + 1) * sizeof(*res)); // +1 for the \0
    if (res == NULL) {
        printf("Malloc Error!\n");
        return NULL_CHAR;
    }
    for(int i=num_of_digits;i>0;i--){
        res[i-1]= '0' + num%10;
        num=num/10;
    }
    res[num_of_digits] = NULL_CHAR;
    return res;
}

char *RLEListExportToString (RLEList list, RLEListResult *result) {
    if (list == NULL) {
        *result = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }
    RLEList list_copy = RLEListCreate();
    if (list_copy == NULL) {
        printf("Malloc Error!\n");
        return NULL_CHAR;
    }
    else {
        list_copy = list;
    }
    list_copy = list_copy->next;
    int res_size = 0;
    while (list_copy != NULL) {
        res_size++; // for the symbol
        res_size += digits(list_copy->repetitions);
        res_size++; // for the \n
        list_copy = list_copy->next;
    }
    char *res = malloc((res_size + 1) * sizeof(*res)); // +1 for the \0
    if (res == NULL) {
        printf("Malloc Error!\n");
        return NULL_CHAR;
    }
    for (int k=0 ; k<=res_size ; k++) { // initialization
        res[k] = NULL_CHAR;
    }
    int i = 0;
    list = list->next;
    while (list != NULL) {
        char *repetetions_str = int_to_str(list->repetitions);
        res[i] = list->symbol;
        i++;
        for (int j=0 ; repetetions_str[j] != NULL_CHAR ; j++) {
            res[i] = repetetions_str[j];
            i++;
        }
        res[i] = '\n';
        i++;
        free(repetetions_str);
        list = list->next;
    }
    if (res != NULL_CHAR) {
        result = RLE_LIST_SUCCESS;
        return res; // res is of type str, meaning char pointer
    }
    return NULL_CHAR;
}
