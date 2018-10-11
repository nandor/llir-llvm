; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s


declare i32 @fn_i32_i32_i32(i32, i32)
declare void @fn_void()

; CHECK-LABEL: _call_i32_i32_i32:
; CHECK-NEXT:  # %bb.0:
; CHECK-NEXT:  arg.i32 $0, 0
; CHECK-NEXT:  addr.i64 $1, _fn_i32_i32_i32@FUNCTION
; CHECK-NEXT:  call.i32  $2, $1, $0, $0
; CHECK-NEXT:  ret.i32 $2
define i32 @call_i32_i32_i32(i32 %a) {
  %r = call i32 @fn_i32_i32_i32(i32 %a, i32 %a)
  ret i32 %r
}

; CHECK-LABEL: _call_void:
; CHECK-NEXT:  # %bb.0:
; CHECK-NEXT:  addr.i64 $0, _fn_void@FUNCTION
; CHECK-NEXT:  call $0
; CHECK-NEXT:  ret.void
define void @call_void(i32 %a) {
  call void @fn_void()
  ret void
}
