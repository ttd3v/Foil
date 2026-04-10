format ELF64
public arena_create
public allocate_and_keep
extrn map
section '.text' executable

; SysV
; @arg0 -> page count
; @arg1 -> pointer to write into
; @arg2 -> page_framentation_lvl 
; @arg3 -> length pointer
allocate_and_keep:
push r8
push r9
push r10

mov r8,  4096 ; accumulator boundary
shr r8,  rdx ; weight boundary

xor r9,  r9 ; iterator counter
mov r10, 4096 ; accumulator state

; This flushes the pointers into the array
; until a page is fully saturated (all pointers flushed).
.iter:
cmp r10, 4096
jae .aak_alloca
cmp r9, rdi
jae .finish
mov qword [rsi+r9*8], rax
inc dword [rcx]
inc r9
add rax, r8 
add r10, r8
jmp .iter

.aak_alloca:
; -----------------------------+
push rdi                      ;|
mov rdi, 4096                 ;|
call map                      ;|
pop rdi                       ;|
; This is allocating memory   ;|
; obviously, the function call;|
; doesn't clobber any register;|
; besides RAX                 ;|
;------------------------------+
cmp rax, -1
jle .aak_err
xor r10, r10
jmp .iter

.aak_finish:
xor rax,rax
.aak_err:
pop r10
pop r9
pop r8
ret


macro ca_clobber_aak_params{
push rdi
push rsi
push rdx
push rcx
}
macro ca_unclobber_aak_params{
pop rcx
pop rdx
pop rsi
pop rdi
}

; SysV
; @arg0 -> page count
; @arg1 -> flags

; OUTPUT:
; >=0 -> arena pointer
; -1 -> failed to allocate memory
arena_create:
; ----------------
push rdi
push rdx
push rcx
push r8
; +--------------+
; |-{ Handling }-|
; +--------------+
mov rcx, rdi
imul rdi, 24
add rdi, 64
call map; allocate N bytes

cmp rax, -1
jle .error_route_mem_failure

test rsi, 0x04
jnz .only_normal_flag

lea rdx, [rax+64+rdi*8]
mov qword [rax+8], rdx

lea rdx, [rax+64+rdi*16]
mov qword [rax+16], rdx

mov r8,rdi
;{~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ca_clobber_aak_params
; -----------------------------
mov rdi, rcx
lea rsi, [rax+64+rdi*8] 
mov rdx, 1
lea rcx, [rax+28]
call allocate_and_keep
cmp rax, -1
jle .error_route_mem_failure_aak
; -----------------------------
mov rdi, r8
lea rsi, [rax+64+r8*16] 
mov rdx, 2
lea rcx, [rax+32]
call allocate_and_keep
cmp rax, -1
jle .error_route_mem_failure_aak
; -----------------------------
ca_unclobber_aak_params
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~}
.only_normal_flag:
; ---------------------------------
ca_clobber_aak_params
mov rdi, r8
lea rsi, [rax+64] 
mov rdx, 0
lea rcx, [rax+24]
call allocate_and_keep
cmp rax, -1
jle .error_route_mem_failure_aak
ca_unclobber_aak_params
; ---------------------------------

mov dword [rax+48], ecx
mov dword [rax+52], ecx
mov dword [rax+56], ecx
mov dword [rax+60], esi 

jmp .success
.error_route_mem_failure_aak:
ca_unclobber_aak_params
jmp .success ; the error is propagated from the allocate_and_keep function
.error_route_mem_failure:
mov rax, -1

.success:
pop r8
pop rcx
pop rdx
pop rdi
ret
