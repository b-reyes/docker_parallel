CC=mpic++  # compiler for MPI c++ program
files=hello_world.cpp # files to complile
exe=./run.exe # name of executable
np=4 # default number of processes to run

all: 
	$(CC) $(files) -o $(exe)

run:
	mpiexec -n $(np) $(exe)

clean:
	rm $(exe)