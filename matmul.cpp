#include "matmul.h"

// C[M x N] = A[M x K] * B[K x N], row-major.
// Deliberately iterates in i-k-j order (wrong: stride-N access on B and C).
void matmul(int M, int N, int K,
            const double *A, int lda,
            const double *B, int ldb,
            double *C, int ldc)
{
    for (int i = 0; i < M; ++i)
        for (int k = 0; k < K; ++k)
            for (int j = 0; j < N; ++j)
                C[i * ldc + j] += A[i * lda + k] * B[k * ldb + j];
}
