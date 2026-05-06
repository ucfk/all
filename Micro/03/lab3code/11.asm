ORG  0000H  
    MOV  DPTR, #1000H 
    MOV  R0, #40H     
    MOV  R1, #55H

LOOP:  
    MOV  A,R1
    MOVX @DPTR,A  
    MOV  A,R0  
    MOVX  A,@DPTR 
    MOV   @R0,A
    INC  DPTR        
    INC  R0          
     
    CJNE R0, #60H, LOOP 
    
    JMP  $  
    END