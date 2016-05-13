# parallelGroup
Allen Wu
Kainoa Eastlack
Michael Metrocavich
Nevin Jacob 

How to run:
Note- Comment out the "#define __LOCAL__" line if running on BGQ, or leave it uncommented if running
on Kratos or on a personal machine. It is recommended that testing be done on BGQ though.
Compile the program using the included Makefile, then run it in a terminal by entering:
./antfarm 0 <matrix size> <number of ants to use> <amount of food to spawn> 0
Alternatively, if using BGQ, you can run the runall.bat to run all of the test cases used for 
our writeup, or you can just run one of the jobs in that batch file if you just want to run
one specific test.

Contribution summary:
Allen Wu-created the skeleton template and implemented run_tick, fixed various bugs
Kainoa Eastlack-wrote the paper
Michael Metrocavich-wrote the exchange_cells code and designed the original functionality
Nevin Jacob-helped with general design, fixed bugs, made some performance improvements
