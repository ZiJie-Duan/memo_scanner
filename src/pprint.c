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
    printf("Memory Scanner v2.0 Help\n");
    printf(" h ------------------------- help\n");
    printf(" q ------------------------- quit\n");
    printf(" sm@[type]@[value]@--------- scan value with type\n");
    printf(" fs@[value]@---------------- filt scan value\n");
}


void print_memolik_data(MEMORY_BLOCK *head){
    info_print(INFO, "running..", "print_memolik_data");
    info_print(INFO_NECESSARY, "PRINT MEMOLIK DATA", "print_memolik_data");

    char *type_str;
    printf("\n");
    printf("+------------------+-----+--------------+--------------+--------------+\n");
    printf("|       addr       |type |  value now   |  value prev  | value first  |\n");
    printf("+------------------+-----+--------------+--------------+--------------+\n");
    for (MEMORY_BLOCK *p = head; p != NULL; p = p->next){
        type_str = value_type_flag_to_str(p->type);
        printf("| %16p | %3s | ", p->addr, type_str);
        print_value_multy_type(p->type, p->values->value);
        printf(" | ");
        if (p->values->next == NULL) {
            printf("        N/A ");
        } else {
            print_value_multy_type(p->type, p->values->next->value);
        }
        printf(" | ");
        VALUE_BLOCK *q;
        for (q = p->values; q->next != NULL; q = q->next) continue;
        print_value_multy_type(p->type, q->value);
        printf(" |\n");
    }
    printf("+------------------+-----+--------------+--------------+--------------+\n");
    printf("\n");
}

void print_value_multy_type(char type, void *value){
    switch (type){
        case INT_8_TYPE:
            printf("%12d", *(char *)value);
            break;
        case INT_16_TYPE:
            printf("%12d", *(short *)value);
            break;
        case INT_32_TYPE:
            printf("%12d", *(int *)value);
            break;
        case INT_64_TYPE:
            printf("%12lld", *(long long *)value);
            break;
        case UINT_8_TYPE:
            printf("%12u", *(unsigned char *)value);
            break;
        case UINT_16_TYPE:
            printf("%12u", *(unsigned short *)value);
            break;
        case UINT_32_TYPE:
            printf("%12u", *(unsigned int *)value);
            break;
        case UINT_64_TYPE:
            printf("%12llu", *(unsigned long long *)value);
            break;
        case FLOAT_TYPE:
            printf("%12.6f", *(float *)value);
            break;
        case DOUBLE_TYPE:
            printf("%12.6lf", *(double *)value);
            break;
        case LONG_DOUBLE_TYPE:
            printf("%12.6Lf", *(long double *)value);
            break;
    }
}