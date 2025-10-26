#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Inicializa a matriz M (N x N). 
// Se "esparsa" for 1, apenas algumas linhas terão valores não nulos.
void inicializa_matriz(double *M, int N, int esparsa) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (esparsa && (i % 10 != 0)) {
                M[i * N + j] = 0.0;  // maioria das linhas com zeros
            } else {
                M[i * N + j] = (double)(rand() % 10);  // valores aleatórios
            }
        }
    }
}

int main() {
    int N = 1024;  // tamanho da matriz (pode testar 8192 ou 11264)
    printf("Multiplicação de matrizes %dx%d\n", N, N);

    // Alocação dinâmica das matrizes
    double *A = (double*) malloc(N * N * sizeof(double));
    double *B = (double*) malloc(N * N * sizeof(double));
    double *C = (double*) malloc(N * N * sizeof(double));

    if (!A || !B || !C) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Inicialização: A esparsa, B densa
    inicializa_matriz(A, N, 1);  // 1 → esparsa
    inicializa_matriz(B, N, 0);  // 0 → densa

    double inicio = omp_get_wtime();

    // Multiplicação sequencial: C = A × B
    #pragma omp parallel for  schedule(runtime)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;
            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }

    double fim = omp_get_wtime();

    printf("Tempo sequencial: %.3f segundos\n", fim - inicio);
    printf("C[0][0] = %.2f\n", C[0]); // pequena checagem

    free(A);
    free(B);
    free(C);

    return 0;
}
