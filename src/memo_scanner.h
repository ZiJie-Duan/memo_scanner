#ifndef MEMO_SCANNER_H
#define MEMO_SCANNER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ERROR_SERIOUS 1
#define ERROR_SIMPLE 0

#define INFO_PRINT_LEVEL 3
#define INFO_NECESSARY 0   // 必要信息
#define INFO_IMPORTANT 1   // 未定义
#define INFO         2     // 普通信息
#define INFO_DEBUG 3       // 调试信息

#define MBIMS_LENTH 10000 // MEMORY_BASIC_INFORMATION_MINI_STRUCTS_LENTH
#define SCANNER_BUF_SIZE 4096
#define SCANNER_BUF_REMAIN 64

#define COMMAND_LINE_BUF_SIZE 1280
#define COMMAND_ARG_BUF_SIZE 128
#define COMMAND_ARG_NUM 10

#define COMMAND_SPLITER '@'
#define COMMAND_EMPTY '#'

#define INT_8_TYPE 0         // char, signed char
#define INT_16_TYPE 1        // short, signed short
#define INT_32_TYPE 2        // int, signed int
#define INT_64_TYPE 3        // long long, signed long long
#define UINT_8_TYPE 4        // unsigned char
#define UINT_16_TYPE 5       // unsigned short
#define UINT_32_TYPE 6       // unsigned int
#define UINT_64_TYPE 7       // unsigned long long
#define FLOAT_TYPE 8         // float
#define DOUBLE_TYPE 9        // double
#define LONG_DOUBLE_TYPE 10  // long double

#define BIGGER_THAN 1
#define SMALLER_THAN 2
#define EQUAL 0

typedef struct _MEMORY_BASIC_INFORMATION_NODE{
    PVOID  BaseAddress;
    SIZE_T RegionSize;
    struct _MEMORY_BASIC_INFORMATION_NODE *next;
} MBI_NODE;

typedef struct _MEMORY_BASIC_INFORMATION_LIST{
    MBI_NODE * head;
    MBI_NODE * tail;
} MBI_LIST;

typedef struct _VALUE_BLOCK{
    void *value;
    int time;
    struct _VALUE_BLOCK *next;  // 指向下一个值, 头节点永远为最新值
} VALUE_BLOCK;

typedef struct _MEMORY_BLOCK{
    int id;
    char type;
    VALUE_BLOCK *values;
    LPVOID addr;
    struct _MEMORY_BLOCK *next;
} MEMORY_BLOCK;

typedef struct _MEMORY_LIST{
    MEMORY_BLOCK * head;
    MEMORY_BLOCK * tail;
} MEMORY_LIST;

typedef struct _GLOBAL_INFO{
    int pid;
    HANDLE hProcess;
    MBI_LIST *mbi_list; 
    MEMORY_LIST *memory_list;
} GLOBAL_INFO;

void info_print(int code, char *msg, char *source);
void printMemoryBasicInformation(MEMORY_BASIC_INFORMATION mbi);
int error(int mode, char *msg);
HANDLE open_process(DWORD processId);
MBI_LIST * scanf_subt_memo_addr(HANDLE hProcess);
void close_process(HANDLE hProcess);
// void fill_zero_k(HANDLE hProcess, MEMORY_BASIC_INFORMATION_MINI *mbim,
//                  int mbim_len);

int scanf_memo_4_byte(GLOBAL_INFO *g_info, int value);
int filt_scanf_memo_4_byte(GLOBAL_INFO *g_info, int value);

int line_split(char *line, char (*command_array)[COMMAND_ARG_BUF_SIZE]);
int get_input(char *line);
void set_zero(char *line, char (*command_array)[COMMAND_ARG_BUF_SIZE]);

void command_classifier(char (*command_array)[COMMAND_ARG_BUF_SIZE], 
        int arg_num, GLOBAL_INFO *g_info);

void scanf_memo_4_byte_core(HANDLE hProcess);
void set_memo_4_byte(HANDLE hProcess, LPVOID addr, int value);

void help(void);





void init_g_info(GLOBAL_INFO *g_info);
MBI_LIST *mbi_list_init(void);
void mbi_list_add(MBI_LIST *mbi_list, PVOID addr, SIZE_T size);
void free_mbi_list(MBI_LIST *mbi_list);
MEMORY_LIST *memoli_init(void);
void memoli_add_block(MEMORY_LIST *memoli, int id,
                      char type, LPVOID addr, void *value);
void free_memoli(MEMORY_LIST *memoli);

VALUE_BLOCK *add_valueblk_multy(void *value, char type);
int8_t * memoblk_v_int_8(int8_t *value);
int16_t * memoblk_v_int_16(int16_t *value);
int32_t * memoblk_v_int_32(int32_t *value);
int64_t * memoblk_v_int_64(int64_t *value);
uint8_t * memoblk_v_uint_8(uint8_t *value);
uint16_t * memoblk_v_uint_16(uint16_t *value);
uint32_t * memoblk_v_uint_32(uint32_t *value);
uint64_t * memoblk_v_uint_64(uint64_t *value);
float * memoblk_v_float(float *value);
double * memoblk_v_double(double *value);
long double * memoblk_v_long_double(long double *value);
int sizeof_multy_type(char type);
int value_cmp_multy(char type, void * v1, void * v2);


BOOL SetDebugPrivilege();

typedef struct _MEMO_NODE_4_BYTE{
    PVOID addr;
    int value;
    struct _MEMO_NODE_4_BYTE *prev;
    struct _MEMO_NODE_4_BYTE *next;
} MEMO_NODE_4_BYTE;



#endif // MEMO_SCANNER_H
