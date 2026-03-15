format ELF64
public aqueue_push
public aqueue_pop
section '.text' executable

; SysV ABI
;rdi, rsi, rdx, rcx, r8, r9 in, rax out

;typedef struct {
;        u64 tail;
;        u64 head;
;        u64* buffer;
;        u64 buffer_size;
;        u64 capacity;
;} aqueue;

; 0 tail
; 8 head
; 16 buffer
; 24 buffer_size
; 32 capacity

macro pop_clobbered {
        pop r11;
        pop r10;
        pop r9;
        pop r8
}

macro push_clobbered {
        push r8
        push r9
        push r10
        push r11
}

macro pull_head_n_tail {
        mov r8, [rdi]           ; ---+
                                ;    + --- Likely decent OoO
        mov r9, [rdi+8]         ; ---+
}



; clobber-list:
; r8,r9,r10,r11

; ERR
; -1 Full

; SUCCESS
; 0 OK
aqueue_push:
        push_clobbered
        ; rdi -> aq pointer
        .roll:
        pull_head_n_tail;
        mov r10, [rdi+32]
        mov r11, r9
        sub r11, r8
        cmp r11,r10
        jae .err

        mov r8,r9
        inc r8
        mov rax, r9
        lock cmpxchg [rdi+8], r8
        jnz .retry
        mov r11, r10
        dec r11
        and r9,r11
        mov r11, qword [rdi+16]
        lea r8, qword [r11+r9*8]
        mov r9, qword [rsi]
        mov r11, 0x8000000000000000 ; I guess "or" instruction cannot hold this inlined data
        or r9, r11
        mov qword [r8], r9
        jmp .success
        .err:
                pop_clobbered
                mov rax, -1
                ret
        .success:
                pop_clobbered
                mov rax, 0
                ret
        .retry:
                pause
                jmp .roll


; SysV ABI
;rdi, rsi, rdx, rcx, r8, r9 in, rax out

;typedef struct {
;        u64 tail;
;        u64 head;
;        u64* buffer;
;        u64 buffer_size;
;        u64 capacity;
;} aqueue;

aqueue_pop:
        push_clobbered;
        .roll:
        mov r8, qword [rdi]; fetch tail
        mov r9, qword [rdi+8]; fetch head
        cmp r9,r8
        jbe .err
        mov r10, [rdi+32]
        dec r10
        mov rax,r8;
        inc r8
        lock cmpxchg [rdi], r8
        jnz .retry

        ;-------------------------------------------------

        and rax,r10
        mov r8,[rdi+16]
        lea r10, [r8+rax*8]
        .roll2:
        mov r11, [r10]
        mov r8, 0x8000000000000000
        cmp r11, r8 
        jb .retry2
        
        ;-------------------------------------------------

        mov qword [r10], 0x00
        xor r11, r8 
        mov qword [rsi], r11
        jmp .success

        ;-------------------------------------------------
        
        .err:
        pop_clobbered;
        mov rax, -1
        ret

        ;-------------------------------------------------

        .success:
        pop_clobbered
        mov rax, 0
        ret

        ;-------------------------------------------------
        
        .retry:
        pause
        jmp .roll

        ;-------------------------------------------------

        .retry2:
        pause
        jmp .roll2
