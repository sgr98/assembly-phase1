A1 = 1
A2 = 2
A3 = 3
A4 = 4
A5 = 5
A6 = 78
&$MAIN:
ADDI R1, R2, 2
label3:
ADD R4, R5, R6
SUB R7, R8, R9
BNE R10, R11, label1
label4:
JUMP label2
label2:
LD R12, A1, R13
ST R14, A2, R15
label1:
BEQ R1, R17, label3
BGT R19, R20, label4
BGE R22, R23, label5
label5:
&$END:
