format ELF64
public btreeU64_new
extrn map
section '.text' executable

btreeU64_new:
xor rax,rax
push r8
push r9
push r10
push r11
push r12
push rdi
push rcx
mov rdi, 4096
call map
pop rcx
pop rdi
xor r8,r8
mov qword [rdi], rax
mov qword [rax+8], -1
mov qword [rax+4072], -1
mov qword [rax+4080], -1
mov qword [rax+4088], -1
cmp rax,0
cmovg rax,r8
pop r12
pop r11
pop r10
pop r9
pop r8
ret
