#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int T = atoi(argv[1]);
    int N = atoi(argv[2]);
    omp_set_num_threads(T);
    double** a = new double*[N];
    double* b = new double[N];
    double* c = new double[N];

    double start = omp_get_wtime();
    #pragma omp parallel shared(N, a, b, c)
	{
		//initialization
		#pragma omp for
		for (int i = 0; i < N; ++i) {
			a[i] = new double[N];
			for (int j = 0; j < N; ++j) {
				a[i][j] = cos(i * j + i + j);
			}
			b[i] = sin(i);
		}

		//matrix-vector product
		#pragma omp for
		for (int i = 0; i < N; ++i) {
			c[i] = 0.0;
			for (int j = 0; j < N; ++j) {
				c[i] += a[i][j] * b[j];
			}
		}
	}
    double time = omp_get_wtime() - start;
    printf("N = %d. T = %d. Wall time(s): %f \n", N, T, time);
	
	for (int i = 0; i < N; ++i)
		delete[] a[i];
	delete[] a;
	delete[] b;
	delete[] c;
}