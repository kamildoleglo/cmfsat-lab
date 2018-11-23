#include <stdio.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <time.h>

#define N_OF_RUNS 10

#define RUSAGE_DIFF(r1, r2) \
    ((double) ((r1).tv_sec - (r2).tv_sec) + (double) ((r1).tv_nsec - (r2).tv_nsec)/(double)1E9)

gsl_vector* random_vector(size_t);
gsl_matrix* random_matrix(size_t);
double* ddot_test(size_t);
double* dgemv_test(size_t);
void test_suite(size_t, size_t, size_t, const char*);

int main() {
    srand48((unsigned int)time(NULL));
    const char filename[] = "results.csv";
    test_suite(1, 15000, 1000, filename);
    return 0;
}

void test_suite(const size_t from, const size_t to, const size_t step, const char* filename){
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening file, exiting \r\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "LEVEL,SIZE,TIME\r\n");

    double* ddot_result, * dgemv_result;

    for(size_t i = from; i < to; i += step){
        for(int j = 0; j < N_OF_RUNS; j++) {
            ddot_result = ddot_test(i);
            dgemv_result = dgemv_test(i);
            fprintf(file, "%d,%d,%lf\r\n", 1, (int) i, *ddot_result);
            fprintf(file, "%d,%d,%lf\r\n", 2, (int) i, *dgemv_result);
            free(ddot_result);
            free(dgemv_result);
        }
    }

    fclose(file);
}

gsl_vector* random_vector(const size_t size) {
    gsl_vector* a = gsl_vector_calloc(size);
    for(int i = 0; i < size; i++)
        gsl_vector_set(a, (size_t)i, drand48()*10);
    return a;
}


gsl_matrix* random_matrix(const size_t size) {
    gsl_matrix* a = gsl_matrix_calloc(size, size);
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            gsl_matrix_set(a, (size_t)i, (size_t)j, drand48()*10);
    return a;
}

double* ddot_test(const size_t size) {
    gsl_vector* v_1 = random_vector(size);
    gsl_vector* v_2 = random_vector(size);
    printf("Multiplying 2 vectors of size %d \r\n", (int)size);
    double * result = calloc(1, sizeof(double));
    struct timespec real_start, real_end;
    clock_gettime(CLOCK_REALTIME, &real_start);
    for(int i = 0; i < N_OF_RUNS; i++) {
        gsl_blas_ddot(v_1, v_2, result);
    }
    clock_gettime(CLOCK_REALTIME, &real_end);
    double* time = malloc(sizeof(double));
    *time = RUSAGE_DIFF(real_end, real_start)/10;
    printf("Real time = %lf \r\n", *time);
    free(result);
    gsl_vector_free(v_1); gsl_vector_free(v_2);
    return time;
}

double* dgemv_test(const size_t size) {
    gsl_vector* v_1 = random_vector(size);
    gsl_vector* v_2 = random_vector(size);
    gsl_matrix* m_1 = random_matrix(size);
    printf("Multiplying vector times matrix of size %d \r\n", (int)size);

    struct timespec real_start, real_end;
    clock_gettime(CLOCK_REALTIME, &real_start);

    gsl_blas_dgemv(CblasNoTrans, 1, m_1, v_1, 0, v_2);

    clock_gettime(CLOCK_REALTIME, &real_end);

    double* time = malloc(sizeof(double));
    *time = RUSAGE_DIFF(real_end, real_start);
    printf("Real time = %lf \r\n", *time);
    gsl_vector_free(v_1); gsl_matrix_free(m_1);
    return time;
}
