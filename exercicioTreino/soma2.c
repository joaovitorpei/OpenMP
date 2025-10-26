#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const int N = 100000000;  // 100 milhões de elementos

    // Aloca dinamicamente o vetor no heap
    int *vetor = (int*) malloc(N * sizeof(int));
    if (vetor == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Inicializa o vetor
    for (int i = 0; i < N; i++)
        vetor[i] = 1;  // Soma esperada = N

    // Mede o tempo de execução da soma sequencial
    double inicio = omp_get_wtime();

    long long soma = 0;
    #pragma omp parallel for default(none) shared(vetor,N) reduction(+:soma) 
    for (int i = 0; i < N; i++) {
        soma += vetor[i];
    }

    double fim = omp_get_wtime();

    printf("Soma = %lld\n", soma);
    printf("Tempo sequencial: %.4f segundos\n", fim - inicio);

    free(vetor);
    return 0;
}
