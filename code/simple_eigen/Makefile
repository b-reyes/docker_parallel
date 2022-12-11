CC=g++  # compiler for MPI c++ program
files=simple_eigen.cpp # files to complile
exe=./run.exe # name of executable
EigenPath = /usr/include/eigen-3.4.0

all: 
	$(CC) -I $(EigenPath) $(files) -o $(exe)

clean:
	rm $(exe)