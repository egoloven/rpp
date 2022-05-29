//
//  main.cpp
//  openmp
//
//  Created by whisper- on 29.05.2022.
//

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <string>
#include <chrono>
#include <omp.h>


bool isSquare(unsigned long long num) {
    int lower_x = floor(sqrt(num));
    return lower_x * lower_x == num;
}

int main(int argc, const char * argv[]) {
    omp_set_num_threads(2);

    unsigned long long length, start, total;


    unsigned long long *array;

    std::cout << "find squares in array\n";

    std::cout << "input length of an array\n";
    bool breakpoint = true;
    while (breakpoint) {
        std::cin >> length;
        if (length < 0) { std::cout << "pass"; }
        else { breakpoint = false; }
    }

    array = new unsigned long long[length];
    for (size_t i = 0; i < length; ++i) {
        std::cin >> array[i];
    }
    unsigned long long counter = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (unsigned long long i = 0; i < length; ++i) {
        if (isSquare(array[i])) {
            counter++;
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\ntotal: " << counter << "\n";
    double sec = 1000000000;
    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "time: " << time / sec << "\n";



    return 0;
}
