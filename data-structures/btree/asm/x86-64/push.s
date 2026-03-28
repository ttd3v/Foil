format ELF64
extrn btreeU64_new
public btreeU64_push
section '.text' executable

macro remove_clobbered{
mov r8,  qword [rsp]
mov r9,  qword [rsp+8]
mov r10, qword [rsp+16]
mov r11, qword [rsp+24]
add rsp, 32
}

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
sub rsp, 32
mov [rsp], r8
mov [rsp+8], r9
mov [rsp+16], r10
mov [rsp+24], r11
.iterate:
mov r8, qword [rdi+16]
cmp r8,200

jae .dive
 lea r11, [896+r8*8]
 lea r10, [2496+r8*8]
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
mov rsi, r8
call btreeU64_new
cmp rax,0
cmova r9,rax
mov qword [r8],r9
jg .iterate
  remove_clobbered
ret
