//
// Created by admin on 2020/4/10.
//

#ifndef AKEP_AKEP_BASE_H
#define AKEP_AKEP_BASE_H

#include "WjCryptLib_Md5.h"
#include"AKEP_DATA_STRUCT.h"
#include <string.h>
#include <stdint.h>



/// N_slash_l 表示为(LEN_N/l)，一个N可以分割为多少个l长的数
#define N_slash_l (LEN_N/l)


/// 长度N需要用多少个CTN寸下，这里可以浮动的
typedef CTN N;
typedef CTN N_2;
typedef CTN KL;
/// definition of OP, consists of op_value(2N) and op_id(N)
typedef struct {
    N_2 op_value;
    N op_id;
} OP;
//HSK 是l的容器
typedef uint32_t RN;


void set_parameters(unsigned N_v, unsigned l_v, unsigned k_v);

int get_N_len();

int get_N_size();

int get_l();

int get_k();

extern unsigned LEN_N;
extern unsigned k;
extern unsigned l;
/// function implement of N
#define init_n(n) init_ctn(n,LEN_N)

#define destroy_n(n) destroy_ctn(n)

#define compare_n(a, b) compare_ctn(a, b)

#define copy_n(dst, src) copy_ctn(dst, src)

#define  rand_n(n) rand_ctn(n)


/// function implement of N
#define init_n_2(n) init_ctn(n,LEN_N*2)

#define destroy_n_2(n) destroy_ctn(n)

#define compare_n_2(a, b) compare_ctn(a, b)

#define copy_n_2(dst, src) copy_ctn(dst, src)

#define rand_n_2(n) rand_ctn(n)

/// function implement of OP
void init_op(OP *op);

void destroy_op(OP *op);

bool compare_op(OP a, OP b);//比对是否相等

void copy_op(OP dst, OP src);

void rand_op(OP op);

int size_op(OP op);

/// function for AKEP base operation
CVL32 f(CVL32 d, N x);

void HashMD5(const char *string, MD5_HASH *md5Hash);

void AKEP_initial();

void g(N const BC_stamp, N const rt_id, OP const op, CTN ret);

void GetAuth(CTN token, RN *h, CTN ret);

#endif //AKEP_AKEP_BASE_H
