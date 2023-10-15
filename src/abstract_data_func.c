#include "memo_scanner.h"

/* mbi_list_init
 * malloc MBI_LIST 结构体链表(头尾)，将head 和 tail 初始化为 NULL
 * 参数: 无
 * 返回值: 内存信息链表
 */
MBI_LIST *mbi_list_init(void){
    MBI_LIST *mbi_list = malloc(sizeof(MBI_LIST));
    if (mbi_list == NULL) {
        error(1, "malloc failed");
    }

    mbi_list->head = NULL;
    mbi_list->tail = NULL;
    return mbi_list;
}

/* mbi_list_add
 * malloc MBI_NODE 结构体节点，将其加入内存信息链表
 * 参数: mbi_list 内存信息链表(头尾)
 *       addr 内存地址
 *       size 内存大小
 * 返回值: 无
 */
void mbi_list_add(MBI_LIST *mbi_list, PVOID addr, SIZE_T size){
    MBI_NODE *node = malloc(sizeof(MBI_NODE));
    if (node == NULL) {
        error(1, "malloc failed");
    }

    node->BaseAddress = addr;
    node->RegionSize = size;
    node->next = NULL;

    if (mbi_list->head == NULL) {
        mbi_list->head = node;
        mbi_list->tail = node;
    } else {
        mbi_list->tail->next = node;
        mbi_list->tail = node;
    }
}

/* free_mbi_list
 * 释放内存信息链表(头尾)
 * 参数: mbi_list 内存信息链表(头尾)
 * 返回值: 无
 */
void free_mbi_list(MBI_LIST *mbi_list){
    MBI_NODE *p = mbi_list->head;
    MBI_NODE *q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    free(mbi_list);
}

/* memoli_init
 * malloc MEMORY_LIST 结构体链表(头尾)，将head 和 tail 初始化为 NULL
 * 参数: 无
 * 返回值: 记忆链表
 */
MEMORY_LIST *memoli_init(void){
    MEMORY_LIST *memoli = malloc(sizeof(MEMORY_LIST));
    if (memoli == NULL) {
        error(1, "malloc failed");
    }

    memoli->head = NULL;
    memoli->tail = NULL;
    return memoli;
}

/* free_memoli
 * 释放记忆链表(头尾) 以及 记忆链表中的值链表
 * 参数: memoli 记忆链表(头尾)
 * 返回值: 无
 */
void free_memoli(MEMORY_LIST *memoli){
    MEMORY_BLOCK *p = memoli->head;
    MEMORY_BLOCK *q;
    VALUE_BLOCK *r;
    VALUE_BLOCK *s;
    while (p != NULL) {
        q = p->next;
        // free values first
        r = p->values;
        while (r != NULL) {
            s = r->next;
            free(r);
            r = s;
        }
        free(p);
        p = q;
    }
    free(memoli);
}

void free_memoli_node(MEMORY_BLOCK *memoblk){
    VALUE_BLOCK *r;
    VALUE_BLOCK *s;
    // free values first
    r = memoblk->values;
    while (r != NULL) {
        s = r->next;
        free(r);
        r = s;
    }
    free(memoblk);
}

/* memoli_add_block
 * 当我们添加一个信息块到memory_list时，我们需要附加传入一个值
 * 这个值 用于添加一个 value_block 到 values 链表 来存储初始值，值是复制的
 * malloc 一个 MEMORY_BLOCK 结构体节点，将其加入记忆链表
 * malloc 一个 VALUE_BLOCK 结构体节点，将其加入值链表
 * malloc 一个 value 内存
 * 参数: memoli 记忆链表(头尾)
 *       id 记忆块id
 *       type 记忆块数据类型
 *       addr 记忆块地址
 *       value 记忆块的值
 * 返回值: 无
 */
