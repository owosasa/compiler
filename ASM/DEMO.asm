.MODEL SMALL
.386
.STACK 100H
.DATA
.CODE
.STARTUP
MAIN:
        PUSH BP
        MOV BP,SP
        SUB SP,140

        MOV AX,0
        MOV [BP-140],AX
        MOV AX,0
        MOV [BP-134],AX
L0:
        MOV AX,[BP-134]
        MOV BX,33
        CMP AX,BX
        JAE L1
        MOV AX,0
        MOV SI,[BP-134]
        ADD SI,SI
        MOV [BP-66+SI],AX
        MOV AX,0
        MOV SI,[BP-134]
        ADD SI,SI
        MOV [BP-132+SI],AX
        MOV AX,[BP-134]
        INC AX
        MOV [BP-134],AX
        JMP L0
L1:
        MOV AX,1
        MOV SI,0
        MOV [BP-66+SI],AX
        MOV AX,1
        MOV SI,2
        MOV [BP-66+SI],AX
        MOV AX,2
        MOV [BP-134],AX
L2:
        MOV AX,[BP-134]
        MOV BX,33
        CMP AX,BX
        JAE L3
        MOV SI,[BP-134]
        ADD SI,SI
        MOV AX,[BP-66+SI]
        MOV BX,0
        CMP AX,BX
        JNE L4
        MOV AX,[BP-134]
        MOV SI,[BP-140]
        ADD SI,SI
        MOV [BP-132+SI],AX
        MOV AX,[BP-140]
        INC AX
        MOV [BP-140],AX
        JMP L5
L4:
L5:
        MOV AX,0
        MOV [BP-136],AX
L6:
        MOV AX,[BP-136]
        MOV BX,[BP-140]
        CMP AX,BX
        JAE L7
        MOV AX,[BP-134]
        MOV SI,[BP-136]
        ADD SI,SI
        MOV BX,[BP-132+SI]
        MUL BX
        MOV BX,33
        CMP AX,BX
        JBE L8
        JMP L7
        JMP L9
L8:
L9:
        MOV AX,[BP-134]
        MOV SI,[BP-136]
        ADD SI,SI
        MOV BX,[BP-132+SI]
        MUL BX
        MOV [BP-138],AX
        MOV AX,1
        MOV SI,[BP-138]
        ADD SI,SI
        MOV [BP-66+SI],AX
        MOV AX,[BP-134]
        MOV SI,[BP-136]
        ADD SI,SI
        MOV BX,[BP-132+SI]
        MOV DX,0
        DIV BX
        MOV [BP-138],DX
        MOV AX,[BP-138]
        MOV BX,0
        CMP AX,BX
        JNE L10
        JMP L7
        JMP L11
L10:
L11:
        MOV AX,[BP-136]
        INC AX
        MOV [BP-136],AX
        JMP L6
L7:
        MOV AX,[BP-134]
        INC AX
        MOV [BP-134],AX
        JMP L2
L3:
.EXIT
END