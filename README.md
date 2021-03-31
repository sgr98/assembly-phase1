# PROJECT ON ARCHITECTURE
## PHASE 1

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

## PHASE 2

### Label feature added:
Now **labels** can be added instead of putting actual values of line difference in **branch** and **jump** instructions.
### Rules to use labels:
* The label cannot use more than 8 characters.
* Any alphabetical, numeric or special character can be used but the label should not start with '&$'.
* The from-label can be in branch and jump instructions.
* The to-label should be placed before the instruction where the Program Counter has to jump to.
* The to-label should not be accompanied by any other word or instruction on the same line.
* The to-label should be accompanied by a colon, ":", at the end of the label, like in normal mips.

Example :
> lable1:                         (line 14)
>
> ADD R1, R2, R3                  (line 15)
> 
> ...
>            
> ...
>    
> JUMP label1                     (line 43)   

### Pipeline Design (in c++):
*The pipelining design is same for both: with and without forwarding, except for the stall cases which is also included within the necessary stage.*


For pipelining I went for a while loop whose condition are 5 boolean variables indicating the individual stage during a cycle with initial value: IF = true; IR = EX = MEM = WB = false.

The stages are used only when their respective boolean is set to *true*.

The stages coded within the while loop is in the order WB => MEM => EX => ID/RF => IF. When a particular stage is executed, the boolean value of its own stage is set to *false* and that of its next stage is set to *true*.

The boolean value for IF stage is always *true* unless the PC value is more than the total number of instructions.

Whenever the WB stage is executed, the WB boolean is set to *false*.


**Branch Instruction stall:**
* An intentional stall is placed which triggers whenever a branch or jump instruction is encountered in the ID/RF stage and the IF stage is skipped.


**With Forwarding:**
* Only one type of stall is encountered.
* Stall occurs when the destination register of a load register is same as one of the source registers from the next instruction.
* In this case, the stall is dected in EX stage and is skipped in ID/RF stage.


**Without Forwarding:**
* One type of stall is where a source register of an instruction is the same as the destination register of CurrentInstruction - 1, CurrentInstruction - 2 and CurrentInstruction - 3 causing 3, 2 and 1 stalls respectively.
* All of these stalls are dected in ID/RF stage, and if valid, it is skipped in the same stage.
* In case of 3 or 2 stalls, mostly MEM and EX stage will be executed.
* In case of 1 stall, it is same as Branch Instruction Stall but is skipped in the ID/RF stage itself.


**Stages:**
* IF: It fetches the instruction from the list of instructions provided and the PC_value( this->registers[0] ).
* ID/RF: It fetches the opcode and the necessary operands_indices of registers, memory or immediate.
* EX: Depending on the opcode, it may perform arithmetic or comparative operation. It also updates the PC_value for branch and jump instruction in the same stage.
* MEM: It updates or fetches the value of or from memory from or into the register depending on store or load instruction.
* WB: It updates the value of the destination register depending on the instruction.
