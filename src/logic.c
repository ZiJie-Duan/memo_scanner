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


// void scanf_memo_4_byte_core(HANDLE hProcess){
//     info_print(INFO, "running..", "scanf_memo_4_byte_core");
//     info_print(INFO_NECESSARY, "Scanf memory with 4 byte INT", 
//                "scanf_memo_4_byte_core");

//     int value;
//     char msg[128]; // 打印缓冲区
    
//     MBIMS mbim;
//     int mbim_len;

//     MEMO_NODE_4_BYTE *head = NULL;

//     scanf_subt_memo_addr(hProcess, mbim, &mbim_len); 

//     sprintf(msg, "find %d writable memory region", mbim_len);
//     info_print(INFO_NECESSARY, msg, "scanf_memo_4_byte_core");

//     printf("Scan Value (int 4byte): ");
//     scanf("%d", &value);
//     int resnum = scanf_memo_4_byte(hProcess, mbim, mbim_len, value, &head);
//     printf("test finished1 \n");//``````````````````````````````````````````````````
//     for (MEMO_NODE_4_BYTE *p = head; p != NULL; p = p->next) {
//         printf("addr: %p  | ", p->addr);
//         printf("value: %d\n", p->value);
//     }
//     printf("find %d result\n", resnum);

//     while (1) {
//         printf("Enter 1 to filt, 0 to exit, 2 to save: ");
//         scanf("%d", &value);
//         if (value == 0) {
//             break;
//         } else if (value == 1) {
//             printf("Filt Scan Value (int 4byte): ");
//             scanf("%d", &value);
//             resnum = filt_scanf_memo_4_byte(hProcess, &head, value);
//             printf("resnum: %d\n", resnum);
//             for (MEMO_NODE_4_BYTE *p = head; p != NULL; p = p->next) {
//                 printf("addr: %p  | ", p->addr);
//                 printf("value: %d\n", p->value);
//             }
//         } else if (value == 2) {
//             break;
//         }
//     }
// }