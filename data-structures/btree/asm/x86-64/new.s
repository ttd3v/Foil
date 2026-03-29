format ELF64
public btreeU64_new
section '.text' executable
include 'const.inc'
; returns pointer
; rdi -> ref*
btreeU64_new:
;stack
push r11
push rcx
push r9
push r8
push r10
push rdx
push rdi
push rsi
; handling syscall
xor rdi, rdi
mov rsi, PAGE
mov rdx, 0x03
mov r8, -1
xor r9,r9
mov r10, 0x8022 ; MAP_ANON | MAP_POP | MAP_PRIV
mov rax, 9
; mmap
syscall
cmp rax,0

pop rsi
pop rdi
pop rdx
pop r10
pop r8
pop r9
pop rcx
pop r11
jl .error
mov qword [rdi], rax
mov qword [rax], -1; min=u64::MAX
xor rax,rax
.error:
ret
