// tarefas task
//  #include <stdio.h>
// #include <stdlib.h>
// #include <omp.h>

// int main(void) {
//     const int N = 20; // usei 20 pra ficar mais fácil de ver
//     double *A = (double *) malloc(N * sizeof(double));

//     for (int i = 0; i < N; i++)
//         A[i] = 1.0; // vetor preenchido com 1.0

//     double sumL = 0.0;
//     double sumR = 0.0;

//     printf("Iniciando com %d threads...\n", omp_get_max_threads());

//     // Região paralela
//     #pragma omp parallel
//     {
//         // Apenas uma thread cria as tarefas
//         #pragma omp single
//         {
//             printf("[Thread %d] Criando as tarefas...\n",
//             omp_get_thread_num());

//             // --- TAREFA 1 ---
//             #pragma omp task shared(sumL, A)
//             {
//                 printf("  -> [Thread %d] Iniciou tarefa 1 (soma esquerda)\n",
//                 omp_get_thread_num()); double acc = 0.0; for (int i = 0; i <
//                 N / 2; i++)
//                     acc += A[i];
//                 sumL = acc;
//                 printf("  -> [Thread %d] Terminou tarefa 1: sumL=%.0f\n",
//                 omp_get_thread_num(), sumL);
//             }

//             // --- TAREFA 2 ---
//             #pragma omp task shared(sumR, A)
//             {
//                 printf("  -> [Thread %d] Iniciou tarefa 2 (soma direita)\n",
//                 omp_get_thread_num()); double acc = 0.0; for (int i = N / 2;
//                 i < N; i++)
//                     acc += A[i];
//                 sumR = acc;
//                 printf("  -> [Thread %d] Terminou tarefa 2: sumR=%.0f\n",
//                 omp_get_thread_num(), sumR);
//             }

//             // --- ESPERA AS DUAS TAREFAS TERMINAREM ---
//             #pragma omp taskwait
//             double total = sumL + sumR;
//             printf("[Thread %d] Final: sumL=%.0f  sumR=%.0f  total=%.0f
//             (esperado=%.0f)\n",
//                    omp_get_thread_num(), sumL, sumR, total, (double) N);
//         }
//     }

//     free(A);
//     return 0;
// }

// teste recursividade com tarefas
//  #include <stdio.h>
//  #include <stdlib.h>
//  #include <omp.h>

// int fib_task(int n) {
//     if (n < 2) return n;

//     int a = 0, b = 0;
//     if (n > 20) {
//         #pragma omp task shared(a) firstprivate(n)
//         { a = fib_task(n - 1); }

//         #pragma omp task shared(b) firstprivate(n)
//         { b = fib_task(n - 2); }

//         #pragma omp taskwait
//     } else {
//         a = fib_task(n - 1);
//         b = fib_task(n - 2);
//     }
//     return a + b;
// }

// int fib_parallel(int n) {
//     int result = 0;
//     #pragma omp parallel
//     {
//         #pragma omp single
//         { result = fib_task(n); }
//     }
//     return result;
// }

// int main(void) {
//     int n = 35;
//     double t0 = omp_get_wtime();
//     int f = fib_parallel(n);
//     double t1 = omp_get_wtime();
//     printf("fib(%d) = %d  tempo=%.3f s\n", n, f, t1 - t0);
//     puts("oiiiiiiiii");
//     puts("fimmmmmmmmmm");
//     return 0;
// }

// teste taskgroup
#include <omp.h>
#include <stdio.h>
void work(int depth) {
  if (depth <= 0)
    return;
// Cada chamada cria uma nova tarefa recursiva
  #pragma omp task firstprivate(depth)
  {
    printf("tarefa em profundidade %d executada pela thread %d\n", depth,omp_get_thread_num());
    work(depth - 1);
  }
}
int main(void) {
  #pragma omp parallel
  {
    #pragma omp single
    {
      // O grupo de tarefas inclui
      // todas as tarefas
      // criadas dentro deste bloco
      #pragma omp taskgroup
      {
        for (int i = 0; i < 4; i++) {
          #pragma omp task firstprivate(i)
          { work(5 + i); }
        }
      }
      // Somente após o término
      // de todas as tarefas (e suas descendentes)
      // o programa chega a este ponto
      printf("grupo de tarefas concluído\n");C
    }
  }
  return 0;
}
