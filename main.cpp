#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <chrono>

#include "matmul.h"

static void ref_matmul(int M, int N, int K,
                       const double *A, int lda,
                       const double *B, int ldb,
                       double *C, int ldc)
{
    for (int i = 0; i < M; ++i)
        for (int k = 0; k < K; ++k)
            for (int j = 0; j < N; ++j)
                C[i * ldc + j] += A[i * lda + k] * B[k * ldb + j];
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "usage: %s N M K\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int K = atoi(argv[3]);

    double *A = (double *)calloc(M * K, sizeof(double));
    double *B = (double *)calloc(K * N, sizeof(double));
    double *C = (double *)calloc(M * N, sizeof(double));
    double *C_ref = (double *)calloc(M * N, sizeof(double));

    srand(42);
    for (int i = 0; i < M * K; ++i) A[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < K * N; ++i) B[i] = (double)rand() / RAND_MAX;

    // Reference
    auto t0 = std::chrono::high_resolution_clock::now();
    ref_matmul(M, N, K, A, K, B, N, C_ref, N);
    auto t1 = std::chrono::high_resolution_clock::now();
    double ref_s = std::chrono::duration<double>(t1 - t0).count();

    // Reinitialize to avoid warm-cache bias from ref_matmul
    srand(42);
    for (int i = 0; i < M * K; ++i) A[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < K * N; ++i) B[i] = (double)rand() / RAND_MAX;

    // Implementation under test
    auto t2 = std::chrono::high_resolution_clock::now();
    matmul(M, N, K, A, K, B, N, C, N);
    auto t3 = std::chrono::high_resolution_clock::now();
    double mut_s = std::chrono::duration<double>(t3 - t2).count();

    // Correctness check
    double max_err = 0.0;
    for (int i = 0; i < M * N; ++i) {
        double err = fabs(C[i] - C_ref[i]);
        if (err > max_err) max_err = err;
    }

    if (max_err > 1e-9) {
        printf("{\"status\": \"ERROR\", \"reason\": \"failed correctness check, %e\"}\n", max_err);
    } else {
        printf("{\"status\": \"SUCCESS\", \"runtime\": %e, \"speedup\": %f}\n", mut_s, ref_s / mut_s);
    }

    free(A);
    free(B);
    free(C);
    free(C_ref);
    return 0;
}
