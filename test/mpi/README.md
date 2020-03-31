# MPI examples
explore use of MPI based packet filtering in user space.

## build
- `mpicc [file]`

## execution
 - single Process: `./[executable]`
 - Multiple Porcess: `mpiexec -np [process-count] [executable]`

# BAISC:
- MPI_INIT – initialize the MPI library (must be the first routine called)
- MPI_COMM_SIZE - get the size of a communicator 
- MPI_COMM_RANK – get the rank of the calling process in the communicator
- MPI_SEND – send a message to another process
- MPI_RECV – send a message to another process
- MPI_FINALIZE – clean up all MPI state (must be the last MPI function called by a process)


# Reference: `https://htor.inf.ethz.ch/teaching/mpi_tutorials/ppopp13/2013-02-24-ppopp-mpi-basic.pdf`


