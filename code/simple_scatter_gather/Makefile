CC=mpic++  # compiler for MPI c++ program
files=scatter_gather.cpp # files to complile
exe=./run.exe # name of executable
EigenPath = /usr/include/eigen-3.4.0  # path to the Eigen directory 
np=4 # default number of processes to run
N=10 # default end integer i.e. 1 to N in full vector 
print_flag=1 # default print_flag is only print out elapsed time, 0 is verbose

all: 
	$(CC) -I $(EigenPath) $(files) -o $(exe)

run:
	mpiexec -n $(np) $(exe) $(N) $(print_flag)
clean:
	rm $(exe)