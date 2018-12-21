#ip 0
## try to make a loop which counts from 0 to 100
gtri 1 100 2
## if the previous statement was true, skip an instruction
addr 0 2 0
## we are not at 100 yet
## jump to instruction 4
seti 3 0 0
## halt the program
seti -2 0 0 
## instruction 4
addi 1 1 1
## go back to the start
seti -1 0 0