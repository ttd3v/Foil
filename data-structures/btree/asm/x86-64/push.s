format ELF64
extrn map
extrn btreeU64_new
public btreeU64_push


;typedef struct btreeU64 {
;        u64 length;                    0
;        u64 min;                       8
;        u64 max;                       16
;        u64 keys[253];                 24
;        u64 values[253];               2048
;        struct btreeU64* lnode;        4072
;        struct btreeU64* rnode;        4080
;        u64 __padding;                 4088
;} btreeU64;

; args
; rdi, rsi, rdx, rcx, r8, r9

; callee can trash
; rax, rcx, rdx, rsi, rdi, r8, r9, r10, r11

section '.text' executable

;   i32 btreeU64_push(btreeU64* self, u64 key, u64 value);
;                     ^^^^^^^^^^^^^^  ^^^^^^^  ^^^^^^^^^
;                          rdi          rsi       rdx
btreeU64_push:
push r11
push r12
xor eax,eax
mov r8, [rdi]; length
mov r9, [rdi+8]; min
mov r10, [rdi+16]; max
cmp r8, 253

jae .descend
inc [rdi]
lea r11, [rdi+24+8*r8]
lea r12, [rdi+2048+8*r8]
mov [r11], rsi
mov [r12], rdx
cmp r9,rsi
cmovb r9, rsi
cmp r10,rsi
cmova r10, rsi
mov qword [rdi+8], r9
mov qword [rdi+16], r10

pop r11
pop r12
ret 

.descend:
cmp r10, rsi
lea r8, [rdi+4072]
lea r9, [rdi+4080]
cmova r10, r9
test r10, -1
jz .ok:

.not_alloc:
mov rdi, r10
call btreeU64_new
cmp rax, -1
jle .failure
.ok:
mov rdi, [r10]
pop r11
pop r12
jmp btreeU64_push

.failure:
pop r11
pop r12
ret



