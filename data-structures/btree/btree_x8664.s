format ELF64
extrn map
public btreeU64_new
section '.text' executable

;typedef struct btreeU64 {
;        u64 min;
;        u64 lnode_min;
;        u64 rnode_min;
;        u64 max;
;        u64 lnode_max;
;        u64 rnode_max;
;        u64 keys[252];
;        u64 values[252];
;        struct btreeU64* lnode;
;        struct btreeU64* rnode;
;} btreeU64;

; rdi, rsi, rdx, rcx, r8, r9

; void btreeU64_new(btreeU64** output);
btreeU64_new:
push rbx
mov rbx, rdi;
mov rdi, 4096
call map ; pointer to allocated memory is returned, given SysV ABI, the return should be in rax
test rax,rax ;; map return
jle .skip
mov qword [rbx], rax
mov rdi, rax
mov rcx, 512 
mov rax, 0
xor eax, eax
rep stosq
.skip:
pop rbx
ret

;void btreeU64_insert(btreeU64* self, u64 key, u64 value);
;                     ^^^^^^^^^^^^^^  ^^^^^^^  ^^^^^^^^^
;                          rdi          rsi       rdx
btreeU64_insert:

