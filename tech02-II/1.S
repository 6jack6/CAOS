.global main

.text
main:
  // Save stack frame and ret address
  stp x29, x30, [sp, -16]! // sp <- sp - 16
  mov x29, sp
  // Allocate local variables
  sub sp, sp, 16

  // Call scanf
  adr x0, SCANF_STR
  sub x1, x29, 4
  sub x2, x29, 8
  bl scanf

  // Add numbers
  ldr x1, [x29, -4]
  ldr x2, [x29, -8]
  add x1, x1, x2

  // Call printf
  adr x0, PRINTF_STR
  bl printf

  // Free local variables
  mov sp, x29
  // Restore fp, sp and free the memory
  ldp x29, x30, [sp], 16
  // Exit with return code 0
  mov w0, 0
  ret

.data
SCANF_STR: .string "%d%d"
PRINTF_STR: .string "%d\n"