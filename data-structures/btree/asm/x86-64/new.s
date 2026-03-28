format ELF64
public btreeU64_new
section '.text' executable

; returns pointer
; rsi -> ref*
btreeU64_new:
;stack
sub rsp,  64
mov qword [rsp+56], r11 ; I hope the OoO likes it  
mov qword [rsp+48], rcx ; I hope the OoO likes it 
mov qword [rsp+40], r9  ; I hope the OoO likes it  
mov qword [rsp+32], r8  ; I hope the OoO likes it 
mov qword [rsp+24], r10 ; I hope the OoO likes it  
mov qword [rsp+16], rdx ; I hope the OoO likes it 
mov qword [rsp+8], rdi 
mov qword [rsp], rsi
; handling syscall
xor rdi, rdi
mov rsi, 4096
mov rdx, 
mov r8, -1
xor r9,r9
mov r10, 0x8022 ; MAP_ANON | MAP_POP | MAP_PRIV
mov rax, 9
; mmap
syscall
cmp rax,0

mov rsi, qword [rsp]    ; I hope the OoO likes it 
mov rdi, qword [rsp+8]  ; I hope the OoO likes it 
mov rdx, qword [rsp+16] ; I hope the OoO likes it
mov r10, qword [rsp+24] ; I hope the OoO likes it 
mov r8,  qword [rsp+32] ; I hope the OoO likes it 
mov r9,  qword [rsp+40] ; I hope the OoO likes it
mov rcx, qword [rsp+48] ; I hope the OoO likes it 
mov r11, qword [rsp+56] ; I hope the OoO likes it 
add rsp, 64

jl .error
mov qword [rsi], rax
mov qword [rax], -1; min=u64::MAX
xor rax,rax
.error:
ret
