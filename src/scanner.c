
#include "memo_scanner.h"


/* printMemoryBasicInformation
 * 打印内存信息
 * 参数: mbi 内存信息结构体
 * 返回值: 无
*/
void printMemoryBasicInformation(MEMORY_BASIC_INFORMATION mbi) {
    printf("\n=== MEMORY_BASIC_INFORMATION ===\n");

    printf("BaseAddress:        %p\n", mbi.BaseAddress);
    printf("AllocationBase:     %p\n", mbi.AllocationBase);
    printf("AllocationProtect:  0x%X\n", mbi.AllocationProtect);
    printf("RegionSize:         %zu bytes\n", mbi.RegionSize);

    // 解析 State 字段
    printf("State:              0x%X (", mbi.State);
    if (mbi.State == MEM_COMMIT)  printf("MEM_COMMIT");
    if (mbi.State == MEM_FREE)    printf("MEM_FREE");
    if (mbi.State == MEM_RESERVE) printf("MEM_RESERVE");
    printf(")\n");

    // 解析 Protect 字段
    printf("Protect:            0x%X (", mbi.Protect);
    if (mbi.Protect & PAGE_READONLY)  printf("PAGE_READONLY ");
    if (mbi.Protect & PAGE_READWRITE) printf("PAGE_READWRITE ");
    if (mbi.Protect & PAGE_EXECUTE)   printf("PAGE_EXECUTE ");
    printf(")\n");

    // 解析 Type 字段
    printf("Type:               0x%X (", mbi.Type);
    if (mbi.Type == MEM_IMAGE)   printf("MEM_IMAGE");
    if (mbi.Type == MEM_MAPPED)  printf("MEM_MAPPED");
    if (mbi.Type == MEM_PRIVATE) printf("MEM_PRIVATE");
    printf(")\n");

    printf("================================\n");
}


MBI_LIST * scanf_subt_memo_addr(HANDLE hProcess){
    info_print(INFO, "running..", "scanf_subt_memo_addr");
    MBI_LIST * mbi_list = mbi_list_init();

    MEMORY_BASIC_INFORMATION mbi;
    // 用于存储内存信息
    LPCVOID lpAddress = 0;  // 用于存储内存地址
    char message[128];  // 打印缓冲区

    // 循环扫描内存地址
    while ((VirtualQueryEx(hProcess, (LPCVOID)lpAddress, 
                &mbi, sizeof(mbi)) != 0)) {

        // 如果内存状态为已分配，且保护属性为可读写，则记录该内存地址
        if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE) {

            // debug info
            sprintf(message, "find a writable memory region: %p",
                    mbi.BaseAddress);
            info_print(INFO_DEBUG, message, "scanf_subt_memo_addr");
            
            // add node to mbi_list 赋值给内存信息链表，拷贝赋值
            mbi_list_add(mbi_list, mbi.BaseAddress, mbi.RegionSize); 
        }
        lpAddress += mbi.RegionSize;
    }
    return mbi_list;
}


int scanf_memo_exat_multy_type(GLOBAL_INFO *g_info, char type, void *value){

    SIZE_T bytesRead; // ReadProcessMemory 读取的字节数
    PVOID address;  // 读取的内存地址
    char message[128];  // 打印缓冲区
    int num_of_result = 0;
    
    // mbin 为内存信息链表节点, 用于提供内存块的地址和大小
    for (MBI_NODE * mbin = g_info->mbi_list->head; 
         mbin != NULL; mbin = mbin->next){

        address = mbin->BaseAddress;
        BYTE *buffer = malloc(mbin->RegionSize);  // 缓冲区

        // 如果读取失败, 则跳过该内存块
        if (ReadProcessMemory(g_info->hProcess, (LPCVOID)address, 
                                buffer, mbin->RegionSize, &bytesRead) == 0){
            error(ERROR_SIMPLE, "ReadProcessMemory ERROR");
            free(buffer);
            continue;
        }

        // 针对不同的数据类型预留 扫描安全空间，防止越界
        int end_safe_len = mbin->RegionSize - size_map[type] + 1;
        void *data_p;  //一个通用类型的指针，用来扫描数据

        // 遍历内存所有数据
        for (int i=0; i < end_safe_len; i++){

            data_p = (void *)&buffer[i]; //指向数据
            // 比较数据 如果数据相等 则记录该内存块
            if (value_cmp_multy(type, data_p, value) == EQUAL){
                // 加入记忆链表
                memoli_add_block(g_info->memory_list, 
                                 num_of_result,
                                 type,
                                 (PVOID)((char*)address+i),
                                 value);
                num_of_result++;
            }
        }
        free(buffer);
    }
    return num_of_result;
}


