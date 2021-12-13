#include <stdio.h>
#include <stdlib.h>
#include "AKEP_BC.h"
#include "AKEP_RT.h"
#include <time.h>
#include <math.h>

#define TestNum 1000


int test() {
    //inital bc
    AKEP_BC_init();
    //initial rt
    RTInfo RTs[RTNum];
    for (int i = 0; i < RTNum; ++i) {
        AKEP_RT_init(&RTs[i]);
    }
    //inital RT AKEP data
    N rtid;
    init_n(&rtid);
    OP op;
    init_op(&op);
    for (int i = 0; i < RTNum; ++i) {
        sendInit(i, rtid, op);
        recInit(&RTs[i], rtid, op);
    }
    //akep
    N bc_stamp;
    N_2 auth, bcAuth;
    init_n(&bc_stamp);
    init_n_2(&auth);
    init_n_2(&bcAuth);


    bool result;
    clock_t start, end;
    start = clock();
    RN h[k], r[k];
    for (int i = 0; i < TestNum; ++i) {
        for (int j = 0; j < RTNum; ++j) {
            //bc->rt bc_stamp h
            sendBCstamp(j, h, bc_stamp);
//            printf("round %d bc send bc_stamp to rt %d !\n continue\n", i, j);
            //rt->bc auth,r
            retRtAuth(&RTs[j], bc_stamp, h, auth);
//            printf("round %d rt %d ret auth to bc !\n continue\n", i, j);
            preRtRn(&RTs[j], r);
//            printf("round %d rt %d send random to bc !\n continue\n", i, j);
            //bc verify rt && cal auth
            result = VerifyRT(j, RTs[j].RTid, auth, r, bcAuth);
            if (result == false) {
                printf("round %d bc verify rt %d fail!\n continue\n", i, j);
                continue;
            }
//            printf("round %d verify rt %d success!\n continue\n", i, j);
            if (VerifyBC(&RTs[j], result, bcAuth) == false) {
                printf("round %d rt %d verify bc  fail!\n continue\n", i, j);
                continue;
            }
//            printf("bc and rt %d akep test success!\n",j);
        }
//        printf("round %d finish\n",i);
    }
    end = clock();
    AKEP_BC_stop();
    for (int i = 0; i < RTNum; ++i) {
        AKEP_RT_stop(&RTs[i]);
    }
//    printf("执行%d轮%d个RT共计%d次完整接入认证所需时间为%lldms\n", TestNum, RTNum, TestNum * RTNum, end - start);
//    printf("单次完整接入认证所需时间为%lfms\n\n", (end - start) * 1.0 / (TestNum * RTNum));
    printf(" %d %d %d %ld ", TestNum, RTNum, TestNum * RTNum, end - start);
    printf(" %lf \n", (end - start) * 1.0 / (TestNum * RTNum) * 1000 / CLOCKS_PER_SEC);
    destroy_n_2(&auth);
    destroy_n_2(&bcAuth);
    destroy_n(&bc_stamp);
    destroy_op(&op);
    destroy_n(&rtid);
    return 0;
}

unsigned long long pow_2(int len) {
    return 1 << len;
}

int main() {
    printf("N\tk\tl\t\tTestNum\tRTnum\tTestNum * RTNum\tTtime\tStime\n");
    for (int N = 16; N <= 256; ++N) {
        for (int k = 1; k <= 32; ++k) {
            for (int l = 4; l <= 32 && l < N; ++l) {
                set_parameters(N, l, k);
                if (l > N)
                    continue;
                if (N % l != 0)
                    continue;
                if (((unsigned long long) 1 << l) < N / l)
                    continue;
                printf("%d\t%d\t%d\t\t", N, k, l);
                test();
            }
        }

    }
    return 0;
}