#include <iostream>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <sstream>
#include<tuple>

using namespace std; 

// This script will sum the values 1 to N in parallel (no arrays). 
// This will be done as follows. First, split up the sum amongst 
// the processors in a round robin fashion. Second, have each process 
// compute their local sum. Last, use MPI_Reduce to gather each local 
// sum and compute a final sum.  


// function declarations 
vector<int> get_master_data(int N, int my_rank);
tuple<int, int> initialize_MPI(int argc, char **argv); 
void print_rank_vector(vector<int>& vec, int position, int my_rank);
vector<int> scatter_data(vector<int>& data, int my_rank, int num_proc); 

int main(int argc, char **argv)
{

    auto [num_proc, my_rank] = initialize_MPI(argc, argv);

    // get the final number in the sum from command line 
    int N = stoi(argv[1]);

    // construct data that will be scattered 
    auto master_data = get_master_data(N, my_rank);



    print_rank_vector(data, 2, my_rank); 


    // MPI_Barrier(MPI_COMM_WORLD); // blocking call 

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

tuple<int, int> initialize_MPI(int argc, char **argv){
    /**
     * Initializes MPI and returns the number of
     * processes and the rank of the running process.
    */

    // initialize MPI 
    MPI_Init(&argc , &argv);

    // Get the total number of processes
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // Get the rank of the running process
    int my_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    return {num_proc,my_rank};
}

void print_rank_vector(vector<int>& vec, int position, int my_rank){
    /**
     * A simple helper function that will print the rank of the
     * running process and then print the input vector up until 
     * the given position. 
    */

    for (int j = 0; j < position; j++)
    {
        if (j == 0){
            cout << "my_rank: " << my_rank << endl;
        }
        cout << vec[j] << " "; 
        if (j == position - 1){
            cout << endl; 
        }
    }

}


vector<int> get_master_data(int N, int my_rank){
    /**
     * Constructs an array filled with values 1 to N,
     * if my_rank is zero, else creates an empty array,
    */

    // vector that will hold data on root process
    vector<int> master_data;

    // create an array on the root process to send to all othe processes 
    if (my_rank == 0){ 

        // resize master_data
        master_data.resize(N);

        // fill data with values 1 to N
        for (int i=0; i<N; i++){
            master_data[i] = i + 1; 
        }
    }

    return master_data; 
}

vector<int> scatter_data(vector<int>& input_data, int my_rank, int num_proc){
    /**
     * A function that scatters the input data to all 
     * processes in MPI_COMM_WORLD.
    */

    

    // create vector that will hold the scatter data 
    vector<int> local_data(2);

    MPI_Scatter( input_data.data() , 2, MPI_INT, local_data.data() , 2, MPI_INT , 0, MPI_COMM_WORLD);

    return local_data;
}