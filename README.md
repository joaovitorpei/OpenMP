# OpenMP  

# formas de compilar codigo  
### linux
- gcc -fopenmp prog.c -o prog     | ./prog  
- g++ -fopenmp prog.c -o prog     | ./prog  
- gcc -3 -fopenmp prog.c prog     | ./prog  
### variaveis de ambiemte 
- OMP_NUM_THREADS=8 ./prog   
- OMP_NUM_THREADS=4 OMP_SCHEDULE="static" ./prog                                                      
- OMP_SCHEDULE="static" ./prog  
- OMP_SCHEDULE="static,1024" ./prog  
- OMP_SCHEDULE="dynamic,512" ./prog  
- OMP_SCHEDULE="guided,256" ./prog  
---
### tempo  
- omp_get_wtime() → retorna o tempo atual em segundos (como double). t1-t0  
- time ./tempo  
 real → tempo total de execução (cronômetro de parede).  
 user → tempo somado de CPU usado por todas as threads.  
 sys → tempo gasto em chamadas de sistema (I/O, etc).  
---
### diretivas para usar em soma e multiplicação de vetor
### soma 
- #pragma omp parallel for reduction(+:soma)  
  -  #pragma omp parallel for reduction(+:soma)  
    for (int i = 0; i < N; i++) {  
        soma += A[i] + B[i];  
    }  
  - Por que reduction(+:soma)?  
Cada thread mantém sua cópia local de soma, acumula ali e no final o OpenMP soma tudo em uma variável    global segura (sem race condition).  
Essa técnica é a forma correta de acumular valores compartilhados  
-  #pragma omp parallel for  
  - #pragma omp parallel for  
    for (int i = 0; i < N; i++) {  
        C[i] = A[i] * B[i];  
    }  
   - Nesse caso, não há redução, porque cada iteração escreve em um índice diferente  
- #pragma omp parallel for collapse(2)  
  - Quando você quer que o OpenMP veja os dois laços (i, j) como um único espaço de iterações, use collapse(2).  
  - útil quando: N é pequeno e M é grande, ou vice-versa  
  -#pragma omp parallel for reduction(+:soma) collapse(2)  
--- 
### multiplicação
- #pragma omp parallel for  ou #pragma omp parallel for collapse(2)
for (int i = 0; i < N; i++) {  
    for (int j = 0; j < N; j++) {  
            double soma = 0.0;    
            for (int k = 0; k < N; k++)  
                soma += A[i][k] * B[k][j];  
            C[i][j] = soma;  
        }  
---
### outra forma usando  c[i*m+j]
- for (int i = 0; i < L; i++) {  
--- for (int j = 0; j < M; j++) {  
----------int indice = i * M + j;  
-----------C[indice] = A[indice] + B[indice];  
}  }  }  

- for (int i = 0; i < N; i++) {  
    - for (int j = 0; j < N; j++) {  
        - double soma = 0.0;
    - for (int k = 0; k < N; k++) {  
        - int indice_A = i * N + k;  
        - int indice_B = k * N + j;  
        - soma += A[indice_A] * B[indice_B];  
    - int indice_C = i * N + j;  
    - C[indice_C] = soma;  

---

