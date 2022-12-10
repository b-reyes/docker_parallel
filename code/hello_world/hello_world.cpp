#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    // initialize MPI 
    MPI_Init(&argc , &argv);

    // Get the total number of processes
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // Get the rank of the running process
    int my_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    // print total number of processes and have every process say hello
    std::cout << "Number of processes:" << num_proc << " my_rank: " << my_rank << std::endl;

    //finalize MPI
    MPI_Finalize();

    return 0;
}