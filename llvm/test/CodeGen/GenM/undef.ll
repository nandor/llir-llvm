; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

declare void @ext_func(i64 %val)


; CHECK-LABEL: _call_undef:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: call.c $1, $undef
define void @call_undef() {
  call void @ext_func(i64 undef)
  ret void
}

; CHECK-LABEL: _ret_undef_i64:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: ret.i64 $undef
define i64 @ret_undef_i64() {
  ret i64 undef
}

; CHECK-LABEL: _ret_undef_i32:
; CHECK-NEXT:  .call c
; CHECK-NEXT: # %bb.0
; CHECK: ret.i32 $undef
define i32 @ret_undef_i32() {
  ret i32 undef
}
