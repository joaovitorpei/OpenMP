#include <stdio.h>
#include <omp.h>


int main(){
    int max=6;

   #pragma omp parallel for collapse(2)
    for(int i=1;i<=max;i++)
      for(int j=1;j<=max;j++)
        printf("%d: (%d , %d)\n",omp_get_thread_num(),i,j);
    return 0;
}