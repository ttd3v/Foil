format ELF64
public btreeU64_get
section '.text' executable



macro hash targ, tmp {
    mov  tmp, 0x733CA476C5148639
    imul targ, tmp
    mov  tmp, targ
    shr  tmp, 30
    xor  targ, tmp
    mov  tmp, 0xA4939A417BFB78A3
    imul targ, tmp
    mov  tmp, targ
    shr  tmp, 27
    xor  targ, tmp
}

; out = in % fac
macro mod in, fac, out {
    push rdx
    push rax
    push rcx          ; scratch for divisor
    xor  rdx, rdx
    mov  rax, in
    mov  rcx, fac     ; can't div by immediate
    div  rcx
    mov  out, rdx     ; remainder → out
    pop  rcx
    pop  rax
    pop  rdx
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

xor rcx,rcx

vpbroadcastq xmm0, rsi
vpbroadcastq ymm0, xmm0
lea r12, [rdi+896]
.loop:
vmovdqu ymm1, [r12+rcx*8]
vpcmpeqq ymm2, ymm1, ymm0
vpmovmskb r11d, ymm2
test r11d, r11d
jnz .found
add rcx, 4
cmp rcx,r10
jae .dive
.found:
bsf r8, r11d
add rcx, r8
mov rax, rcx
remove_clobbered
ret

.dive:
mov r8, rsi
xor r9,r9
  hash r8, r9
  mod r8, 109, r8
lea r8, [rsi+24+r8*8]
mov r9, qword [r8]
cmp r9, 0
cmovne rdi,r9
jne .iterate
mov rax, -1
ret


; rdi self
; rsi key
; rdx *value
btreeU64_get:
push rdi
call btreeU64_query
cmp rax,-1
je not_found
lea rax, qword [rdi+rax*8]
mov r10, qword [rax+2496]
mov qword [rdx], r10
xor rax,rax
pop rdi
ret
.not_found:
pop rdi
mov rax,-1
ret
