README - 5/31/2012

Team: default
Names: Bartholomew, Dan
	   Choe, Ju
	   Hardin, Kurt

How to compile the program:
	- run make within source directory

How to run the program:
	- run ./SchedSim

Command line options:
	- you can run the program without any argument for default config
	- up to 3 integers following the programming: 
		1st argument: number of cpu bound processes
		2nd argument: number of IO bound processes
		3rd argument: number of producer-consumer pairs
	- the maximum number of processes is 8(each producer-consumer pairs create 2 processes)

Log file output
	- When the program runs the program outputs the entire log to a text file.
	- This text file can be found on the current directory that you are in when you run the program 
	   and named as "log.txt"

How to quit the program:
	- press "q"

* some of the functions in the .c files are not commented because the function names themselves do
   a very good job of explaining what they do.

