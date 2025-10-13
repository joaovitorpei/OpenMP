//----------------------------------------------------------------------------------------
// #include <stdio.h>
//  #include <omp.h>
//  int main(void) {
//  const int N = 100000;
//  static int A[100000], B[100000];
//  #pragma omp parallel
//  {
//  #pragma omp for
//  for (int i = 0; i < N; i++) {
//  // Fase 1: produzir A
//  A[i] = i;
// } // <- barreira implícita: A pronto
//  #pragma omp for
//  for (int i = 0; i < N; i++) {
//  B[i] = 2 * A[i]; // Fase 2: consumir A com segurança
//  } // <- barreira implícita
//  }
//  printf("B[123]=%d\n", B[123]);
//  return 0;
//  }
//----------------------------------------------------------------------------------------
// private
//  #include <stdio.h>
//  #include <omp.h>

// int main(void) {
//   const int N = 8;
//   static int A[8], B[8];
//   for (int i = 0; i < N; i++)
//     A[i] = i;
//   // se ficar compartilhado,
//   // várias threads disputam 'tmp'
//   //int tmp = 0;
//   #pragma omp parallel for default(none) shared(A, B,N) //private(tmp)
//   for (int i = 0; i < N; i++) {
//     int tmp = 0;
//     // cada thread tem seu próprio 'tmp'
//     tmp = A[i] * A[i];
//     B[i] = tmp + 1;
//    // printf("%d\n",tmp);
//   }
//   for (int i = 0; i < N; i++)
//     printf("%d ", B[i]);
//   puts("");
//   return 0;
// }
//----------------------------------------------------------------------------------------
// firstprivate(list)
// #include <omp.h>
// #include <stdio.h>
// int main(void) {
//   const int N = 8;
//   static int A[8], B[8];
//   for (int i = 0; i < N; i++)
//     A[i] = i;
//   // definido fora da região
//   int escala = 3;
// #pragma omp parallel for default(none) shared(A, B,N) firstprivate(escala)
//   for (int i = 0; i < N; i++) {
//     // cada thread tem sua própria
//     // cópia inicializada com 3
//     B[i] = escala * A[i];
//     // modifica a CÓPIA local,
//     // não afeta as outras threads
//     escala += 1;
//   }
//   for (int i = 0; i < N; i++)
//     printf("%d ", B[i]);
//   puts("");
//   // aqui, 'escala' fora
//   // da região continua valendo 3
//   return 0;
// }

//----------------------------------------------------------------------------------------
// lastprivate(list)
// #include <omp.h>
// #include <stdio.h>
// int main(void) {
//   const int N = 20;
//   int ultimo = -1; // fora do for
// #pragma omp parallel for default(none) lastprivate(ultimo) shared(N)
//   for (int i = 0; i < N; i++) {
//     // 'ultimo' aqui é privado por thread
//     if (i % 3 == 0)
//       ultimo = i;
//   } // <- ao sair, 'ultimo' recebe o
//   // valor da ÚLTIMA iteração (i=N-1) em ordem seq.
//   printf("ultimo multiplo de 3 em [0,%d): %d\n", N, ultimo);
//   // deve ser 18
//   return 0;
// }

//----------------------------------------------------------------------------------------
// schedule
// #include <stdio.h>
// #include <omp.h>
// int main(void){
//     int a[10],b[10];
//     for(int i=0;i<10;i++){
//         a[i]=i;
//         b[i]=i;
//     }
//     int soma=0;
//     #pragma omp parallel shared(a,b)
//     #pragma omp for schedule (runtime) reduction(+:soma)
//     for(int i=0;i<10;i++){
//         soma=soma+a[i]+b[i];
//     }
//     printf("soma total = %d\n",soma);
//       //para rodar: export OMP_SCHEDULE="static,2" ./t2
//       //OMP_SCHEDULE="static" ./prog
//       //OMP_SCHEDULE="static,1024" ./prog
//       //OMP_SCHEDULE="dynamic,512" ./prog
//       //OMP_SCHEDULE="guided,256" ./prog
// }

// static
#include <omp.h>
#include <stdio.h>
int main(void) {
  const int N = 100000;
  static double A[100000], B[100000];
  for (int i = 0; i < N; i++)
    A[i] = i;
  double t0 = omp_get_wtime();
#pragma omp parallel for schedule(static,1024)
  // ou schedule(static, 1024)
  for (int i = 0; i < N; i++) {
    B[i] = 2.0 * A[i];
  }
  double t1 = omp_get_wtime();
  //printf("tempo do t0 = %.3f s\n", t0);
  //printf("tempo do t1 = %.3f s\n", t1);
  printf("tempo(static) = %.3f s  (B[99999]=%.1f)\n", t1 - t0, B[N - 1]);
  return 0;
}
