format ELF64
extrn btreeU64_query
public btreeU64_remove
section '.text' executable

;rdi self
;rsi key
btreeU64_remove:
call btreeU64_query
cmp rax,-1
je .not_found

push r8
push r9
push r10

mov r8,  qword [rdi]
mov r9,  qword [rdi+8]
mov r10, qword [rdi+16]

lea r8, [rdi+896+r10*8]
lea r9, [rdi+2496+r10*8]
mov r8, [r8]
mov r9, [r9]

mov [rdi+896+rax*8], r8
mov [rdi+2496+rax*8], r9
dec r10
mov [rdi+16], r10

cmp r10,0
je .all_zero
cmp rsi, r8
je .min_mut
cmp rsi, r9
je .max_mut
jmp .pass

.min_mut:
push rcx
mov r8, -1
xor r9,r9
xor rcx

.min_mut_step:
mov r9, qword [rdi+896+rcx*8]
cmp r9,r8
cmovb r8,r9
inc rcx
cmp rcx,r10
jb .min_mut_step
mov qword [rdi],r9
pop rcx
jmp .pass

.max_mut:

push rcx
mov r8, 0
xor r9,r9
xor rcx

.max_mut_step:
mov r9, qword [rdi+896+rcx*8]
cmp r9,r8
cmova r8,r9
inc rcx
cmp rcx,r10
jb .max_mut_step
mov qword [rdi+8],r9
pop rcx
jmp .pass

.all_zero:
mov r8, -1
mov r9, 0
mov qword [rdi], r8
mov qword [rdi+8], r9
.pass:
pop r10
pop r9
pop r8
xor rax,rax
.not_found:
ret
