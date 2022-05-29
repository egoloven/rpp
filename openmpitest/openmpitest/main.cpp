//
//  main.cpp
//  openmpitest
//
//  Created by whisper- on 26.05.2022.
//

#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <cmath>
#include <string>

bool isSquare(int num) {
    int lower_x = floor(sqrt(num));
    return lower_x * lower_x == num;
}

int main(int argc, char **argv) {

    int rank, n_rank;
    unsigned long long length, start, total;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
    unsigned long long iterations[n_rank];

    unsigned long long *array;

    if (rank == 0) {
        std::cout << "find squares in array\n";

        std::cout << "input length of an array\n";
        bool breakpoint = true;
        while (breakpoint) {
            std::cin >> length;
            if (length < n_rank) { std::cout << "length should be greater than " << n_rank << "\n"; }
            else { breakpoint = false; }
        }

        array = new unsigned long long[length];
        for (size_t i = 0; i < length; ++i) {
            std::cin >> array[i];
        }

        unsigned long long miniter = length / n_rank;
        unsigned long long iteradd = length % n_rank;
        for (int i = 0; i < n_rank; ++i) {
            iterations[i] = miniter;
            if (iteradd) {
                iterations[i]++;
                iteradd--;
            }
        }
    }
    MPI_Bcast(&length, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        array = new unsigned long long[length];
    }
    MPI_Bcast(array, length, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Scatter(iterations, 1, MPI_UNSIGNED_LONG_LONG, &start, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    unsigned long long counter = 0;
    for (unsigned long long i = 0; i < start; ++i) {
        if (isSquare(array[i * n_rank + rank])) {
//            std::cout << array[i * n_rank + rank] << " ";
            counter++;
        }
    }
    MPI_Reduce(&counter, &total, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        double t2 = MPI_Wtime();
        std::cout << "\ntotal: " << total << "\n";
        std::cout << "time spent: " << (t2 - t1) << "\n";
    }

    MPI_Finalize();

    return 0;
}
