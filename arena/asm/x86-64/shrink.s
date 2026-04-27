format ELF64
extrn lock_activate
extrn lock_deactivate
extrn unmap
extrn map
public arena_shrink
section '.text' executable

macro clobber{
push rdi
push rsi
push rbx
push r10
push r11
}
macro unclobber{
pop r11
pop r10
pop rbx
pop rsi
pop rdi
}

; SysV
; @arg0 -> Arena
arena_shrink:
clobber
;----------------------------
add rdi, 36
mov esi, dword [rdi+24]
call lock_activate
cmp rax, -1
jle .end
;----------------------------
mov edx, dword [rdi-12] ; length
xor rcx,rcx
mov rbx, [rdi-36]
;----------------------------
xchg rdi, r10
mov rsi, 4096
.itr:
cmp ecx,edx
jae .itr_end
mov rdi, [rbx]
call unmap
inc rcx
add rbx, 8
jmp .itr
.itr_end
;----------------------------
mov esi, dword [r10+12]
imul esi, 8
sub dword [r10+12], ecx
mov dword [r10-12], 0x00000000
mov edi, dword [r10+12]
imul edi, 8
call map
cmp rax, 0
jl .xchg_before_unlock
mov rdi, [r10-36]
call unmap
mov [r10-36], rax
;----------------------------
xor rax,rax
.xchg_before_unlock:
xchg rdi, r10
.end_unlock:
call lock_deactivate
.end:
unclobber
ret