int filt_scanf_memo_exat_multy(GLOBAL_INFO *g_info, void *value){
    info_print(INFO, "running..", "filt_scanf_memo_4_byte");

    int8_t buffer[8]; //8*8=64位, 用于存储读取的内存数据

    MEMORY_BLOCK * root = malloc(sizeof(MEMORY_BLOCK));
    root->next = g_info->memory_list->head;
    MEMORY_BLOCK * pre = root;

    int count = 0;

    for (MEMORY_BLOCK *mbk = root->next; mbk != NULL; ){

        if (ReadProcessMemory(g_info->hProcess, (LPCVOID)mbk->addr, 
                              &buffer, size_map[mbk->type], NULL) == 0){
            // 错误处理
            error(ERROR_SIMPLE, "ReadProcessMemory ERROR");
            pre = mbk;
            mbk = mbk->next;
            continue;
        }

        int res = value_cmp_multy(mbk->type, buffer, value);

        if (res != EQUAL){
            // 如果不相等, 则删除该节点
            pre->next = mbk->next;
            MEMORY_BLOCK *temp = mbk;
            mbk = mbk->next;
            free_memoli_node(temp);

        } else if (res == EQUAL){
            // 如果相等, 则继续遍历
            memoblk_update_value(mbk, mbk->type, value);
            pre = mbk;
            mbk = mbk->next;
            count++;
        }
    }
    g_info->memory_list->head = root->next;
    return count;
}


void set_memo_4_byte(HANDLE hProcess, LPVOID addr, int value){
    info_print(INFO, "running..", "set_memo_4_byte");
    char message[128];  // 打印缓冲区

    printf("set %p to %d\n", addr, value);

    if (WriteProcessMemory(hProcess, addr, &value, sizeof(int), NULL) == 0){
        error(ERROR_SIMPLE, "WriteProcessMemory ERROR");
    } else {
        sprintf(message, "set %p to %d", addr, value);
        info_print(INFO_NECESSARY, message, "set_memo_4_byte");
    }
}

//     close_process(hProcess);
//     // fill_zero_k(hProcess, mbim, mbim_len);
//     // close_process(hProcess);
//     // printf("done\n");
//     // getchar();
//     // getchar();
//     return 0;
// }


// void fill_zero_k(HANDLE hProcess, MEMORY_BASIC_INFORMATION_MINI *mbim, 
//                int mbim_len){
//     info_print(INFO, "running..", "fill_zero_k");

//     SIZE_T bytes_remain; // 未读取的字节数
//     PVOID address;  // 读取的内存地址
//     BYTE value = 0;
//     SIZE_T bytesWritten;
//     char message[128];  // 打印缓冲区

//     for (int i=0; i<mbim_len; i++){
//         sprintf(message, "filling region.. addr: %p  size: %d", 
//                 mbim[i].BaseAddress, mbim[i].RegionSize);
//         info_print(INFO_DEBUG, message, "fill_zero_k");

//         bytes_remain = mbim[i].RegionSize;
//         address = mbim[i].BaseAddress;

//         while(bytes_remain > 0){
//             sprintf(message, "filling.. addr: %p", address);
//             info_print(INFO_DEBUG, message, "fill_zero_k");
            
//             WriteProcessMemory(hProcess, (LPVOID)address, 
//                                &value, sizeof(BYTE), &bytesWritten);
//             bytes_remain -= bytesWritten;
//             address += bytesWritten;
//         }
//     }
// }

