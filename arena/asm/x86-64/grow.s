format ELF64
public grow_internal
public grow
extrn map
extrn unmap
extrn allocate_and_keep
section '.text' executable


; SysV
; @arg0 -> arena pointer
; @arg1 -> count of items
; @arg2 -> array number
; return:
; -1 -> failed to allocate memory
grow_internal:
;----------------------------------------------
push r8
push r9
push r10
;----------------------------------------------
mov r8d, dword [rdi+24+rdx*4]
mov r10d, dword [rdi+48+rdx*4]
mov r9d, r10d
;----------------------------------------------
add r10d, esi
imul r10d, 8
imul r9d,8
imul r8d, 8
;----------------------------------------------
push rdi
mov rdi, r10
call map
pop rdi
cmp rax, -1
jle .failed_to_map
;----------------------------------------------
push rdi
push rsi
mov rsi, [rdi+rdx*8]
mov rdi, rax
mov rcx, r8
rep movsb 
mov rdi, rsi
mov rsi, r9
call unmap
pop rsi
pop rdi
;----------------------------------------------
mov qword [rdi+rdx*8], rax
add dword [rdi+48+rdx*4], esi 
pop r10
pop r9
mov r8, rdi
; SysV
; @arg0 -> page count
; @arg1 -> pointer to write into
; @arg2 -> page_framentation_lvl 
; @arg3 -> length pointer
push rdi
push rsi
push rdx
push rcx
lea rcx, [rdi+24+rdx*4]
mov rdi, rsi
lea rsi, [r8+rdx*8]
call allocate_and_keep
pop rcx
pop rdx
pop rsi
pop rdi
pop r8
ret
;----------------------------------------------
.failed_to_map:
pop r10
pop r9
pop r8
mov rax, -1
ret

;SysV
;@arg0 -> arena
;@arg1 -> to insert
;@arg2 -> array number
grow:
push r8
push r9
push r10
mov r8d,   dword [rdi+60]
mov r9d,   r8d
mov r10d,  0
and r8d,  0x02
and r9d,  0x04
test r9d, r9d
cmovnz rdx, r10d
mov r10d, r8d
test r8d, r8d
jz .locka
call grow_internal
lea rdi, [rdi+36+rdx*4]
call lock_deactivate
pop r10
pop r9
pop r8
ret
.locka:
push rdi
push rsi
lea rdi, [rdi+36+4*rdx]
mov rsi, r10d 
and rsi, 0x08
call lock_activate
pop rsi
pop rdi
pop r10
pop r9
pop r8
cmp rax,0
je .step_1
mov rax,-2
ret
