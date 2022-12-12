#include <iostream>
#include <mpi.h>
#include <cmath>
#include <numeric>
#include <sstream>
#include<tuple>
#include <Eigen/Dense>

using namespace std; 

using Eigen::VectorXi; 

/*
This script will first construct a vector filled with 1 to N 
on the master core. Second, the full constructed vector will be 
scattered to all cores. Next, each core will add 5 to their 
local vector. Last, each local vector will be gathered and 
provided to the master core. 
*/

// function declarations 
VectorXi get_master_vector(int N, int my_rank);
tuple<int, int> initialize_MPI(int argc, char **argv); 
void print_rank_vector(VectorXi& vec, int my_rank);
VectorXi scatter_vector(VectorXi& vec, int my_rank, int num_proc); 
void add_n(VectorXi& vec, int n);
VectorXi gather_vectors(VectorXi& vec, int my_rank, int num_proc); 

int main(int argc, char **argv)
{

    auto [num_proc, my_rank] = initialize_MPI(argc, argv);

    // get the final number in the sum from command line 
    int N = stoi(argv[1]);

    // flag to say what to print
    int print_flag = stoi(argv[2]);

    // construct vector that will be scattered 
    auto master_vec = get_master_vector(N, my_rank);

    MPI_Barrier(MPI_COMM_WORLD); // blocking call
    double start_time = MPI_Wtime();

    // scatter master_vec to each process 
    auto local_vec = scatter_vector(master_vec, my_rank, num_proc); 

    // have each process add 5 to their local vector 
    add_n(local_vec, 5);

    // collect all local vectors and give them to the master core 
    auto full_vector = gather_vectors(local_vec, my_rank, num_proc);

    MPI_Barrier(MPI_COMM_WORLD); // blocking call
    double end_time = MPI_Wtime();

    // display full gathered vector 
    if (my_rank == 0){

        if (print_flag == 0){
            cout << "Final vector: " << full_vector.transpose() << endl; 
            cout << "Elapsed time: " << end_time - start_time << endl; 
        }else{
            cout << end_time - start_time << endl; 
        }
    }

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

void print_rank_vector(VectorXi& vec, int my_rank){
    /**
     * A simple helper function that will print the rank of the
     * running process and then print the input vector as a row vector.
    */

   cout << "my_rank: " << my_rank << ", vector: " << vec.transpose() << endl;

}


VectorXi get_master_vector(int N, int my_rank){
    /**
     * Constructs a vector filled with values 1 to N,
     * if my_rank is zero, else creates an empty vector,
    */

    // vector that will hold data on root process
    VectorXi master_vec;

    // create a vector on the root process to send to all othe processes 
    if (my_rank == 0){ 

        // resize master_vec
        master_vec.resize(N);

        // fill vector with values 1 to N
        for (int i=0; i<N; i++){
            master_vec[i] = i + 1; 
        }
    }

    return master_vec; 
}

VectorXi scatter_vector(VectorXi& input_vec, int my_rank, int num_proc){
    /**
     * A function that scatters the input vector to all 
     * processes in MPI_COMM_WORLD.
    */

    // get remainder of input_vec size / num_proc and broadcast it 
    int r; 
    if (my_rank == 0){
        r = input_vec.size() % num_proc; 
    }
    MPI_Bcast(&r, 1 , MPI_INT , 0, MPI_COMM_WORLD);

    // initialize local vector portion
    VectorXi local_vec; 
    
    if (r != 0){
        
        if (my_rank == 0){
            cout << "Cannot evenly scatter vector!" << endl;     
        }

        MPI_Abort( MPI_COMM_WORLD , 72); 

        return local_vec; 

    }else{

        // get the number of elements each process should get
        int num_elem;
        if (my_rank == 0){
            num_elem = input_vec.size() / num_proc; 
        }
        MPI_Bcast(&num_elem, 1 , MPI_INT , 0, MPI_COMM_WORLD);

        // create vector that will hold the scattered data 
        local_vec.resize(num_elem);

        MPI_Scatter(input_vec.data() , num_elem, MPI_INT, local_vec.data() , num_elem, MPI_INT , 0, MPI_COMM_WORLD);

        return local_vec;
    }
}

void add_n(VectorXi& vec, int n){
    /**
     * Adds the input n to each element in vec
    */

    for (int i=0; i<vec.size(); i++){
        vec[i] += n;
    }
}

VectorXi gather_vectors(VectorXi& vec, int my_rank, int num_proc){
    /**
     * Gathers each processes local vector and provides it to the 
     * master core.
    */

    // initialize the final gathered vector and its size
    VectorXi full_vec;
    int full_vec_size; 

    // get local vector sizes
    int vec_size = vec.size();

    // determine the final gathered vector size
    MPI_Reduce(&vec_size , &full_vec_size , 1 , MPI_INT, MPI_SUM , 0 , MPI_COMM_WORLD);

    // resize the final vector so that Gather can correctly fill it 
    if (my_rank == 0){
        full_vec.resize(full_vec_size);
    }

    MPI_Gather( vec.data() , vec_size , MPI_INT , full_vec.data(), vec_size , MPI_INT , 0 , MPI_COMM_WORLD);

    return full_vec;
}