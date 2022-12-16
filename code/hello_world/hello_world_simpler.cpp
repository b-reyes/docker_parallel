#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    // initialize MPI 
    MPI_Init(&argc , &argv);

    // Get the rank of the running process
    int my_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    // print the rank
    std::cout << " my_rank: " << my_rank << std::endl;

    //finalize MPI
    MPI_Finalize();

    return 0;
}