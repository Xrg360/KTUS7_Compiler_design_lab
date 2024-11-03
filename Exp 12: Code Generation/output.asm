section .data

section .text
global _start

Start:
    MOV a, b

    MOV a, op b

    MOV a, b op c

Exit program;
    MOV AX, 4C00h
    INT 21h
