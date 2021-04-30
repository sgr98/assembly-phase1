# PROJECT ON ARCHITECTURE
## PHASE 1 (Initiating the architecture)

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
> ...           
> ...  
> JUMP -28                        (line 43)  

instead of-

> label: ADD R1, R2, R3     (line 15)  
> ...  
> ...  
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

## PHASE 2 (Adding the Pipeline)

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
> ADD R1, R2, R3                  (line 15)  
> ...  
> ...  
> JUMP label1                     (line 43)  

### Rules to use command line arguments:
* The argument should contain the relative asm file location and attach it with the appropriate fileName.extension
* The argumnent should be inputted while executing the file, i.e., while executing as given by the syntax on c++.

EXample : 

For Compiling: `g++ .\phase2.cpp .\Components\Processor\processor.cpp .\Components\LexicalAnalyser\lexical_analyser.cpp .\Components\Encoder\encoder.cpp -o phase2`

For executing: `./phase2 ./FileLocation/fileName.extension`

example: 
1) `./phase2 ./Testing/t.asm`, 
2) `./phase2 ./bubble_sort.asm` and
3) `./phase2 ./Testing/rigorous.asm`.


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

## PHASE 3 (Caching the processor)  

### Details about the MEMORY:  
* The architecture possesses 2 levels of cache: L1 and L2. 
* The L1 cache is faster to access and smaller in size whereas the L2 cache is slower compared to L1 but bigger in size.
* Both of them are set-associative which can be converted to either direct mapping or fully associative mapping depending on the values set in "cache_specs.txt" inside "Sample" directory.
* Both caches implement LRU (Least-Recently Used) policy to maintain blocks in the cache.
* LRU is implemented using a integer array of array_size equal to the total number of lines in the cache.
  * `Totla Number of Lines = Cache_Size / Block_Size`
* Physically the number of bits in a LRU would be equal to the `Log2(Number of Lines per Set)`
  * `Number of Lines per Set = Total_number_of_lines / Total_number_of_sets = associativity`
  * `Total_number_of_sets = Total_Number_of_lines / associativity`
* The cache also contains a tag array of same array_size as LRU which contains tag bits (integer array to contain the total value of bits instead of actually storing individual bits).
* During the MEM Stage, the address is first searched in the L1. 
  * If found, the data is returned. 
  * If not, then the same is repeated for L2. 
  * If address is not found in L2 then the block of data is requested from main memory.
  * Then the data is stored in L1 following the set-associative property and if a data is expunged from L1 and stored in L2.
* The caches are partially Exclusive.
  * If a data is expunged from L1, it is stored in L2.
  * If a data is expunged from L2, it is completely removed.
  * A data from the main memory is requested and stored only in L1 and not in L2.
* The cache follow a Write-Through policy to have a good reliabiliyt of storing data into the main memory. That is when a STORE instruction is called, then it's data is modified in all the locations where the address exists, i.e., L1, L2 and main memory in one stage.


### Incorporating cache into Pipeline:
* After going through different cases, we can say that by incorporating cache and memory latency, the whole pipeline is stalled until the data is fetched from the cache or memory.
* This made it easy to incorporate it into the pipleine as the only changes in code we had to made was in the MEM stage for both Executions, with and without data forwarding.
* Thus we simply added the total latency and made a for loop to increase 'my_clock'. This is done in order to simulate the whole latency problem.
* This is same for both Load and Store operations.
* Details of the cases are present in "./phase3/Testing/P3.xlsx".

## PHASE 3 (EXTENDED: MEMORY IMPLEMENTING WRITE BACK)

*30/4/2021: Note: Incomplete and throwing a lot of logical errors*
