//
// Created by admin on 2020/4/10.
//
#include "AKEP_Base.h"
#include <stdlib.h>
#include <time.h>

#define MD5_LEN 128
/// Parameter LEN_N
unsigned LEN_N = 64;
/// Parameter l
unsigned l = 4;
/// Parameter k
unsigned k = 12;
/// N_N定义为当前N需要几个CTN来表示
#define N_SIZE_IN_CTN ((LEN_N-1)/8+1)

void set_parameters(unsigned N_v, unsigned l_v, unsigned k_v) {
    LEN_N = N_v;
    l = l_v;
    k = k_v;
}

int get_N_len() {
    return LEN_N;
}

int get_N_size() {
    return N_SIZE_IN_CTN;
}

int get_l() {
    return l;
}

int get_k() {
    return k;
}


void init_op(OP *op) {
    if (op) {
        init_n_2(&op->op_value);
        init_n(&op->op_id);
    }
}

void destroy_op(OP *op) {
    if (op) {
        destroy_n_2(&op->op_value);
        destroy_n(&op->op_id);
    }
}

bool compare_op(OP a, OP b) {
    return compare_ctn(a.op_value, b.op_value) &&
           compare_ctn(a.op_id, b.op_id);
}

void copy_op(OP dst, OP src) {
    copy_ctn(dst.op_value, src.op_value);
    copy_ctn(dst.op_id, src.op_id);
}

void rand_op(OP op) {
    rand_ctn(op.op_value);
    rand_ctn(op.op_id);
}

int size_op(OP op) {
    return op.op_id.size + op.op_value.size;
}

/// confirm that the inputs of function f satisfy 2^l>=LEN_N/l
CVL32 f(CVL32 d, N const x) {
    CVL32 result = 0;
    CVL32 mask = 0;
    /// construct mask
    mask = l >= 32 ? 0xffffffff : ((1 << l) - 1);
    for (unsigned int i = 0; i < N_slash_l; ++i) {
        if ((d >> i) & 1) {
            result = (result + get_part_of_ctn(x, i + 1, l)) & mask;
        }
    }
    return result;
}

void md5_to_ctn(MD5_HASH *md5, CTN ctn) {
    if (ctn.size <= 0)return;
    if (md5) {
        int pos = 0;
        for (int i = 0; i < ctn.size; ++i) {
            ctn.bytes[i] = md5->bytes[pos];
            pos = (pos + 1) % MD5_HASH_SIZE;
        }
        uint16_t mask = (ctn.len % 8) == 0 ? 0xff : (1 << (ctn.len % 8));
        ctn.bytes[0] &= mask;
    }
}

/// input 4N len output (k+1)N len
/// \param BC_stamp N len
/// \param rt_id N len
/// \param op 2N + N len
/// \param ret the ret value, (k+1)N len
void g(N const BC_stamp, N const rt_id, OP const op, CTN ret) {
    int input_len = BC_stamp.size + rt_id.size + op.op_value.size;
    int p = 0;
    byte to_be_hashed[input_len + 1];
    to_be_hashed[input_len] = 0;
    memcpy(to_be_hashed, BC_stamp.bytes, BC_stamp.size);
    p += BC_stamp.size;
    memcpy(to_be_hashed + p, rt_id.bytes, rt_id.size);
    p += rt_id.size;
    memcpy(to_be_hashed + p, op.op_value.bytes, op.op_value.size);
    to_be_hashed[input_len] = '\0';
    //把hash结果存入ret，长度为(k+1)N
    MD5_HASH md5Hash;
    HashMD5(to_be_hashed, &md5Hash);
    md5_to_ctn(&md5Hash, ret);
}

/// get authentication passport though H1(d,token)
/// \param token (k+1)N len
/// \param h k len
/// \param ret  auth, 2N len
void GetAuth(CTN token, RN *h, CTN ret) {
    CTN f_res;
    N f_input;
    init_n(&f_input);
    init_ctn(&f_res, l);
    int input_len = (k) * f_res.size;
    byte to_be_hashed[input_len + 1];
    to_be_hashed[input_len] = 0;
    CVL32 t;
    for (int i = 0; i < k; ++i) {
        memcpy(f_input.bytes, token.bytes + i * f_input.size, f_input.size);
        t = f(h[i], f_input);
        u32_to_ctn(f_res, t);
        memcpy(to_be_hashed + f_res.size * i, f_res.bytes, f_res.size);
    }
    //把hash结果存入ret 2N length.
    MD5_HASH md5Hash;
    HashMD5(to_be_hashed, &md5Hash);
    md5_to_ctn(&md5Hash, ret);
    destroy_ctn(&f_res);
    destroy_n(&f_input);
}

///
/// \param string need to be ended as a string with '\0' at the end.
/// \param md5Hash a new structure which restores the result.
void HashMD5(const char *string, MD5_HASH *md5Hash) {
    Md5Context md5Context;
    Md5Initialise(&md5Context);
    Md5Update(&md5Context, string, (uint32_t) strlen(string));
    Md5Finalise(&md5Context, md5Hash);
}

/// init AKEP
/// including init random seed.
void AKEP_initial() {
    srand(time(0));
}
