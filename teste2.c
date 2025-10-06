//  #include <stdio.h>
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


#include <stdio.h>
#include <omp.h> // Biblioteca para paralelização

// Definição das dimensões da matriz 
// (1000 linhas e 1000 colunas)
#define N 1000 // Linhas
#define M 1000 // Colunas

int main(void) {
    
    // Declaração dos vetores que representam as matrizes A, B e C.
    // O tamanho é N*M (1 milhão de elementos).
    // O 'static' é usado para alocar essa grande quantidade de memória fora da pilha (stack).
    static double A[N*M], B[N*M], C[N*M];

    // --- 1. Inicialização das Matrizes ---
    // Percorre o vetor 1D completo (do índice 0 até N*M - 1)
    for (int i = 0; i < N*M; i++) {
        A[i] = 1.0;
        B[i] = 2.0;
        // A matriz C não precisa ser inicializada, pois será preenchida
    }

    // --- 2. Cálculo da Soma Paralela (C = A + B) ---
    
    // #pragma omp parallel for:
    // Esta diretiva do OpenMP divide o loop (laço 'for' da linha) entre 
    // os núcleos do processador para acelerar o cálculo.
    #pragma omp parallel for
    for (int i = 0; i < N; i++) { 
        // i é o índice da LINHA (de 0 a N-1)
        
        for (int j = 0; j < M; j++) { 
            // j é o índice da COLUNA (de 0 a M-1)
            
            // FÓRMULA DE LINEARIZAÇÃO: C[i*M + j]
            // Converte a posição (linha i, coluna j) em um único índice 1D.
            // i*M: Calcula o deslocamento para o início da linha 'i'.
            // +j: Adiciona o deslocamento da coluna 'j' dentro daquela linha.
            
            C[i*M + j] = A[i*M + j] + B[i*M + j];
        }
    }

    // --- 3. Exibição do Resultado ---

    // Imprime o primeiro elemento (índice 0) e o último elemento (índice N*M-1).
    // O resultado esperado é 1.0 + 2.0 = 3.0 para qualquer elemento.
    printf("C[0]=%.1f  C[%d]=%.1f\n", C[0], N*M-1, C[N*M-1]);
    
    return 0;
}