//
// Created by GaoYun on 2021/12/6.
//
#include<string.h>
#include <stdlib.h>
#include "AKEP_DATA_STRUCT.h"


void init_ctn(CTN *ctn, int len) {
    if (ctn == NULL)return;
    ctn->bytes = NULL;
    ctn->len = len;
    ctn->size = ((len - 1) / 8) + 1;
    ctn->bytes = (byte *) malloc(ctn->size);
    memset(ctn->bytes, 0, ctn->size);
}

void destroy_ctn(CTN *ctn) {
    if (ctn == NULL)return;
    if (ctn->bytes != NULL) {
        free(ctn->bytes);
        ctn->bytes = NULL;
        ctn->size = 0;
        ctn->len = 0;
    }
}

bool compare_ctn(const CTN a, const CTN b) {
    if (a.size != b.size)return false;
    if (a.len != b.len)return false;
    for (int i = 0; i < a.size; ++i) {
        if (a.bytes[i] != b.bytes[i])
            return false;
    }
    return true;
}

void copy_ctn(CTN dst, const CTN src) {
    if (dst.size != src.size)return;
    if (dst.len != src.len)return;
    memcpy(dst.bytes, src.bytes, dst.size);
}

void rand_ctn(CTN ctn) {
    if (ctn.len <= 0)return;
    for (int i = 0; i < ctn.size; ++i) {
        ctn.bytes[i] = rand() & 0xff;
    }
    uint16_t mask = (ctn.len % 8) == 0 ? 0xff : (1 << (ctn.len % 8));
    ctn.bytes[0] &= mask;
}

/// get part of container
CVL32 get_part_of_ctn(CTN ctn, int index, int len) {
    if (index <= 0)return -1;
    if (index * len > LEN_OF_CTN(ctn))return -1;
    CVL32 ret = 0;
    unsigned mask = len >= 32 ? 0xffffffff : (1 << len) - 1;
    int end = ctn.size - 1, start;
    end -= (index - 1) * len / 8;
    start = (end >= 3) ? end - 3 : 0;
    int left = (index - 1) * len % 8;
    for (int i = start; i <= end; ++i) {
        ret = (ret << 8) | ctn.bytes[i];
    }
    ret = ret >> left;
    return ret & mask;
}


/// A function turn uint32 to CTN
/// \param d
/// \param s
void u32_to_ctn(CTN d, uint32_t s) {
    for (int i = d.size - 1; i >= 0; --i) {
        d.bytes[i] = s & 0xff;
        s = s >> 8;
    }
    if (d.size > 0) {
        uint16_t mask = (d.len % 8) == 0 ? 0xff : (1 << (d.len % 8));
        d.bytes[0] &= mask;
    }
}