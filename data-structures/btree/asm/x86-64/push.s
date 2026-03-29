format ELF64
extrn btreeU64_new
public btreeU64_push
section '.text' executable

macro remove_clobbered{
pop r8
pop r9
pop r10
pop r11
}

include 'const.inc'
macro hash targ, tmp {
    mov  tmp, 0x9E3779B97F4A7C15
    imul targ, tmp
    shr  targ, 12
}

; out = in % fac
macro mod in, fac, out {
    push rdx
    push rax
    push rcx 
    xor  rdx, rdx
    mov  rax, in
    mov  rcx, fac
    div  rcx
    mov  out, rdx
    pop  rcx
    pop  rax
    pop  rdx
}

; rdi -> self
; rsi -> key
; rdx -> value
btreeU64_push:
push r11
push r10
push r9
push r8
.iterate:
mov r8, qword [rdi+16]
cmp r8, CELLS

jae .dive
 lea r11, [rdi+KEYS_OFF+r8*8]
 lea r10, [rdi+VALUES_OFF+r8*8]
 xor rax,rax
 mov qword [r11], rsi
 mov qword [r10], rdx
inc r8
 mov r9, qword [rdi]
 mov r10, qword [rdi+8]
 cmp rsi, r9
 cmovb r9, rsi
 cmp rsi, r10
 cmova r10, rsi
 mov [rdi], r9
 mov [rdi+8], r10
 mov [rdi+16], r8
   remove_clobbered
 ret 

.dive:
mov r8, rsi
xor r9,r9
  hash r8, r9
  mod r8, 7, r8
lea r8, [rdi+24+r8*8]
mov r9, qword [r8]
cmp r9, 0
cmovne rdi,r9
jne .iterate
push rdi
mov rdi, r8
call btreeU64_new 
pop rdi
cmp rax, -1
jle .fail
mov rdi, [r8]
jmp .iterate
.fail:
remove_clobbered
ret
