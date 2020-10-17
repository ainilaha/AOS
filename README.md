## Advance Operator System

### MPI code examples
[examples](https://github.com/ainilaha/AOS/tree/master/code)

### Project 1: Implement MPIGather(),MPIscatter() and MPI_Allgather()
The objective of [this project](https://github.com/ainilaha/AOS/tree/master/project1) is to compare MPI scatter and gather functions with your own
scatter and gather functions. 


Program 1 Myscatter.c \
Program 2 Mygather.c \
Program 3 Myallgather.c \
Within each program, collect the time it takes to execute both MPI functions and your functions.
Run the program with different number of processes: 4, 6, 8, 10, 12, 14, 16, 18, 20, 22 and 24.
Plot the graph (time vs # of processes) for each program. 

`mpicc laha_gater.c -o gater`\
`mpirun -np 8  gater`

### Project 2: Berkeley Clock Algorithm
In [project2](https://github.com/ainilaha/AOS/tree/master/project2),Write a C or C++ program named berkeley.c or berkeley.cpp (executable name should be berkeley) to implement the Berkeley time synchronization algorithm as discussed in the class using MPI for communication. 
The screenshot from jumpshot should be saved as berkeley.pdf. Read the values for the clocks at each processor and the coordinator processor from an input file specified as a command line argument.

`mpicc berkeley.c -o berkeley`\
`mpirun -np 4  berkeley berkeley1.txt`

### Project 3: Logical Clocks and Vector Clocks
Write a C or C++ program named [logical.c](https://github.com/ainilaha/AOS/blob/master/project3/logical.c) or logical.cpp (executable name should be logical) to implement Lamport's logical clocks using MPI. The screenshot from jumpshot should be saved as logical.pdf. You will implement a set of primitives to maintain the state of logical clocks. To show that your logical clocks work correctly, your program should read in a set of simulation parameters and then print the logical clocks associated with each event. It is recommended that a single manager process and a set of worker processes be implemented. The manager process will manage the simulation, and the worker processes will exchange messages and implement the logical clocks.
\
\
Write a C or C++ program named [vector.c](https://github.com/ainilaha/AOS/blob/master/project3/vector.c) or vector.cpp (executable name should be aosproj2_1) to implement vector clocks using MPI. The screenshot from jumpshot should be saved as vector.pdf. You will implement a set of primitives to maintain the state of vector clocks. To show that your vector clocks work correctly, your program should read in a set of simulation parameters and then print the vector clocks associated with each event. It is recommended that a single manager process and a set of worker processes be implemented. The manager process will
manage the simulation, and the worker processes will exchange messages and implement the vector clocks.

### Project 4: Suzuki and Kasami’s Broadcast Algorithm
Write a C or C++ program named [suzuki.c](https://github.com/ainilaha/AOS/tree/master/project4) or suzuki.cpp (executable name should be suzuki) to implement Suzuki and Kasami’s Broadcast Algorithm using MPI. The screenshot from jumpshot should be saved as suzuki.pdf.
Each process records its state of being outside the critical section (RELEASED), requesting entry (REQUESTED) or being in the critical section (GRANTED). A process can randomly request entry to critical section after waiting for some time, if it has not already requested one or is not currently in the critical section.
In order to get access to the critical section a process must send (N - 1) requests and wait for 1 reply if it is not already holding the token. At the start of the simulation randomly assign the token to any process.
When a process is requesting entry to the critical section, it defers processing requests from other processes until its own requests have been sent to all (N-1) processes.
On every process maintain a data structure which contains the largest sequence number for each thread received (RN vector). The token comprises of a data structure (LN vector) which contains the sequence number of the latest executed request from a thread i and another data structure for maintaining a queue (Q) of requesting threads.
