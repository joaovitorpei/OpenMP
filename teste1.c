#include <stdio.h>
#include <omp.h>

int main() {
    /*int a=1;
    //omp_set_num_threads(2);//numero de threads serem ultilizadas nas regios paralelas
    #pragma omp parallel reduction(+:a)
    a=a+2;
    printf("resultado= %d\n",a);
    */
   

   /* #pragma omp parallel
    {
        int num=omp_get_num_threads();//numero de threads na regiao paralela
        printf("numero de threads: %d\n",num);
        int id=omp_get_thread_num();//id da thread indentificador das threads
        printf("primeiro thread:%d\n",id);
        printf("segundo thread: %d\n",id);
    }*/

    /*
    //clausula private
    int a=-1;
    #pragma omp parallel private(a)
    printf("dentro = %d\n",a);
    printf("fora = %d\n",a);
    */


    //clausula  firstprivate
    /*int a=-1;
    #pragma omp parallel firstprivate(a)
    {
        printf("dentro = %d\n",a);
        a=10;
        printf("dentro pos = %d\n",a);
    }
    printf("fora = %d\n",a);
    */

    //clausula shared
    /*int a=-1;
    #pragma omp parallel shared(a)
    {
        printf("dentro = %d\n",a);
        a= omp_get_thread_num();
        printf("dentro pos = %d\n",a);
    }
    printf("fora = %d\n",a);//ultimo valor atribuido
    */

    //clausula reduction
    /*int a=2;
    #pragma omp parallel reduction(+:a)
    a=a+2;
    printf("resultado = %d\n",a);
    */

    //directiva for
    /*int a[10],b[10],c[10];
    for(int i=0;i<10;i++){
        a[i]=i;
        b[i]=i;
    }
    int id;
    #pragma omp parallel 
    {
        printf("thead : %d\n",id=omp_get_thread_num());
       #pragma omp for
       for(int i=0;i<10;i++){
        c[i]=a[i]+b[i];
        printf("thread %d calculou c[%d] = %d\n", id, i, c[i]);
       }
   }
    for(int i=0;i<10;i++)
        printf("c[%d]=%d\n",i,c[i]);*/
    


    // clausula schedule
    /*
    int a[10],b[10];
    for(int i=0;i<10;i++){
        a[i]=i;
        b[i]=i;
    }
    int soma=0;
    #pragma omp parallel shared(a,b)
    #pragma omp for schedule (static,2) reduction(+:soma)
    for(int i=0;i<10;i++){
        soma=soma+a[i]+b[i];
    }
    printf("soma total = %d\n",soma);

    */

    //diretiva sections
    /*
    int id;
    #pragma omp parallel 
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                id=omp_get_thread_num();
                printf("thread %d executando a secao 1\n",id);
            }
            #pragma omp section
            {
                id=omp_get_thread_num();
                printf("thread %d executando a secao 2\n",id);
            }
            #pragma omp section
            {
                id=omp_get_thread_num();
                printf("thread %d executando a secao 3\n",id);
            }
            
        }
    }
        */

    //diretiva single
   /*
    #pragma omp parallel
    {
        printf("thread %d : inicianda\n",omp_get_thread_num());
        #pragma omp single
        printf("total de thread: %d\n",omp_get_num_threads());
    }
        */

    //diretiva critical
    /*
    int x=0;
    #pragma omp parallel shared(x)
    {
       #pragma omp critical
        x++;
    }
    printf("x= %d\n",x);
    */
    //diretiva barrier
    /*
    int soma=0;
    int a[5],b[5];
    #pragma omp parallel shared(soma,a,b)
    {
        #pragma omp sections 
        {
            #pragma omp section
            {
                for(int i=0;i<2;i++){
                    a[i]=10;
                }
            }
            #pragma omp section
            {
                for(int i=0;i<2;i++){
                    b[i]=100;
                }
            }
            
        }
        #pragma omp barrier
        //pode usar tambem single para so uma thread executar esse trecho ou critical para entra uma thread por vez na soma
    
        #pragma omp for reduction(+:soma)
        for(int i =0;i<2;i++){
            soma=soma+a[i]+b[i];
        }
    }
    printf("soma: %d\n",soma);
    */
    return 0;
}