//
// Created by admin on 2020/4/11.
//

#ifndef AKEP_AKEP_RT_H
#define AKEP_AKEP_RT_H

#include "AKEP_Base.h"

typedef struct {
    N RTid, BC_stamp;
    CTN token;/// (K+1)N len
    OP Op;
    RN *H;
    RN *R;
} RTInfo;

//每个RT初始化的函数
void AKEP_RT_init();

//RT结束时释放资源
void AKEP_RT_stop(RTInfo *rt);

//RT初始化时接受BC的口令集函数
void recInit(RTInfo *rt, const N RT, const OP Op);

//根据提供的bctemp和h1、h2，计算返回AUTH+
void retRtAuth(RTInfo *rt, N bcstamp, const RN *h, N_2 ret);

//产生r
void preRtRn(RTInfo *rt, RN *r);

//验证BC的身份
bool VerifyBC(RTInfo *rt, unsigned int ack, CTN bcAuth);

#endif //AKEP_AKEP_RT_H
