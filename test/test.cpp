#include <iostream>
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request;
    MPI_Status status;
    unsigned int N = atoi(argv[1]); //длина массива
    unsigned int n = atoi(argv[2]); //количество блоков для передачи (если N делится на n)

    double start = MPI_Wtime(); 

    if (rank == 0) {
        int* a = new int[N];

        //получение массива по блокам
        for (int i = 0; i < n; ++i)
            MPI_Recv(a + i*N/n, N/n, MPI_INT, 1, 111, MPI_COMM_WORLD, &status);
        //получаем оставшуюся часть, если N не делится на n
        MPI_Recv(a + n*N/n, N%n, MPI_INT, 1, 111, MPI_COMM_WORLD, &status);

        delete[] a;
    }
    if (rank == 1) {
        //инициализация массива
        int* a = new int[N];
        for (int i = 0; i < N; ++i)
            a[i] = i;

        //пересылка массива по блокам
        for (int i = 0; i < n; ++i)
            MPI_Send(a + i*N/n, N/n, MPI_INT, 0, 111, MPI_COMM_WORLD);
        //пересылка оставшейся части, если N не делится на n
        MPI_Send(a + n*N/n, N%n, MPI_INT, 0, 111, MPI_COMM_WORLD);

        delete[] a;
    }

    double end = MPI_Wtime(); 
    if (rank == 0)
        printf("N = %u. Number of blocks: %u. Time: %f s\n", N, n, end - start);

    MPI_Finalize();
}