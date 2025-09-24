#include <stdio.h>
#include <omp.h>

int main() {
    int a=1;
    //omp_set_num_threads(2);//numero de threads serem ultilizadas nas regios paralelas
    #pragma omp parallel reduction(+:a)
    a=a+2;
    printf("resultado= %d\n",a);
    return 0;
}