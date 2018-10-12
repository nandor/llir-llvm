; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

declare void @ext_func(i64* %ptr)
declare void @ext_func_i32(i32* %ptr)
declare void @use_i8_star(i8*)
declare i8* @llvm.frameaddress(i32)


; CHECK-LABEL: _alloca32:
; CHECK-NEXT: .stack 4
; CHECK-NEXT: # %bb.0
define void @alloca32() noredzone {
  ; CHECK-NEXT: imm.i32 $0, 0
  ; CHECK-NEXT: st.4.i32 [$sp], $0
  ; CHECK-NEXT: ret.void
  %retval = alloca i32
  store i32 0, i32* %retval
  ret void
}
; CHECK: .size _alloca32

; CHECK-LABEL: _alloca3264:
; CHECK-NEXT: .stack 16
; CHECK-NEXT: # %bb.0
define void @alloca3264() {
  %r1 = alloca i32
  %r2 = alloca double
  store i32 0, i32* %r1
  store double 0.0, double* %r2
  ret void
}
; CHECK: .size _alloca3264

; CHECK-LABEL: _allocarray:
; CHECK-NEXT: .stack 132
; CHECK-NEXT: # %bb.0
define void @allocarray() {
  %r = alloca [33 x i32]
  %p = getelementptr [33 x i32], [33 x i32]* %r, i32 0, i32 0
  store i32 1, i32* %p
  %p2 = getelementptr [33 x i32], [33 x i32]* %r, i32 0, i32 3
  store i32 1, i32* %p2
  ret void
}
; CHECK: .size _allocarray

; CHECK-LABEL: _non_mem_use:
; CHECK-NEXT: .stack 48
; CHECK-NEXT: # %bb.0
define void @non_mem_use(i8** %addr) {
  %buf = alloca [27 x i8], align 16
  %r = alloca i64
  %r2 = alloca i64
  call void @ext_func(i64* %r)
  call void @ext_func(i64* %r2)
  %gep = getelementptr inbounds [27 x i8], [27 x i8]* %buf, i32 0, i32 0
  store i8* %gep, i8** %addr
  ret void
}
; CHECK: .size _non_mem_use

; CHECK-LABEL: _allocarray_inbounds:
; CHECK-NEXT: .stack 20
; CHECK-NEXT: # %bb.0
define void @allocarray_inbounds() {
  %r = alloca [5 x i32]
  %p = getelementptr inbounds [5 x i32], [5 x i32]* %r, i32 0, i32 0
  store i32 1, i32* %p
  %p2 = getelementptr inbounds [5 x i32], [5 x i32]* %r, i32 0, i32 3
  store i32 1, i32* %p2
  call void @ext_func(i64* null)
  ret void
}
; CHECK: .size _allocarray_inbounds
