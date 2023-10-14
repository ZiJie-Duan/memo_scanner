#include "memo_scanner.h"

/* print info
 * 打印信息
 * 参数: code 信息优先级
 *       msg 信息内容
 *       source 信息来源
 * 返回值: 无
 * code 优先级，0为必须打印
 * msg 信息内容
*/
void info_print(int code, char *msg, char *source) {
    if (code <= INFO_PRINT_LEVEL) {
        printf("[%s] %s\n", source, msg);
    }
}

/* help 
* 打印帮助信息
*/
void help(void){
    printf("Memory Scanner v1.0 Help\n");
    printf(" h -------------------- help\n");
    printf(" op@[processId] ------- open process\n");
    printf(" cp ------------------- close process\n");
    printf(" sm4b ----------------- scan 4 byte in memory\n");
    printf(" sem4b@[addr]@[value] - set 4 byte in memory\n");
}