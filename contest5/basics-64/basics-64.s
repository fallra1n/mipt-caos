  .global very_important_function
  .intel_syntax noprefix
  .text

very_important_function:
  push rbp
  mov rbp, rsp
  push r12 # A
  push r13 # len
  push r14 # cur val
  push r15 # counter
  sub rsp, 32

  xor r15, r15
  mov r13, rdi

.Loop:
  cmp r15, r13
  jge .LoopEnd

  # scanf
  mov rdi, offset input
  mov rsi, rsp
  call scanf
  movsxd r14, [rsp]
  
  mov r12, A
  movsxd r12, [r12 + r15 * 4]
  imul r14, r12

  # printf
  mov rdi, offset output
  mov rsi, r14
  call printf

  inc r15
  jmp .Loop

.LoopEnd:
  pop r12 # A
  pop r13 # len
  pop r14 # cur val
  pop r15 # counter
  mov rsp, rbp
  pop rbp
  ret


.section .rodata

input:
  .string "%d"
output:
  .string "%lld\n"