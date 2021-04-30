A1 = 5
&$MAIN:
LD R1, A1, R2
label3:
ADDI R1, R1, -1
BGT R2, R2, label2
label2:
BNE R1, R2, label3
BEQ R1, R2, label4
label4:
ST R1, A1, R2
&$END: