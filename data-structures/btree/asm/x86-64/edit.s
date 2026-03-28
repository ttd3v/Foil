format ELF64
extrn btreeU64_query
public btreeU64_edit
section '.text' executable

;rdi self
;rsi key
;rdx new_value
btreeU64_edit:
call btreeU64_query
cmp rax,-1
je .not_found
mov [rdi+2496+rax*8], rdx
xor rax,rax
.not_found:
ret
