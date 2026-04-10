format ELF64
public arena_deallocate
extrn lock_activate
extrn lock_deactivate
section '.text' executable

; SysV
; @arg0 -> Arena
; @arg1 -> Array number
; @arg2 -> pointer
deallocate:
push r8
push r9
push r10
mov r10d, dword [rdi+60]
mov r9d, r10d
mov r8d, r10d
and r10d, 0x02
and r9d, 0x08
and r8d, 0x04
;-------------------
test r8d,r8d
cmovnz rsi, 0
;-------------------
test r10d,r10d
jz .locka
;-------------------
.back:
mov r8d,  dword [rdi+rsi*8]
mov r9d,  dword [rdi+24+rsi*4]
mov r10d, dword [rdi+48+rsi*4]
;-------------------
cmp r9d, r10d
jae .failure
;-------------------
push rdi
push rsi
push rdx
push rcx
        mov rcx, rdx
        mov r8, rsi
        mov r9, rdi
        lea rdi, [r9+r8*8]
        lea rsi, [r9+24+r8*4]
        lea rdx, [r9+48+r8*4]
        call array_push
pop rcx
pop rdx
pop rsi
pop rdi
pop r10
pop r9
pop r8
ret
;-------------------
.locka:
push rdi
push rsi
mov rdi, [rdi+36+rsi*4]
mov rsi, r9 
pop rsi
pop rdi
call lock_activate
cmp rax, -1
mov r9, -2
cmove rax, r9
jne .back
pop r10
pop r9
pop r8
ret
;-------------------
.failure:
pop r10
pop r9
pop r8
mov rax,-1
ret
