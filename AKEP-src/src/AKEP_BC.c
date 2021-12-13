//
// Created by admin on 2020/4/11.
//

#include "AKEP_BC.h"
#include<stdlib.h>

N RTIDs[RTNum], BC_stamps[RTNum];
OP OPs[RTNum];
RN *H[RTNum];
RN *R[RTNum];

void AKEP_BC_init() {
    AKEP_initial();
    for (int i = 0; i < RTNum; ++i) {
        /// allocate memory
        H[i] = (RN *) malloc(k * sizeof(RN));
        R[i] = (RN *) malloc(k * sizeof(RN));
        init_n(&RTIDs[i]);
        init_n(&BC_stamps[i]);
        init_op(&OPs[i]);

        /// assign random data
        rand_n(RTIDs[i]);
        rand_op(OPs[i]);
    }
}

void AKEP_BC_stop() {
    for (int i = 0; i < RTNum; ++i) {
        free(H[i]);
        free(R[i]);
        destroy_n(&RTIDs[i]);
        destroy_n(&BC_stamps[i]);
        destroy_op(&OPs[i]);
    }
}

void sendBCstamp(int rtNo, RN *h, N bc_stamp) {
    rand_n(BC_stamps[rtNo]);
    unsigned mask = l >= 32 ? 0xffffffff : ((1 << l) - 1);
    for (int i = 0; i < k; ++i) {
        H[rtNo][i] = rand() & mask;
        h[i] = H[rtNo][i];
    }
    copy_n(bc_stamp, BC_stamps[rtNo]);
}

void sendInit(int rtNum, N RTid, OP op) {
    copy_n(RTid, RTIDs[rtNum]);
    copy_op(op, OPs[rtNum]);
}

bool VerifyRT(int rtNum, N rt_id, N_2 rtAuth, RN *r, N bcAuth) {
    CTN token;
    init_ctn(&token, (k + 1) * rt_id.size * 8);
    g(BC_stamps[rtNum], RTIDs[rtNum], OPs[rtNum], token);
    N_2 auth;
    init_n_2(&auth);
    GetAuth(token, H[rtNum], auth);
    if (compare_n(rt_id, RTIDs[rtNum]) == false)
        return false;
    if (compare_n_2(auth, rtAuth) == false)
        return false;
    GetAuth(token, r, bcAuth);

    for (int i = 0; i < k; ++i) {
        R[rtNum][i] = r[i];
    }
    /// end free
    destroy_ctn(&token);
    destroy_n_2(&auth);
    return true;
}

