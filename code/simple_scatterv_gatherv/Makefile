CC=mpic++  # compiler for MPI c++ program
files=scatterv_gatherv.cpp # files to complile
exe=./run.exe # name of executable
np=4 # default number of processes to run
N=10 # default integer to sum to
print_flag=1 # default print_flag is only print out elapsed time, 0 is verbose

all: 
	$(CC) $(files) -o $(exe)

run:
	mpiexec -n $(np) $(exe) $(N) $(print_flag)
clean:
	rm $(exe)