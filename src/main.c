#include "memo_scanner.h"


void command_classifier(char (*command_array)[COMMAND_ARG_BUF_SIZE], 
        int arg_num, GLOBAL_INFO *g_info){

    if (strcmp(command_array[0], "h") == 0){
        help();
    }
    // } else if (strcmp(command_array[0], "op") == 0){
    //     *hProcess = open_process(atoi(command_array[1]));
    // } else if (strcmp(command_array[0], "cp") == 0) {
    //     close_process(*hProcess);
    // } else if (strcmp(command_array[0], "sm4b") == 0){
    //     scanf_memo_4_byte_core(*hProcess);
    // } else if (strcmp(command_array[0], "sem4b") == 0){
    //     char *end;
    //     uintptr_t addr_int = strtoul(command_array[1], &end, 16);  // 基数设为16表示16进制
    //     LPVOID addr = (LPVOID)addr_int;  // 类型转换
    //     int value = atoi(command_array[2]);  // 将字符串转换为整数
    //     set_memo_4_byte(*hProcess, addr, value);  // 调用函数
    // }
    // } else if (strcmp(command_array[0], "h") == 0){
}


int main(){
    printf("Memory Scanner v2.0\n");
    printf("Type 'help' for more information.\n\n");
    if (SetDebugPrivilege()) {
        printf("Successfully obtained debug privilege.\n");
    } else {
        printf("Failed to obtain debug privilege.\n");
    }

    GLOBAL_INFO g_info;
    init_g_info(&g_info); // 初始化全局信息

    char line[COMMAND_LINE_BUF_SIZE];
    char command_array[COMMAND_ARG_NUM][COMMAND_ARG_BUF_SIZE];
    int arg_num;
    int line_lenth;

    while (1){
        set_zero(line, command_array);
        printf(">>");
        line_lenth = get_input(line);
        arg_num = line_split(line, command_array);
        command_classifier(command_array, arg_num, &g_info);
    }


}

