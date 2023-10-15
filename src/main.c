#include "memo_scanner.h"


void command_classifier(char (*command_array)[COMMAND_ARG_BUF_SIZE], 
        int arg_num, GLOBAL_INFO *g_info){
    
    if (strcmp(command_array[0], "h") == 0){
        help();

    } else if (strcmp(command_array[0], "sm") == 0){
        scanf_memo_exat(g_info, 
                        str_to_value_type_flag(command_array[1]), 
                        command_array[2]);

    } else if (strcmp(command_array[0], "mp") == 0){
        print_memolik_data(g_info->memory_list->head);

    } else if (strcmp(command_array[0], "fs") == 0){
        filt_scanf_memo_exat(g_info,
                             command_array[1]);
    }
    // } else if (strcmp(command_array[0], "sem") == 0){
    //     set_memo_4_byte(g_info->hProcess, 
    //                     (LPVOID)strtol(command_array[1], NULL, 16), 
    //                     atoi(command_array[2]));
    // }
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
        if (arg_num == 0) continue;
        command_classifier(command_array, arg_num, &g_info);
    }

}

