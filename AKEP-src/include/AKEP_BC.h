//
// Created by admin on 2020/4/11.
//

#ifndef AKEP_AKEP_BC_H
#define AKEP_AKEP_BC_H

#include "AKEP_Base.h"
//生成33个RT，0号不用
#define RTNum 32

//BC初始化函数，包含随机数初始化和生成各RT的初始化数据
void AKEP_BC_init();

//BC释放资源
void AKEP_BC_stop();

//发送rt初始化数据
void sendInit(int rtNum, N RTid, OP op);

//发送BCtemp和h1、h2
void sendBCstamp(int rtNo, RN *h, N bc_stamp);

//验证RT的rtid，并根据r1、r2生成bcAuth
bool VerifyRT(int rtNum, N rt_id, N_2 rtAuth, RN *r, N bcAuth);

#endif //AKEP_AKEP_BC_H
