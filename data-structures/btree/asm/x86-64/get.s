format ELF64
extrn map
public btreeU64_get

section '.text' executable


macro look_into{
        vpcmpeqq ymm1, ymm0, [r11]
        vpcmpeqq ymm2, ymm0, [r11+32]
        vpcmpeqq ymm3, ymm0, [r11+64]
        vpcmpeqq ymm4, ymm0, [r11+96]
        
        vpmovmskb r13d, ymm1
        cmp r13d, eax
        mov r12, 0
        cmovne rcx, r12
        cmovne r14d, r13d

        vpmovmskb r13d, ymm2
        cmp r13d,eax
        mov r12, 8
        cmovne rcx, r12
        cmovne r14d, r13d
        
        vpmovmskb r13d, ymm3
        cmp r13d, eax
        mov r12, 16
        cmovne rcx, r12
        cmovne r14d, r13d
        
        vpmovmskb r13d, ymm4
        cmp r13d, eax
        mov r12, 24
        cmovne rcx, r12
        cmovne r14d, r13d
        
        bsf r14d, r14d
        shr r14d, 3
        add rcx, r14
        cmp rcx, 4
        jb .finish

        add r11,96
}

; rdi, rsi, rdx 
; params: self, key, *value 
btreeU64_get:
push r11
push r12
push r13
push r14
xor eax,eax


mov r8, [rdi]
mov r9, [rdi+8]
mov r10, [rdi+16]


push rdi ; will be used as counter

test r8,r8 ; dive condititions 
jz .dive
cmp rsi, r9
jb .dive_bellow
cmp rsi, r10
ja .dive_above

; iteration prepare
lea r11, [rdi+24]
mov rcx, -1
xor rdi,rdi

; iteration
vpbroadcastq ymm0, rsi
repeat 63
look_into
add rdi,4
cmp rdi,r8
ja .not_found
end repeat

; handling the outliar
mov r13, [r11]
test rsi, r13
jz .not_found
jnz .finish

.dive:
cmp rsi,r12
jae .dive_above

.dive_bellow:
pop r14
pop r13
pop r12
pop r11
mov rdi, [rdi+4072]
cmp rdi, -1
je not_found_pop
jmp btreeU64_get

.dive_above:
pop r14
pop r13
pop r12
pop r11
mov rdi, [rdi+4080]
cmp rdi, -1
je not_found_pop
jmp btreeU64_get

.finish:
lea r11, [r11+rcx+2024]
mov qword [rdx], r11
.not_found:
pop rdi
pop r14
pop r13
pop r12
pop r11
.not_found_pop:
ret
