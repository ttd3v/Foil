format ELF64
public btreeU64_get
section '.text' executable

define seed 0x9e3779b97f4a7c15

macro m_pop{
pop rcx
pop r10
pop r9
pop r8
}

btreeU64_get:
push r8
push r9
push r10
push rcx
xor rax,rax
mov r8, qword [rdi]
mov r9, qword [rdi+8]
mov r10, qword [rdi+16]
cmp r8, 0
;-------------
je .dive
cmp rsi, r9
jb .dive
cmp rsi, r10
ja .dive
;-------------
xor rcx,rcx
imul r8,r8,8
vmovq xmm0, rsi
vpbroadcastq ymm1, xmm0
.pivot:
vmovdqu ymm0, [rdi+24+rcx]
vpcmpeqq ymm2, ymm1, ymm0
vpmovmskb r10d, ymm2
cmp r10d, 0
jne .found
add rcx, 32
cmp rcx, r8
jb .pivot
jmp .dive
.found:
bsf r10d, r10d
add rcx, r10
mov r10, qword [rdi+rcx+2040]
mov qword [rdx], r10
m_pop
ret
.dive:
mov r10, seed
imul r10, rsi
imul r10, rdi
and r10, 2
mov r9, [rdi+4072]
cmp r10, 0
mov r10, [rdi+4080]
cmove r10, r9 
cmp r10, -1
je .not_found
mov rdi, r10
m_pop
jmp btreeU64_get
.not_found:
m_pop
mov rax, -2
ret
