format ELF64
extrn map
public btreeU64_push
section '.text' executable

define seed 0x9e3779b97f4a7c15 
define capacity 252

macro m_pop{
pop rbx
pop rcx
pop r11
pop r10
pop r9
pop r8
}

btreeU64_push:
push r8
push r9
push r10
push r11
push rcx
push rbx
xor eax,eax
mov r8, qword [rdi]
mov r9, qword [rdi+8]
mov r10, qword [rdi+16]
cmp r8, capacity
jae .dive
mov qword [rdi+24+r8*8], rsi
mov qword [rdi+2040+r8*8], rdx
cmp rsi,r9
cmovb r9,rsi
cmp rsi,r10
cmova r10,rsi
inc r8
mov qword [rdi],r8
mov qword [rdi+8],r9
mov qword [rdi+16],r10
m_pop
ret
.dive:
lea r9, qword [rdi+4072]
lea r10, qword [rdi+4080]
mov r8, seed
imul r8, rsi
imul r8, rdi
and r8, 2
cmp r8,0
cmovne r9, r10
mov rbx,r9;
mov r9, qword [r9]
cmp r9, -1
jne .next
push r8
push r9
push r10
push r11
push r12
push rcx
push rdi
push rsi
push rdx
mov rdi, 4096
call map
cmp rax, -1
pop rdx
pop rsi
pop rdi
pop rcx
pop r12
pop r11
pop r10
pop r9
pop r8
jle .end
mov qword [rbx], rax
mov qword [rax+8], -1
mov qword [rax+4072], -1
mov qword [rax+4080], -1
mov qword [rax+4088], rdi
mov rdi, rax
xor rax,rax
m_pop
jmp btreeU64_push
.end:
mov rax,-1
m_pop
ret
.next:
mov rdi, r9
m_pop
jmp btreeU64_push
