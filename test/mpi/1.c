#include "mpi.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main( argc, argv )
int argc;
char **argv;
{
        unsigned cpu = 0;

        MPI_Init( &argc, &argv );
        if (syscall(SYS_getcpu, &cpu, NULL, NULL) != -1)
                printf( "Hello world from CPU %u\n", cpu);

        while(1);
        MPI_Finalize();

        return 0;
}
