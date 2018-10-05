; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

; Test that basic functions assemble as expected.

target datalayout = "e-m:e-p:32:32-i64:64-n32:64-S128"
target triple = "wasm32-unknown-unknown"

; DIS-LABEL: f0:
; CHECK: return
; CHECK: .size f0,
define void @f0() {
  ret void
}

; CHECK-LABEL: f1:
; CHECK: i32.const   $0, 0
; CHECK-NEXT: return  $0
; CHECK: .size f1,
 define i32 @f1() {
  ret i32 0
}

; CHECK-LABEL: f2:
; CHECK: i32.const   $2, 0
; CHECK-NEXT: return  $2
; CHECK: .size f2,
define i32 @f2(i32 %p1, float %p2) {
  ret i32 0
}

; CHECK-LABEL: f3:
; CHECK: return
; CHECK: .size f3,
define void @f3(i32 %p1, float %p2) {
  ret void
}

; CHECK-LABEL: f4:
; CHECK: jf
; CHECK: jmp
; CHECK: return
; CHECK: return
; CHECK: .size f4,
define i32 @f4(i32 %x) {
entry:
   %c = trunc i32 %x to i1
   br i1 %c, label %true, label %false
true:
   ret i32 0
false:
   ret i32 1
}