void memoli_add_block(MEMORY_LIST *memoli, int id, char type, LPVOID addr, 
        void *value){

    MEMORY_BLOCK *node = malloc(sizeof(MEMORY_BLOCK));
    if (node == NULL) {
        error(1, "malloc failed");
    }
    node->addr = addr;
    node->id = id;
    node->type = type;
    // malloc value block and add it to values
    node->values = add_valueblk_multy(value, type);
    node->next = NULL;

    if (memoli->head == NULL) {
        memoli->head = node;
        memoli->tail = node;
    } else {
        memoli->tail->next = node;
        memoli->tail = node;
    }
}

/* memoblk_update_value
 * 当我们更新一个信息块的值时，我们需要附加传入一个值
 * 这个值 用于添加一个 value_block 到 values 链表，值是复制的
 * malloc 一个 VALUE_BLOCK 结构体节点，将其加入值链表
 * malloc 一个 value 内存
 * 参数: memoli 记忆链表(头尾)
 *       type 记忆块数据类型
 *       value 记忆块的值
 * 返回值: 无
 */
void memoblk_update_value(MEMORY_BLOCK *memoblk, char type, void * value){
    VALUE_BLOCK *last_v = memoblk->values;
    memoblk->values = add_valueblk_multy(value, type);
    memoblk->values->next = last_v;
}

/* add_valueblk_multy
 * malloc 一个 VALUE_BLOCK 结构体节点，将其加入值链表
 * malloc 一个 value 内存，value_blk->value 指向该内存
 * 参数: value 记忆块的值
 *       type 记忆块数据类型
 * 返回值: 无
 */
VALUE_BLOCK *add_valueblk_multy(void *value, char type){
    VALUE_BLOCK * value_blk = malloc(sizeof(VALUE_BLOCK));
    if (value_blk == NULL) error(1, "malloc failed");
    value_blk->next = NULL;
    value_blk->time = 0;
    value_blk->value = malloc_cpy_value_multy(type, value);
    return value_blk;
}

/* value_cmp_multy
* 比较两个值的大小
* 参数: type 数据类型
*       v1 指向第一个值的指针
*       v2 指向第二个值的指针
* 返回值: BIGGER_THAN, SMALLER_THAN, EQUAL
*/
int value_cmp_multy(char type, void * v1, void * v2){
    compare_func cmpfun = multy_cmp_func_map[type];
    return cmpfun(v1, v2);
}

/* malloc_cpy_value_multy
* malloc 一个 value 内存, 并将value的值复制到该内存
* 参数: type 数据类型
*       value 指向值的指针
* 返回值: 指向新malloc的内存的指针
*/
void * malloc_cpy_value_multy(char type, void *value){
    void *p = malloc(size_map[type]);
    if (p == NULL) {
        error(1, "malloc failed");
    }
    memcpy(p, value, size_map[type]); // 使用memcpy进行深复制
    return p;
}


char str_to_value_type_flag(char *str){
    if (strcmp(str, "1b") == 0) return INT_8_TYPE;
    if (strcmp(str, "u1b") == 0) return UINT_8_TYPE;
    if (strcmp(str, "2b") == 0) return INT_16_TYPE;
    if (strcmp(str, "u2b") == 0) return UINT_16_TYPE;
    if (strcmp(str, "4b") == 0) return INT_32_TYPE;
    if (strcmp(str, "u4b") == 0) return UINT_32_TYPE;
    if (strcmp(str, "8b") == 0) return INT_64_TYPE;
    if (strcmp(str, "u8b") == 0) return UINT_64_TYPE;
    if (strcmp(str, "f") == 0) return FLOAT_TYPE;
    if (strcmp(str, "d") == 0) return DOUBLE_TYPE;
    if (strcmp(str, "ld") == 0) return LONG_DOUBLE_TYPE;
}

char *value_type_flag_to_str(char type){
    char *str = malloc(4);
    if (type == INT_8_TYPE) strcpy(str, "1b");
    if (type == UINT_8_TYPE) strcpy(str, "u1b");
    if (type == INT_16_TYPE) strcpy(str, "2b"); 
    if (type == UINT_16_TYPE) strcpy(str, "u2b");
    if (type == INT_32_TYPE) strcpy(str, "4b");
    if (type == UINT_32_TYPE) strcpy(str, "u4b");
    if (type == INT_64_TYPE) strcpy(str, "8b");
    if (type == UINT_64_TYPE) strcpy(str, "u8b");
    if (type == FLOAT_TYPE) strcpy(str, "f");
    if (type == DOUBLE_TYPE) strcpy(str, "d");
    if (type == LONG_DOUBLE_TYPE) strcpy(str, "ld");
    return str;
}

