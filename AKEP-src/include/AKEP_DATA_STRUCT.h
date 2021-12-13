//
// Created by GaoYun on 2021/12/6.
//

#ifndef AKEP_AKEP_DATA_STRUCT_H
#define AKEP_AKEP_DATA_STRUCT_H

#include <stdint.h>

#define true 1
#define false 0
typedef uint8_t bool;

typedef uint8_t byte;
/// CTN(container) is a container with varying data length.
typedef struct {
    byte *bytes; /// data on heap
    short size;
    int len;
} CTN;
#define SIZE_OF_CTN(x) (x.size)
#define LEN_OF_CTN(x) (SIZE_OF_CTN(x)*8)
#define CTN_MAX ((1<<LEN_OF_CTN)-1)

/// container to restore varying-length l
typedef uint32_t CVL32;

void init_ctn(CTN *ctn, int len);

void destroy_ctn(CTN *ctn);

void rand_ctn(CTN ctn);

void copy_ctn(CTN dst, const CTN src);

bool compare_ctn(const CTN a, const CTN b);

CVL32 get_part_of_ctn(CTN ctn, int index, int len);

void u32_to_ctn(CTN d, uint32_t s);

#endif //AKEP_AKEP_DATA_STRUCT_H
