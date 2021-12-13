//
// Created by GaoYun on 2021/11/29.
//
#include<stdlib.h>
#include<stdio.h>
#include <stdint.h>
#include <time.h>
#include"AKEP-src/include/AKEP_Base.h"

int main() {
    srand(time(0));
    set_parameters(256, 17, 4);
    N n;
    init_n(&n);
    rand_n(n);
    for (int i = 0; i < n.size; ++i) {
        printf("%u ", n.bytes[i]);
    }
    printf("\n");
    for (int i = 0; i < 256 / l; ++i) {
        printf("%d\n", get_part_of_ctn(n, i + 1, l));
    }
    destroy_n(&n);


    return 0;
}