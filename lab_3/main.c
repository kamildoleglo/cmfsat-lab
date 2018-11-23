#include <stdio.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <time.h>

#define N_OF_RUNS 10

#define RUSAGE_DIFF(r1, r2) \
    ((double) ((r1).tv_sec - (r2).tv_sec) + (double) ((r1).tv_nsec - (r2).tv_nsec)/(double)1E9)


gsl_matrix* random_matrix(size_t);
inline gsl_matrix* zero_matrix(const size_t size);

double* naive_test(size_t);
double* better_test(size_t);
double* dgemm_test(size_t);

void test_suite(size_t, size_t, size_t, const char*);

int main() {
    srand48((unsigned int)time(NULL));
    const char filename[] = "results_unoptimized.csv";
    test_suite(100, 1000, 100, filename);
    return 0;
}

void test_suite(const size_t from, const size_t to, const size_t step, const char* filename){
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening file, exiting \r\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "METHOD,SIZE,TIME\r\n");

    double* naive_result, * dgemm_result, * better_result;

    for(size_t i = from; i <= to; i += step){
        for(int j = 0; j < N_OF_RUNS; j++) {
            naive_result = naive_test(i);
            better_result = better_test(i);
            dgemm_result = dgemm_test(i);
            fprintf(file, "Naive,%d,%lf\r\n", (int) i, *naive_result);
            fprintf(file, "Better,%d,%lf\r\n", (int) i, *better_result);
            fprintf(file, "BLAS,%d,%lf\r\n", (int) i, *dgemm_result);
            free(naive_result);
            free(better_result);
            free(dgemm_result);
        }
    }

    fclose(file);
}


gsl_matrix* random_matrix(const size_t size) {
    gsl_matrix* a = gsl_matrix_calloc(size, size);
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            gsl_matrix_set(a, (size_t)i, (size_t)j, drand48()*10);
    return a;
}

gsl_matrix* zero_matrix(const size_t size) {
    gsl_matrix* a = gsl_matrix_calloc(size, size);
    return a;
}

double** random_double_matrix(const size_t size) {
    double** a = calloc(size, sizeof(double*));
    for(int i = 0; i < size; i++)
        a[i] = calloc(size, sizeof(double));

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            a[i][j] = drand48() * 10;
    return a;
}

double** zero_double_matrix(const size_t size) {
    double** a = calloc(size, sizeof(double*));
    for(int i = 0; i < size; i++)
        a[i] = calloc(size, sizeof(double));
    return a;
}

void free_double_matrix(double** matrix, const size_t size){
    for(int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}

double* naive_test(const size_t size) {
    double** m_1 = random_double_matrix(size);
    double** m_2 = random_double_matrix(size);
    double** m_3 = zero_double_matrix(size);

    printf("Multiplying matrices of size %d using naive method\r\n", (int)size);

    struct timespec real_start, real_end;
    clock_gettime(CLOCK_REALTIME, &real_start);

    for(int k = 0; k < size; k++)
        for(int j = 0; j < size; j++)
            for(int i = 0; i < size; i++)
                m_3[i][j] += m_1[i][k] * m_2[k][j];

    clock_gettime(CLOCK_REALTIME, &real_end);

    double* time = malloc(sizeof(double));
    *time = RUSAGE_DIFF(real_end, real_start);
    printf("Real time = %lf \r\n", *time);
    free_double_matrix(m_1, size); free_double_matrix(m_2, size); free_double_matrix(m_3, size);
    return time;
}

double* better_test(const size_t size) {
    double** m_1 = random_double_matrix(size);
    double** m_2 = random_double_matrix(size);
    double** m_3 = zero_double_matrix(size);

    printf("Multiplying matrices of size %d using better method\r\n", (int)size);

    struct timespec real_start, real_end;
    clock_gettime(CLOCK_REALTIME, &real_start);

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            for(int k = 0; k < size; k++)
                m_3[i][j] += m_1[i][k] * m_2[k][j];

    clock_gettime(CLOCK_REALTIME, &real_end);

    double* time = malloc(sizeof(double));
    *time = RUSAGE_DIFF(real_end, real_start);
    printf("Real time = %lf \r\n", *time);
    free_double_matrix(m_1, size); free_double_matrix(m_2, size); free_double_matrix(m_3, size);
    return time;
}

double* dgemm_test(const size_t size) {
    gsl_matrix* m_1 = random_matrix(size);
    gsl_matrix* m_2 = random_matrix(size);
    gsl_matrix* m_3 = zero_matrix(size);

    printf("Multiplying matrices of size %d using BLAS\r\n", (int)size);

    struct timespec real_start, real_end;
    clock_gettime(CLOCK_REALTIME, &real_start);

    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, m_1, m_2, 0.0, m_3);

    clock_gettime(CLOCK_REALTIME, &real_end);

    double* time = malloc(sizeof(double));
    *time = RUSAGE_DIFF(real_end, real_start);
    printf("Real time = %lf \r\n", *time);
    gsl_matrix_free(m_1); gsl_matrix_free(m_2); gsl_matrix_free(m_3);
    return time;
}