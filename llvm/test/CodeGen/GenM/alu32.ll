; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s -check-prefix=GENM

define i32 @addi(i32 %a) nounwind {
; GENM-LABEL: addi:
; GENM:       # %bb.0:
; GENM-NEXT:    addi a0, a0, 1
; GENM-NEXT:    ret
  %1 = add i32 %a, 1
  ret i32 %1
}
