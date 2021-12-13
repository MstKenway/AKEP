//
// Created by admin on 2020/4/11.
//

#include "AKEP_RT.h"
#include <stdlib.h>

void AKEP_RT_init(RTInfo *rt) {
    AKEP_initial();
    /// allocate memory
    init_n(&rt->BC_stamp);
    init_n(&rt->RTid);
    init_op(&rt->Op);
    init_ctn(&rt->token, (k + 1) * rt->RTid.size * 8);
    rt->H = (RN *) malloc(k * sizeof(RN));
    rt->R = (RN *) malloc(k * sizeof(RN));

    /// get random value
//    rand_n(rt->BC_stamp);
//    rand_n(rt->RTid);
//    rand_op(rt->Op);
}

void AKEP_RT_stop(RTInfo *rt) {
    destroy_n(&rt->BC_stamp);
    destroy_n(&rt->RTid);
    destroy_op(&rt->Op);
    destroy_ctn(&rt->token);
    if (rt->H != NULL)
        free(rt->H);
    if (rt->R != NULL)
        free(rt->R);
}

void recInit(RTInfo *rt, const N RT, const OP Op) {
    copy_n(rt->RTid, RT);
    copy_op(rt->Op, Op);
}

void retRtAuth(RTInfo *rt, N bcstamp, const RN *h, N_2 ret) {
    copy_n(rt->BC_stamp, bcstamp);
    for (int i = 0; i < k; ++i) {
        rt->H[i] = h[i];
    }
    g(rt->BC_stamp, rt->RTid, rt->Op, rt->token);
    GetAuth(rt->token, rt->H, ret);
}

void preRtRn(RTInfo *rt, RN *r) {
    unsigned mask = l >= 32 ? 0xffffffff : ((1 << l) - 1);

    for (int i = 0; i < k; ++i) {
        rt->R[i] = rand() & mask;
        r[i] = rt->R[i];
    }
}

bool VerifyBC(RTInfo *rt, unsigned int ack, CTN bcAuth) {
    if (ack != true) {
        return false;
    }
    N_2 auth;
    init_n_2(&auth);
    GetAuth(rt->token, rt->R, auth);
    bool ret = true;
    ret = compare_n_2(bcAuth, auth);
    destroy_n_2(&auth);
    return ret;
}
