// #define BILLION 1000000000.0
//  caso o arquivo esteja na extensão .c, adicionar isso ao topo: #define _POSIX_C_SOURCE 199309L para que o CLOCK_REALTIME seja definido

#include <time.h>
#include <stdio.h>

// If the time in seconds suffices, time(2) can be used; gettimeofday(2) gives microseconds; clock_gettime(2) gives nanoseconds but is not as widely available.

/*
    struct timespec {
        time_t tv_sec;   // segundos
        long tv_nsec;   // nanosegundos
    };
*/

struct timespec start_clock()
{
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return start;
}

void end_clock(struct timespec start)
{
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // time_spent = end - start
    int time_spent_in_msec = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000000;

    printf("\n______________________________________________________________________\n");
    printf("Tempo de execução: %dms\n", time_spent_in_msec);
}

int get_time_in_seconds(struct timespec time)
{
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // time_spent = end - start
    int time_spent_in_sec = (end.tv_sec - time.tv_sec) * 1000000 + (end.tv_nsec - time.tv_nsec) / 100000000;

    return time_spent_in_sec;
}

/*
The gettimeofday() function returns the wall clock time elapsed since the Epoch and store it in the timeval structure, expressed as seconds and microseconds.

It is defined in the <sys/time.h> header file and takes two arguments – the first argument is a reference to the timeval structure, and the second argument is a null pointer. The timeval structure is declared as follows by the <time.h> header:

struct timeval {
    long tv_sec;    // seconds
    long tv_usec;   // microsseconds
};


The following code demonstrates the usage of gettimeofday() by measuring the wall clock time:

#include <stdio.h>
#include <sys/time.h>   // for gettimeofday()
#include <unistd.h>     // for sleep()

// main function to find the execution time of a C program
int main()
{
    struct timeval start, end;

    gettimeofday(&start, NULL);

    // do some stuff here
    sleep(5);

    gettimeofday(&end, NULL);

    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

    printf("The elapsed time is %d seconds and %d micros\n", seconds, micros);

    return 0;
}
Download  Run Code

Output (may vary):

The elapsed time is 5 seconds and 5000147 micros


This function is supported by GCC compilers and might not work on Windows.
*/