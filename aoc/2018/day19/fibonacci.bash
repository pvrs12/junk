#ip 0
seti 1 0 2
seti 1 0 3
## try to make a loop which fibonaccis up to 100
gtri 2 100 4
## if the previous statement was true, jump to halt, relative 2
muli 4 2 4
addr 0 4 0
##not at 100 on 2, check 3
gtri 3 100 4
## if previous statment was true, relative jump 1
addr 0 4 0
## otherwise, jump 1
addi 0 1 0
## halt the program
seti -2 0 0 
## do fib math
addr 2 3 2
addr 2 3 3
## go back to the start
seti 1 0 0