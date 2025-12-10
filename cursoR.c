
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Atualiza a malha usando a equação de onda discreta 2D
// u_next = 2*u_curr - u_prev + alpha * Laplaciano(u_curr)
void wave2D_step(double* prev, double* curr, double* next, int N, double alpha) {
    #pragma omp parallel for  schedule(runtime) num_threads(4) 
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {

            int idx = i * N + j;

            double lap =
                curr[(i-1)*N + j] +
                curr[(i+1)*N + j] +
                curr[i*N + (j-1)] +
                curr[i*N + (j+1)] -
                4.0 * curr[idx];

            next[idx] = 2.0 * curr[idx] - prev[idx] + alpha * lap;
        }
    }

    // bordas fixas em zero (condição de contorno simples)
    for (int i = 0; i < N; i++) {
        next[i * N + 0]     = 0.0;
        next[i * N + (N-1)] = 0.0;
        next[0 * N + i]     = 0.0;
        next[(N-1) * N + i] = 0.0;
    }
}

int main() {
    const int N = 1000;
    const int T = 500;
    const double alpha = 0.2;

    double* prev = (double*) malloc(N * N * sizeof(double));
    double* curr = (double*) malloc(N * N * sizeof(double));
    double* next = (double*) malloc(N * N * sizeof(double));

    if (!prev || !curr || !next) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        return 1;
    }

    // inicializa malha
    for (int i = 0; i < N * N; i++) {
        prev[i] = 0.0;
        curr[i] = 0.0;
        next[i] = 0.0;
    }

    // perturbação inicial no centro (deslocamento inicial)
    int centro = N / 2;
    curr[centro * N + centro] = 1.0;
    prev[centro * N + centro] = 1.0;

    // abre arquivo para salvar a evolução da linha central
    FILE* f = fopen("wave2d_evolution.txt", "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo para escrita\n");
        return 1;
    }

    double t0 = omp_get_wtime();

    // registra estado inicial (passo 0) na linha central
    for (int j = 0; j < N; j++) {
        double val = curr[centro * N + j];
        fprintf(f, "%f ", val);
    }
    fprintf(f, "\n");

    // evolução no tempo
    for (int passo = 1; passo < T; passo++) {

        wave2D_step(prev, curr, next, N, alpha);

        // roda os ponteiros: prev <- curr, curr <- next
        double* tmp = prev;
        prev = curr;
        curr = next;
        next = tmp;

        // grava a linha central de curr
        for (int j = 0; j < N; j++) {
            double val = curr[centro * N + j];
            fprintf(f, "%f ", val);
        }
        fprintf(f, "\n");
    }

    double t1 = omp_get_wtime();

    fclose(f);

    printf("Simulacao de onda 2D concluida em %f segundos.\n", t1 - t0);
    printf("Dados salvos em wave2d_evolution.txt (T linhas x N colunas).\n");

    free(prev);
    free(curr);
    free(next);

    return 0;
}

// compile: gcc -O2 -fopenmp sudoku_check.c -o sudoku_check
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

static inline int readInt() {
    int c = getchar_unlocked();
    while (c != '-' && (c < '0' || c > '9')) c = getchar_unlocked();
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar_unlocked(); }
    int x = 0;
    for (; c >= '0' && c <= '9'; c = getchar_unlocked()) x = x * 10 + (c - '0');
    return x * sign;
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    // quick sanity on N
    if (N < 1) { printf("INVALID\n"); return 0; }

    // read rest of line after scanf (for consistent use of getchar_unlocked)
    int ch = getchar_unlocked();
    (void)ch;

    // allocate grid as linear array
    long long NN = (long long)N * (long long)N;
    int *grid = malloc(sizeof(int) * NN);
    if (!grid) { fprintf(stderr, "Mem alloc failed\n"); return 0; }

    // fast read all N*N values
    for (long long i = 0; i < NN; ++i) {
        grid[i] = readInt();
    }

    // check sqrt(N) is integer (subblock size)
    int k = (int) llround(sqrt((double)N));
    if ((long long)k * k != N) {
        printf("INVALID\n");
        free(grid);
        return 0;
    }

    volatile int valid = 1; // flag visible across threads

    // Number of threads default (OpenMP decides) — can set OMP_NUM_THREADS
    // 1) check rows
    #pragma omp parallel
    {
        char *seen = malloc((size_t)(N + 1));
        if (!seen) { /* if malloc fails, set invalid and exit */ 
            #pragma omp atomic write
            valid = 0;
        }

        #pragma omp for schedule(static)
        for (int i = 0; i < N; ++i) {
            if (!valid) continue;
            memset(seen, 0, N + 1);
            long long base = (long long)i * N;
            for (int j = 0; j < N; ++j) {
                int v = grid[base + j];
                if (v < 1 || v > N || seen[v]) {
                    #pragma omp atomic write
                    valid = 0;
                    break;
                }
                seen[v] = 1;
            }
        }

        free(seen);
    }

    if (!valid) { printf("INVALID\n"); free(grid); return 0; }

    // 2) check columns
    #pragma omp parallel
    {
        char *seen = malloc((size_t)(N + 1));
        if (!seen) { #pragma omp atomic write valid = 0; }

        #pragma omp for schedule(static)
        for (int j = 0; j < N; ++j) {
            if (!valid) continue;
            memset(seen, 0, N + 1);
            for (int i = 0; i < N; ++i) {
                int v = grid[(long long)i * N + j];
                if (v < 1 || v > N || seen[v]) {
                    #pragma omp atomic write
                    valid = 0;
                    break;
                }
                seen[v] = 1;
            }
        }
        free(seen);
    }

    if (!valid) { printf("INVALID\n"); free(grid); return 0; }

    // 3) check k x k blocks
    #pragma omp parallel
    {
        char *seen = malloc((size_t)(N + 1));
        if (!seen) { #pragma omp atomic write valid = 0; }

        #pragma omp for collapse(2) schedule(static)
        for (int brow = 0; brow < k; ++brow) {
            for (int bcol = 0; bcol < k; ++bcol) {
                if (!valid) continue;
                memset(seen, 0, N + 1);
                int r0 = brow * k;
                int c0 = bcol * k;
                for (int dr = 0; dr < k; ++dr) {
                    long long base = (long long)(r0 + dr) * N + c0;
                    for (int dc = 0; dc < k; ++dc) {
                        int v = grid[base + dc];
                        if (v < 1 || v > N || seen[v]) {
                            #pragma omp atomic write
                            valid = 0;
                            goto block_done;
                        }
                        seen[v] = 1;
                    }
                }
                block_done: ;
            }
        }
        free(seen);
    }

    printf(valid ? "VALID\n" : "INVALID\n");
    free(grid);
    return 0;
}
