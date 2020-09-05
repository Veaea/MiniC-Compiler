* TINY Compilation to TM Code
* File: .tm
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> function declaration
  2:     ST  0,0(5) 	store function
* -> param list
* -> param
  3:     ST  0,2(5) 	param: store the value of a parameter list
* <- param
* -> param
  4:     ST  0,1(5) 	param: store the value of a parameter list
* <- param
* <- param list
* -> compound
* -> local declaration
* <- local declaration
* -> statement list
* -> if
* -> Op
* -> Id
  5:     LD  0,1(5) 	load id value
* <- Id
  6:     ST  0,0(6) 	op: push left
* -> Const
  7:    LDC  0,0(0) 	load const
* <- Const
  8:     LD  1,0(6) 	op: load left
  9:    SUB  0,1,0 	op ==
 10:    JEQ  0,2(7) 	br if true
 11:    LDC  0,0(0) 	false case
 12:    LDA  7,1(7) 	unconditional jmp
 13:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> return 
* -> Id
 15:     LD  0,2(5) 	load id value
* <- Id
 16:     ST  0,2(5) 	return: store value
* <- return
* if: jump to end belongs here
 14:    JEQ  0,3(7) 	if: jmp to else
* -> else
* -> return 
* -> call
* -> arglist
* -> arg
* -> Id
 18:     LD  0,1(5) 	load id value
* <- Id
* <- arg
* <- arglist
 19:     ST  0,0(5) 	call: store value
* <- call
 20:     ST  0,0(5) 	return: store value
* <- return
* <- else
 17:    LDA  7,3(7) 	jmp to end
* <- if
* <- statement list
* <- compound
* <- function declaration
* -> function declaration
 21:     ST  0,3(5) 	store function
* -> param list
* -> param
* <- param list
* -> compound
* -> local declaration
* -> var declaration
 22:     ST  0,4(5) 	store value
* <- var declaration
* -> var declaration
 23:     ST  0,5(5) 	store value
* <- var declaration
* <- local declaration
* -> statement list
* -> assign
* -> call
 24:     ST  0,6(5) 	call: store value
* <- call
 25:     ST  0,4(5) 	assign: store value
* <- assign
* -> assign
* -> call
 26:     ST  0,6(5) 	call: store value
* <- call
 27:     ST  0,5(5) 	assign: store value
* <- assign
* -> call
* -> arglist
* -> arg
* -> call
* -> arglist
* -> arg
* -> Id
 28:     LD  0,4(5) 	load id value
* <- Id
* <- arg
* <- arglist
 29:     ST  0,0(5) 	call: store value
* <- call
* <- arg
* <- arglist
 30:     ST  0,7(5) 	call: store value
* <- call
* <- statement list
* <- compound
* <- function declaration
* End of execution.
 31:   HALT  0,0,0 	
