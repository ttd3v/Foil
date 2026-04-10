format ELF64
public array_push
public array_pop
section '.text' executable

; SysV
; @arg0 -> Array pointer
; @arg1 -> Array length pointer
; @arg2 -> Array capacity pointer
; @arg3 -> Value
;
; return:
; 0 -> success
; -1 -> out of space
array_push:
push r8
push r9
xor r8,r8
xor r9,r9
mov r8d, dword [rsi]
mov r9d, dword [rdx]
cmp r8d,r9d
jae .oos
mov qword [rdi+r8*8], rcx
inc dword [rsi]
xor rax,rax
pop r9
pop r8
ret
.oos:
mov rax, -1
pop r9
pop r8
ret



; SysV
; @arg0 -> Array pointer
; @arg1 -> Array length pointer
; return:
; >= 0 -> success
; -2 -> empty
array_pop:
push r8
xor r8,r8
cmp rsi, -1
je .invalid
mov r8d, dword [rsi]
test r8d,r8d
jz .empty
mov r8, qword [rdi+r8*8-8]
dec dword [rsi]
mov rax,r8
ret
.empty:
pop r8
mov rax, -2
ret
.invalid:
pop r8
mov rax, -1
ret
