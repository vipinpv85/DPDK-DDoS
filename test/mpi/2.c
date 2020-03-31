#include "mpi.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main( argc, argv )
int argc;
char **argv;
{
        unsigned cpu = 0;
        int rank, size;

        MPI_Init( &argc, &argv );

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        if (syscall(SYS_getcpu, &cpu, NULL, NULL) != -1)
                printf( "CPU (%u) with rank:size (%u:%u)\n", cpu, rank, size);

        while(1);
        MPI_Finalize();

        return 0;
}
