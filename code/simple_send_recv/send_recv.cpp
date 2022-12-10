#include <iostream>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <sstream>

using namespace std; 

// This script will sum the values 1 to N in parallel (no arrays)

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

    // get the final number in the sum from command line 
    long int N = stoi(argv[1]);
    
    // flag to say what to print
    int print_flag = stoi(argv[2]);

    // initial number of tasks per processes 
    long int init_tasks = floor(N/num_proc);

    // the number of left over tasks 
    long int r_tasks = N % num_proc;

    // determine how many tasks each process will get 
    vector<long int> tasks(num_proc, init_tasks);

    // distribute the remainder of tasks 
    for (int i=0; i < r_tasks; i++){
        tasks[i] += 1;
    }

    // obtain the starting natural number for each process 
    long int start;
    if (my_rank == 0){
        start = 1; 
    }else{
        start = accumulate(tasks.begin(), tasks.begin() + my_rank, 0) + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // sum up each processes portion of sum 
    long int local_sum = 0;
    for (int i=0; i<tasks[my_rank]; i++){
        local_sum += start + 1*i;
    }

    long int final_sum = local_sum; // final sum of 1 to N

    // have each process send their local sum to master rank 
    if (my_rank == 0){

        long int rank_local_sum; // variable that will temporarily hold each rank's local sum

        // get each rank's local sum and add it to final_sum
        for (int i=1; i<num_proc; i++){
            MPI_Status status; 
            MPI_Recv( &rank_local_sum , 1 , MPI_LONG , i , i , MPI_COMM_WORLD , &status);
            final_sum += rank_local_sum;
            }
    }else{
        MPI_Send( &local_sum, 1, MPI_LONG , 0 , my_rank , MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0){
        double elapsed_time = MPI_Wtime() - start_time; 

        if (print_flag == 0){
            cout << "Elapsed time: " << elapsed_time << endl; 
            cout << "Calculated value = " << final_sum << endl;
            cout << "True value = " << (N*(N+1))/2 << endl;
        }else{
            cout << elapsed_time << endl;
        }
    }

    //finalize MPI
    MPI_Finalize();

    return 0;
}