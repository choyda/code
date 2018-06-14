#include <stdio.h>
#include <mm_malloc.h>



int main(){
int a=12345,d;

    while(a!=0){
        d=a%10;
        printf("%d", d);
        a/=10;
    }

}

// 分类 -------------- 内部比较排序

两两比较，以此往后放，直到不能移动为止
﻿


