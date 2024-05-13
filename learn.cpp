#include "bm.h"

int main(void){
    int n = 10;
    int num = 100;
    BM bm1(n);
    BM bm2(n);

    bm1.dataGen(num);
    bm2.dataRead(num);
    bm2.train();

    return 0;
}