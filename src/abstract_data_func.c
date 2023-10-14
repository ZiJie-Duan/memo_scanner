#include "memo_scanner.h"

/* mbi_list_init
 * 初始化内存信息链表
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
 * 向内存信息链表中添加节点
 * 参数: mbi_list 内存信息链表
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
 * 释放内存信息链表
 * 参数: mbi_list 内存信息链表
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

MEMORY_LIST *memoli_init(void){
    MEMORY_LIST *memoli = malloc(sizeof(MEMORY_LIST));
    if (memoli == NULL) {
        error(1, "malloc failed");
    }

    memoli->head = NULL;
    memoli->tail = NULL;
    return memoli;
}

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

void memoli_add_block(MEMORY_LIST *memoli, int id, char type, LPVOID addr, 
        void *value){

    MEMORY_BLOCK *node = malloc(sizeof(MEMORY_BLOCK));
    if (node == NULL) {
        error(1, "malloc failed");
    }
    node->addr = addr;
    node->id = id;
    node->type = type;
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

void memoblk_update_value(MEMORY_BLOCK *memoblk, char type, void * value){
    VALUE_BLOCK *last_v = memoblk->values;
    memoblk->values = add_valueblk_multy(value, type);
    memoblk->values->next = last_v;
}

VALUE_BLOCK *add_valueblk_multy(void *value, char type){
    VALUE_BLOCK * value_blk = malloc(sizeof(VALUE_BLOCK));
    if (value_blk == NULL) error(1, "malloc failed");
    value_blk->next = NULL;
    value_blk->time = 0;

    if (type == INT_8_TYPE) value_blk->value = memoblk_v_int_8(value);
    if (type == INT_16_TYPE) value_blk->value = memoblk_v_int_16(value);
    if (type == INT_32_TYPE) value_blk->value = memoblk_v_int_32(value);
    if (type == INT_64_TYPE) value_blk->value = memoblk_v_int_64(value);
    if (type == UINT_8_TYPE) value_blk->value = memoblk_v_uint_8(value);
    if (type == UINT_16_TYPE) value_blk->value = memoblk_v_uint_16(value);
    if (type == UINT_32_TYPE) value_blk->value = memoblk_v_uint_32(value);
    if (type == UINT_64_TYPE) value_blk->value = memoblk_v_uint_64(value);
    if (type == FLOAT_TYPE) value_blk->value = memoblk_v_float(value);
    if (type == DOUBLE_TYPE) value_blk->value = memoblk_v_double(value);
    if (type == LONG_DOUBLE_TYPE)
    value_blk->value = memoblk_v_long_double(value);
    return value_blk;
}

int value_cmp_multy(char type, void * v1, void * v2){
    if (type == INT_8_TYPE){
        if (*(int8_t *)v1 > *(int8_t *)v2) return BIGGER_THAN;
        if (*(int8_t *)v1 < *(int8_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == INT_16_TYPE){
        if (*(int16_t *)v1 > *(int16_t *)v2) return BIGGER_THAN;
        if (*(int16_t *)v1 < *(int16_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == INT_32_TYPE){
        if (*(int32_t *)v1 > *(int32_t *)v2) return BIGGER_THAN;
        if (*(int32_t *)v1 < *(int32_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == INT_64_TYPE){
        if (*(int64_t *)v1 > *(int64_t *)v2) return BIGGER_THAN;
        if (*(int64_t *)v1 < *(int64_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == UINT_8_TYPE){
        if (*(uint8_t *)v1 > *(uint8_t *)v2) return BIGGER_THAN;
        if (*(uint8_t *)v1 < *(uint8_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == UINT_16_TYPE){
        if (*(uint16_t *)v1 > *(uint16_t *)v2) return BIGGER_THAN;
        if (*(uint16_t *)v1 < *(uint16_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == UINT_32_TYPE){
        if (*(uint32_t *)v1 > *(uint32_t *)v2) return BIGGER_THAN;
        if (*(uint32_t *)v1 < *(uint32_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == UINT_64_TYPE){
        if (*(uint64_t *)v1 > *(uint64_t *)v2) return BIGGER_THAN;
        if (*(uint64_t *)v1 < *(uint64_t *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == FLOAT_TYPE){
        if (*(float *)v1 > *(float *)v2) return BIGGER_THAN;
        if (*(float *)v1 < *(float *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == DOUBLE_TYPE){
        if (*(double *)v1 > *(double *)v2) return BIGGER_THAN;
        if (*(double *)v1 < *(double *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    if (type == LONG_DOUBLE_TYPE){
        if (*(long double *)v1 > *(long double *)v2) return BIGGER_THAN;
        if (*(long double *)v1 < *(long double *)v2) return SMALLER_THAN;
        return EQUAL;
    }
    return -1;
}

int sizeof_multy_type(char type){
    if (type == INT_8_TYPE) return sizeof(int8_t);
    if (type == INT_16_TYPE) return sizeof(int16_t);
    if (type == INT_32_TYPE) return sizeof(int32_t);
    if (type == INT_64_TYPE) return sizeof(int64_t);
    if (type == UINT_8_TYPE) return sizeof(uint8_t);
    if (type == UINT_16_TYPE) return sizeof(uint16_t);
    if (type == UINT_32_TYPE) return sizeof(uint32_t);
    if (type == UINT_64_TYPE) return sizeof(uint64_t);
    if (type == FLOAT_TYPE) return sizeof(float);
    if (type == DOUBLE_TYPE) return sizeof(double);
    if (type == LONG_DOUBLE_TYPE) return sizeof(long double);
    return -1;
}

int8_t * memoblk_v_int_8(int8_t *value){
    int8_t *p = malloc(sizeof(int8_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

int16_t * memoblk_v_int_16(int16_t *value){
    int16_t *p = malloc(sizeof(int16_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

int32_t * memoblk_v_int_32(int32_t *value){
    int32_t *p = malloc(sizeof(int32_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

int64_t * memoblk_v_int_64(int64_t *value){
    int64_t *p = malloc(sizeof(int64_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

uint8_t * memoblk_v_uint_8(uint8_t *value){
    uint8_t *p = malloc(sizeof(uint8_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

uint16_t * memoblk_v_uint_16(uint16_t *value){
    uint16_t *p = malloc(sizeof(uint16_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

uint32_t * memoblk_v_uint_32(uint32_t *value){
    uint32_t *p = malloc(sizeof(uint32_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

uint64_t * memoblk_v_uint_64(uint64_t *value){
    uint64_t *p = malloc(sizeof(uint64_t));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

float * memoblk_v_float(float *value){
    float *p = malloc(sizeof(float));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

double * memoblk_v_double(double *value){
    double *p = malloc(sizeof(double));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

long double * memoblk_v_long_double(long double *value){
    long double *p = malloc(sizeof(long double));
    if (p == NULL) {
        error(1, "malloc failed");
    }
    *p = *value;
    return p;
}

