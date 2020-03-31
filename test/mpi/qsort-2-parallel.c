#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char ** argv)
{
 int rank;
 int a[1000], b[500];

 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    for (int i = 0; i < 1000; i++)
      a[i] = rand();

    for (int i = 0, count = 0; i < 10; i++) {
      if (i % 4 == 0)
        printf("\n %d. ", count++);
      printf(" %d | ", a[i]);
      }
  }

  MPI_Status status;
    if (rank == 0) {
      MPI_Send(&a[500], 500, MPI_INT, 1, 0, MPI_COMM_WORLD);
      //for (int i = 0; i < 500; i++)
      //printf(" rank (%u) - a[%d] is (%d)\n", rank, i, a[i]);

      qsort(a, 500, sizeof(int), cmpfunc);
      MPI_Recv(b, 500, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      /* Serial: Merge array b and sorted part of array a */
    } else if (rank == 1) {
      MPI_Recv(b, 500, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      //for (int i = 500; i < 1000; i++)
      //printf(" rank (%u) - a[%d] is (%d)\n", rank, i, a[i]);

      qsort(b, 500, sizeof(int), cmpfunc);
      MPI_Send(b, 500, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  MPI_Finalize();

  if (rank == 0) {
    for (int i = 0, count = 0; i < 10; i++) {
      if (i % 4 == 0)
        printf("\n %d. ", count++);
      printf(" %d | ", a[i]);
    }
  }

  return 0;
}
