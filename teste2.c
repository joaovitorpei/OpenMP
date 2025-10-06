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


// 

