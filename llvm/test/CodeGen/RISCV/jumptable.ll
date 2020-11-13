; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=riscv32 -verify-machineinstrs < %s \
; RUN:   | FileCheck %s -check-prefix=RV32I

define void @jt(i32 %in, i32* %out) nounwind {
; RV32I-LABEL: jt:
; RV32I:       # %bb.0: # %entry
; RV32I-NEXT:   addi  a0, a0, -1
; RV32I-NEXT:   addi  a2, zero, 3
; RV32I-NEXT:   bltu  a2, a0, .LBB0_7
; RV32I-NEXT: # %bb.1:                                # %entry
; RV32I-NEXT:   slli  a0, a0, 2
; RV32I-NEXT:   lui a2, %hi(.LJTI0_0)
; RV32I-NEXT:   addi  a2, a2, %lo(.LJTI0_0)
; RV32I-NEXT:   add a0, a0, a2
; RV32I-NEXT:   lw  a0, 0(a0)
; RV32I-NEXT:   jr  a0
; RV32I-NEXT: .LBB0_2:                                # %bb1
; RV32I-NEXT:   addi  a0, zero, 4
; RV32I-NEXT:   j .LBB0_6
; RV32I-NEXT: .LBB0_3:                                # %bb2
; RV32I-NEXT:   addi  a0, zero, 3
; RV32I-NEXT:   j .LBB0_6
; RV32I-NEXT: .LBB0_4:                                # %bb3
; RV32I-NEXT:   addi  a0, zero, 2
; RV32I-NEXT:   j .LBB0_6
; RV32I-NEXT: .LBB0_5:                                # %bb4
; RV32I-NEXT:   addi  a0, zero, 1
; RV32I-NEXT: .LBB0_6:                                # %exit
; RV32I-NEXT:   sw  a0, 0(a1)
; RV32I-NEXT: .LBB0_7:                                # %exit
; RV32I-NEXT:   ret
; RV32I-NEXT: .Lfunc_end0:
; RV32I-NEXT:   .size jt, .Lfunc_end0-jt
; RV32I-NEXT:   .section  .rodata,"a",@progbits
; RV32I-NEXT:   .p2align  2
; RV32I-NEXT: .LJTI0_0:
; RV32I-NEXT:   .word .LBB0_2
; RV32I-NEXT:   .word .LBB0_3
; RV32I-NEXT:   .word .LBB0_4
; RV32I-NEXT:   .word .LBB0_5
entry:
  switch i32 %in, label %exit [
    i32 1, label %bb1
    i32 2, label %bb2
    i32 3, label %bb3
    i32 4, label %bb4
  ]
bb1:
  store i32 4, i32* %out
  br label %exit
bb2:
  store i32 3, i32* %out
  br label %exit
bb3:
  store i32 2, i32* %out
  br label %exit
bb4:
  store i32 1, i32* %out
  br label %exit
exit:
  ret void
}
