# assembly-phase1

### Architecture:
* Memory availaible is an integer array of size 1000, thus there is 4kB memory.
* Memory is labelled in A0, A1, A2, ..., A999 and all of it is available for use.
* Number of registers = 32
* Registers are numbered - R0, R1, R2, ..., R31
* R0 is used as PC register, hence should not be used in the program
* Registers other R0 are available for use.
* The types of instructions used are R, I, J, L.
* The branch and jump instructions do not use labels, rather it uses an immediate value which is calculated as the difference between the line where the label would have been and the line where the instruction contains that label.
Example :
> ADD R1, R2, R3                  (line 15)
> 
> ...
>            
> ...
>    
> JUMP -28                        (line 43)   

instead of-

> label: ADD R1, R2, R3     (line 15)   
> 
> ...
> 
> ...
> 
> JUMP label                (line 43)

This is so because JUMP is storing (15 - 43) instead of using a label.
* Note: Do not leave a space after the final operand, immediately press enter after the last operand

*underscore represent spaces*
- [ ]  ADD R1, R2, R3__\n **(WRONG)**
- [x]  ADD R1, R2, R3\n **(RIGHT)**
* Also the program and the initialising should be compact, i.e., there shouldn't be any empty lines between instructions or memory initialisations.
* It uses two keywords: "&$MAIN:" and "&$END:", in order to seperate the initialisation and program.

Refer to the t.asm file in the repository for better understanding.

### Instructions:
R-TYPE:

`ADD R1, R2, R3` means `R1 = R2 + R3`

`SUB R1, R2, R3` means `R1 = R2 - R3`

I-TYPE:

`ADDI R1, R2, imm` means `R1 = R2 + imm`

imm can be both positive and negative integers

`BNE R1, R2, imm` means `if(R1 != R2) => {PC -> PC + imm}`

`BEQ R1, R2, imm` means `if(R1 == R2) => {PC -> PC + imm}`

`BGT R1, R2, imm` means `if(R1 > R2) => {PC -> PC + imm}`

`BGE R1, R2, imm` means `if(R1 >= R2) => {PC -> PC + imm}`

J-TYPE:

`JUMP imm` means `PC -> PC + imm`

L-TYPE:

`LD R1, A1, R3` means `R1 = A1[R3]  OR  R1 = A0[1 + R3]`

`ST R1, A0, R3` means `A0[R3] = R1  OR  A[0 + R3] = R1`
