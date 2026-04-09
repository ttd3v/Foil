format ELF64
public lock_activate
section '.text' executable

; SysV
; @arg0 -> pointer
; @arg1 -> can loop

; OUTPUT
; 0     -> activated
; -1    -> Failed to activate
;
lock_activate:
mov rax, 0
lock cmpxchg [rdi], 0xFF
je .return
test rsi,rsi
jnz lock_activate
pause
mov rax, -1
.return:
ret

; SysV
; @arg0 -> pointer
lock_deactivate:
mov [rdi], 0x00
