format ELF64
extrn map
public btreeU64_new
section '.text' executable

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

; void btreeU64_new(btreeU64** output);
btreeU64_new:
push rbx
mov rbx, rdi;
mov rdi, 4096
call map ; pointer to allocated memory is returned, given SysV ABI, the return should be in rax
cmp rax, -1 ;; map return
jle .skip
mov qword [rbx], rax
mov rdi, rax
mov rcx, 512 
mov rax, 0
xor eax, eax
rep stosq
mov qword [rdi+4072], -1
mov qword [rdi+4080], -1
.skip:
pop rbx
ret

