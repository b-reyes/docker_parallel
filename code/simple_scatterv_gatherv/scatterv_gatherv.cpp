#include <iostream>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <sstream>

using namespace std; 

// This script will sum the values 1 to N in parallel (no arrays). 
// This will be done as follows. First, split up the sum amongst 
// the processors in a round robin fashion. Second, have each process 
// compute their local sum. Last, use MPI_Reduce to gather each local 
// sum and compute a final sum.  

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

    vector<int> init_data(N); // vector that will hold data on root process

    // create an array on the root process to send to all othe processes 
    if (my_rank == 0){ 

        // fill data with values 1 to N
        for (int i=0; i<N; i++){
            init_data[i] = i + 1; 
        }
    }

    vector<int> data(2);

    MPI_Barrier(MPI_COMM_WORLD); // blocking call 

    MPI_Scatter( init_data.data() , 2, MPI_INT, data.data() , 2, MPI_INT , 0, MPI_COMM_WORLD);

    for (int j = 0; j < 2; j++)
    {
        if (j == 0){
            cout << "my_rank: " << my_rank << endl;
        }
        cout << data[j] << " "; 
        if (j ==1){
            cout << endl;
        }
    }
    


    // // flag to say what to print
    // int print_flag = stoi(argv[2]);

    // // initial number of tasks per processes 
    // long int init_tasks = floor(N/num_proc);

    // // the number of left over tasks 
    // long int r_tasks = N % num_proc;

    // // determine how many tasks each process will get 
    // vector<long int> tasks(num_proc, init_tasks);

    // // distribute the remainder of tasks 
    // for (int i=0; i < r_tasks; i++){
    //     tasks[i] += 1;
    // }

    // // obtain the starting natural number for each process 
    // long int start;
    // if (my_rank == 0){
    //     start = 1; 
    // }else{
    //     start = accumulate(tasks.begin(), tasks.begin() + my_rank, 0) + 1;
    // }

    // MPI_Barrier(MPI_COMM_WORLD);
    // double start_time = MPI_Wtime();

    // // sum up each processes portion of sum 
    // long int local_sum = 0;
    // for (int i=0; i<tasks[my_rank]; i++){
    //     local_sum += start + 1*i;
    // }

    // long int final_sum; // variable to hold the final sum 

    // // Use Reduce to collect all local sum and create the final sum
    // MPI_Reduce( &local_sum , &final_sum , 1, MPI_LONG , MPI_SUM, 0 , MPI_COMM_WORLD);

    // MPI_Barrier(MPI_COMM_WORLD);

    // if (my_rank == 0){
    //     double elapsed_time = MPI_Wtime() - start_time; 

    //     if (print_flag == 0){
    //         cout << "Elapsed time: " << elapsed_time << endl; 
    //         cout << "Calculated value = " << final_sum << endl;
    //         cout << "True value = " << (N*(N+1))/2 << endl;
    //     }else{
    //         cout << elapsed_time << endl;
    //     }
    // }

    //finalize MPI
    MPI_Finalize();

    return 0;
}