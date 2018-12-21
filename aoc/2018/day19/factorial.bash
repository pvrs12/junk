#ip 0
## input in reg 2
seti 9 0 2
seti 1 0 1
## 
## 
## 
## try to do factorial
gtri 2 0 4
## if we are greater than 0, skip past kill func
addr 0 4 0
## kill
seti -99 0 0
##
## do factorial
mulr 1 2 1
addi 2 -1 2
seti 1 0 0