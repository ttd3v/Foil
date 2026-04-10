format ELF64
public grow_internal
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
