#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const int N = 100000000; // 100 milhões
    
    int *A = (int*) malloc(N * sizeof(int));
    int *B = (int*) malloc(N * sizeof(int));
    int *C = (int*) malloc(N * sizeof(int));

    if ((A == NULL) || (B == NULL) || (C == NULL)) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Inicializa o vetor 1
    for (int i = 0; i < N; i++){
        A[i] = i;
        B[i] = i*2; 
    }
        
    double inicio = omp_get_wtime();
    
    long long soma = 0;
    #pragma omp parallel shared(A,B,C) //num_threads(4)
    {
       #pragma omp for        
       for (int i = 0; i < N; i++) {
          C[i] =  A[i] +  B[i];
        }
    }
    double fim = omp_get_wtime();

    for (int i = 0; i < N; i++){
        soma +=  C[i];        
    }

    printf("Soma = %lld\n", soma);
    printf("Tempo sequencial: %.4f segundos\n", fim - inicio);

    free(A);
    free(B);
    free(C);
    return 0;
}

// # **Soma de Vetor**


// Dado um vetor de 100 milhões de inteiros, calcule a soma total de todos os elementos.
// Compare o tempo de execução sequencial com a versão paralelizada com OpenMP.

// ---

// ### Código Sequencial

// ```c
// #include <stdio.h>
// #include <stdlib.h>
// #include <omp.h>

// int main() {
//     const int N = 100000000; // 100 milhões
//     int *vetor = (int*) malloc(N * sizeof(int));
//     if (vetor == NULL) {
//         printf("Erro ao alocar memória.\n");
//         return 1;
//     }

//     // Inicializa o vetor
//     for (int i = 0; i < N; i++)
//         vetor[i] = 1; // soma esperada = N

//     double inicio = omp_get_wtime();
    
//     long long soma = 0;
//     for (int i = 0; i < N; i++) {
//         soma += vetor[i];
//     }
    
//     double fim = omp_get_wtime();

//     printf("Soma = %lld\n", soma);
//     printf("Tempo sequencial: %.4f segundos\n", fim - inicio);

//     free(vetor);
//     return 0;
// }
// ```


