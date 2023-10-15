#include "memo_scanner.h"


void init_g_info(GLOBAL_INFO *g_info){
    info_print(INFO, "running..", "init_g_info");
    info_print(INFO_NECESSARY, "INIT GLOBAL INFO", "init_g_info");

    printf("Enter process id: ");
    while (scanf("%d", &g_info->pid) != 1) {
        printf("Please enter a valid pid: ");
    }
    g_info->hProcess = open_process(g_info->pid);
    g_info->memory_list = memoli_init();
    g_info->mbi_list = scanf_subt_memo_addr(g_info->hProcess);
}


void scanf_memo_exat(GLOBAL_INFO *g_info, char type, char *value){
    info_print(INFO, "running..", "scanf_memo_exat");
    info_print(INFO_NECESSARY, "Scanf memory with exact value", 
               "scanf_memo_exat");
    
    // 将字符串转换为 特定类型的值
    void *value_ptr = str_to_multy_value(type, value);
    if (value_ptr == NULL) {
        error(1, "str_to_multy_value failed");
    }

    int num_of_result = 0;
    num_of_result = scanf_memo_exat_multy_type(g_info, type, value_ptr);
    printf("Found %d results.\n", num_of_result);
    free(value_ptr);
}


void filt_scanf_memo_exat(GLOBAL_INFO *g_info, char *value){
    info_print(INFO, "running..", "filt_scanf_memo_exat");
    info_print(INFO_NECESSARY, "Filter memory with exact value", 
               "filt_scanf_memo_exat");

    // 关于 扫描的类型, 我大致分为，精确扫描，和 模糊扫描
    // 精确扫描中 仅支持 单类型扫描
    // 模糊扫描中 还在考虑 
    void *value_ptr = 
        str_to_multy_value(g_info->memory_list->head->type, value);
    if (value_ptr == NULL) {
        error(1, "str_to_multy_value failed");
    }

    int res = filt_scanf_memo_exat_multy(g_info, value_ptr);
    printf("Found %d results.\n", res);
    free(value_ptr);
}