void *str_to_multy_value(char type, char * str){
    if (type == INT_8_TYPE){
        int8_t *p = malloc(sizeof(int8_t));
        *p = (int8_t)atoi(str);
        return p;
    } else if (type == INT_16_TYPE){
        int16_t *p = malloc(sizeof(int16_t));
        *p = (int16_t)atoi(str);
        return p;
    } else if (type == INT_32_TYPE){
        int32_t *p = malloc(sizeof(int32_t));
        *p = (int32_t)atoi(str);
        return p;
    } else if (type == INT_64_TYPE){
        int64_t *p = malloc(sizeof(int64_t));
        *p = (int64_t)atoi(str);
        return p;
    } else if (type == UINT_8_TYPE){
        uint8_t *p = malloc(sizeof(uint8_t));
        *p = (uint8_t)atoi(str);
        return p;
    } else if (type == UINT_16_TYPE){
        uint16_t *p = malloc(sizeof(uint16_t));
        *p = (uint16_t)atoi(str);
        return p;
    } else if (type == UINT_32_TYPE){
        uint32_t *p = malloc(sizeof(uint32_t));
        *p = (uint32_t)atoi(str);
        return p;
    } else if (type == UINT_64_TYPE){
        uint64_t *p = malloc(sizeof(uint64_t));
        *p = (uint64_t)atoi(str);
        return p;
    } else if (type == FLOAT_TYPE){
        float *p = malloc(sizeof(float));
        *p = (float)atof(str);
        return p;
    } else if (type == DOUBLE_TYPE){
        double *p = malloc(sizeof(double));
        *p = (double)atof(str);
        return p;
    } else if (type == LONG_DOUBLE_TYPE){
        long double *p = malloc(sizeof(long double));
        *p = (long double)atof(str);
        return p;
    }
}

DEFINE_COMPARE_FUNC(int8_t, value_cmp_int8_t)
DEFINE_COMPARE_FUNC(int16_t, value_cmp_int16_t)
DEFINE_COMPARE_FUNC(int32_t, value_cmp_int32_t)
DEFINE_COMPARE_FUNC(int64_t, value_cmp_int64_t)
DEFINE_COMPARE_FUNC(uint8_t, value_cmp_uint8_t)
DEFINE_COMPARE_FUNC(uint16_t, value_cmp_uint16_t)
DEFINE_COMPARE_FUNC(uint32_t, value_cmp_uint32_t)
DEFINE_COMPARE_FUNC(uint64_t, value_cmp_uint64_t)
DEFINE_COMPARE_FUNC(float, value_cmp_float)
DEFINE_COMPARE_FUNC(double, value_cmp_double)
DEFINE_COMPARE_FUNC(long double, value_cmp_long_double)

compare_func multy_cmp_func_map[11] = {
    (compare_func)value_cmp_int8_t,
    (compare_func)value_cmp_int16_t,
    (compare_func)value_cmp_int32_t,
    (compare_func)value_cmp_int64_t,
    (compare_func)value_cmp_uint8_t,
    (compare_func)value_cmp_uint16_t,
    (compare_func)value_cmp_uint32_t,
    (compare_func)value_cmp_uint64_t,
    (compare_func)value_cmp_float,
    (compare_func)value_cmp_double,
    (compare_func)value_cmp_long_double
};

int size_map[11] = {sizeof(int8_t), sizeof(int16_t), sizeof(int32_t),
                  sizeof(int64_t), sizeof(uint8_t), sizeof(uint16_t),
                  sizeof(uint32_t), sizeof(uint64_t), sizeof(float),
                  sizeof(double), sizeof(long double)};