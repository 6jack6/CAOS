#include <sys/syscall.h>

#define stdin 0
#define stdout 1

    .intel_syntax noprefix
    .global _start
    .text

_start:
// Allocate memory
    sub rsp, 1024
// Read from stdin
    mov rax, SYS_read
    mov rdi, stdin
    mov rsi, rsp
    mov rdx, 1024
    syscall
// Compare the number of symbols read and 0
// and finish if they are equal
    cmp rax, 0
    jz end
// Write to stdout
    mov rdx, rax
    mov rax, SYS_write
    mov rdi, stdout
    mov rsi, rsp
    syscall
// Free memory
    add rsp, 1024
// Repeat
    jmp _start

end:
// Free memory
    add rsp, 1024
// Terminate the current process with 0 code
    mov rax, SYS_exit
    mov rdi, 0
    syscall