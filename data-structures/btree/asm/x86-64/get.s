format ELF64
public btreeU64_get
public btreeU64_query
section '.text' executable
include 'const.inc'
macro hash targ, tmp {
    mov  tmp, 0x9E3779B97F4A7C15
    imul targ, tmp
    shr  targ, 12
}
; out = in % fac
macro mod reg {
    and reg, BRANCHES - 1
}

macro remove_clobbered{
pop rcx
pop r12
pop r11
pop r10
pop r9
pop r8
}

; rdi self
; rsi key
; rax -> index
btreeU64_query:
push r8
push r9
push r10
push r11
push r12
push rcx
mov rax,-1
.iterate:
mov r8,  qword [rdi]
mov r9,  qword [rdi+8]
mov r10, qword [rdi+16]

test r10, r10
jz .dive

cmp rsi, r8
jb .dive
cmp rsi, r9
ja .dive
xor r11,r11
xor rcx,rcx
prefetcht0 [rdi+KEYS_OFF]

vmovq xmm0, rsi
vpbroadcastq ymm0, xmm0
lea r12, [rdi+KEYS_OFF]
.loop:
vmovdqu ymm1, [r12+rcx*8]
vpcmpeqq ymm2, ymm1, ymm0
vpmovmskb r11d, ymm2
test r11d, r11d
jnz .found
add rcx, 4
cmp rcx,r10
jae .dive
jmp .loop
.found:
xor r8,r8
bsf r8d, r11d
shr r8d, 3
add rcx, r8 
mov rax, rcx
remove_clobbered
ret

.dive:
mov r8, rsi
xor r9,r9
  hash r8, r9
  mod r8
lea r8, [rdi+24+r8*8]
mov r9, qword [r8]
cmp r9, 0
cmovne rdi,r9
jne .iterate
mov rax, -1
remove_clobbered
ret


; rdi self
; rsi key
; rdx *value
btreeU64_get:
call btreeU64_query
cmp rax,-1
je .not_found
mov rax, qword [rdi+VALUES_OFF+rax*8]
mov qword [rdx], rax
xor rax,rax
ret
.not_found:
ret
