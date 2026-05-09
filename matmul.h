#pragma once

void matmul(int M, int N, int K,
            const double *A, int lda,
            const double *B, int ldb,
            double *C, int ldc);
