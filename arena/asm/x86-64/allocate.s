format ELF64
extrn lock_activate
extrn lock_deactivate
extrn array_push
extrn array_pop
extrn grow_internal
section '.text' executable

; SysV
; @arg0 -> Arena pointer
; @arg1 -> Array_number
arena_allocate:
;--------------
push r8
push r9
push r10
;--------------
mov r8d, dword [rdi+60] ; flags
mov r10d, dword [rdi+60]; flags
mov r9d, dword [rdi+60]; flags
and r10d, 0x02
;--------------
and r8d, 0x04
cmp r8d, 0x04
cmove rsi, 0
;--------------
test r10d,r10d
jz .locka
;--------------
.step_1:
mov r10, rsi
lea rsi, [rdi+24+r10*4]
call array_pop
cmp rax,-1
;--------------
jle .fail_to_pull
call lock_deactivate
pop r10
pop r9
pop r8
ret
;--------------
.fail_to_pull:
lea rdi, [rdi+36+r10*4]
call lock_deactivate
pop r10
pop r9
pop r8
mov r8d, dword [rdi+60]
and r8d, 0x01
test r8d, r8d
jnz .alloc_dyn
ret
;--------------
.alloc_dyn:
push rsi
push rdx
mov rdx, rsi
mov esi, dword [rdi+48+rsi*4]
call grow_internal
cmp rax, -1
jle .fail_to_pull
pop rdx
pop rsi
jmp arena_allocate
;--------------
.locka:
push rdi
push rsi
lea rdi, [rdi+36+4*rsi]
mov rsi, r9d 
and rsi, 0x08
call lock_activate
pop rsi
pop rdi
pop r10
pop r9
pop r8
cmp rax,0
je .step_1
mov rax,-3
ret
