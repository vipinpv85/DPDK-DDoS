#include <mpi.h>
#include <stdio.h>
int main(int argc, char ** argv)
{
 int rank;
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 if (rank == 0) {
                int data[100];
                data[0] = 12;

                int size = 0;
                MPI_Comm_size(MPI_COMM_WORLD, &size);
                printf(" total count %u\n", size);

                for (int i = 0; i < size - 1; i++)
                        if (rank != i) {
                        printf(" sending data from rank (%u)!!!\n", rank);
                        MPI_Send(data, 100, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
        }else {
                int data[100];
                printf(" recieveing %u from rank %u!!!\n", data[0], rank);
                MPI_Recv(data, 100, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf(" recieved %u iby rank %u!!!\n", data[0], rank);
        }
 MPI_Finalize();

 return 0;
}
