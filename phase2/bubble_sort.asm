A0 = 5
A1 = 30
A2 = 7
A3 = 19
A4 = 70
A5 = 3
&$MAIN:
LD R1, A0, R10
ADDI R1, R1, -1
    label5:
    BGE R2, R1, label1
    SUB R5, R1, R2
        label4:
        BGE R3, R5, label2
        ADDI R6, R3, 1
        LD R7, A1, R3
        LD R8, A1, R6
        BGT R8, R7, label3
        ST R8, A1, R3
        ST R7, A1, R6
        label3:
        ADDI R3, R3, 1
        JUMP label4
    label2:
    SUB R3, R3, R3
    ADDI R2, R2, 1
    JUMP label5
label1:
&$END: