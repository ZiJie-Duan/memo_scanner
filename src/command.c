#include "memo_scanner.h"


int line_split(char *line, char (*command_array)[COMMAND_ARG_BUF_SIZE]){

    int index = 0;
    int last_line_index = 0;

    for (int i=0; i<COMMAND_LINE_BUF_SIZE; i++){
        // 这里，使用 \0 和 strcpy 分割字符串
        // 但是最后一个 \0 是由 get_input 函数添加的
        if (line[i] == COMMAND_SPLITER){
            line[i] = '\0';
            strcpy(command_array[index], &line[last_line_index]);
            last_line_index = i+1;
            index ++;
        }
    }
    return index;
}

int get_input(char *line){

    char temp_char;
    int count = 0;

    while ((temp_char=getchar()) != '\n'){
        *line = temp_char;
        line ++;
        count ++;
    }
    *line = '\0';

    return count;
}

void set_zero(char *line, char (*command_array)[COMMAND_ARG_BUF_SIZE]){

    for (int i=0; i<COMMAND_LINE_BUF_SIZE; i++){
        line[i] = COMMAND_EMPTY;
    }
    for (int i=0; i<COMMAND_ARG_NUM; i++){
        for (int j=0; j<COMMAND_ARG_BUF_SIZE; j++){
            command_array[i][j] = COMMAND_EMPTY;
        }
    }
}