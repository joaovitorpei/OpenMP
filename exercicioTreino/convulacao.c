#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// ------------------------------------------------------
// Inicializa matriz de imagem com valores pseudo-aleatórios (0 a 255)
// ------------------------------------------------------
void inicializa_imagem(float *img, int N) {
    for (int i = 0; i < N * N; i++) {
        img[i] = (float)(rand() % 256);
    }
}

// ------------------------------------------------------
// Inicializa kernel 3x3 para um filtro "blur" (média)
// ------------------------------------------------------
void inicializa_kernel(float K[3][3]) {
    float blur[3][3] = {
        {1.0f/9, 1.0f/9, 1.0f/9},
        {1.0f/9, 1.0f/9, 1.0f/9},
        {1.0f/9, 1.0f/9, 1.0f/9}
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            K[i][j] = blur[i][j];
}

// ------------------------------------------------------
// Convolução 2D simples (varre a imagem e aplica o kernel 3x3)
// ------------------------------------------------------
void convolucao2D(float *entrada, float *saida, float K[3][3], int N) {
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 2400; j++) {
            float soma = 0.0f;

            // Aplica o kernel 3x3 ao redor do pixel (i, j)
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    soma += entrada[(i + ki) * N + (j + kj)] * K[ki + 1][kj + 1];
                }
            }

            saida[i * N + j] = soma;
        }
    }
}

// ------------------------------------------------------
// Função principal
// ------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <tamanho da imagem>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    printf("Convolução 2D com imagem %dx%d\n", N, N);

    // Alocação dinâmica
    float *imagem = (float*) malloc(N * N * sizeof(float));
    float *saida  = (float*) malloc(N * N * sizeof(float));
    float K[3][3];

    if (!imagem || !saida) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Inicializações
    inicializa_imagem(imagem, N);
    inicializa_kernel(K);

    // Medição do tempo da convolução
    double inicio = omp_get_wtime();
    convolucao2D(imagem, saida, K, N);
    double fim = omp_get_wtime();

    printf("Tempo total (sequencial): %.4f segundos\n", fim - inicio);
    printf("Saída exemplo (pixel central): %.2f\n", saida[(N/2) * N + (N/2)]);

    // Liberação de memória
    free(imagem);
    free(saida);

    return 0;
}
