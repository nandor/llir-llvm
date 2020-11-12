; RUN: llc -mtriple=llir_x86_64 -verify-machineinstrs < %s | FileCheck %s


declare void @fdefault()

declare void @f10()
declare void @f11()
declare void @f12()
declare void @f13()
declare void @f14()

declare void @f20()
declare void @f21()
declare void @f22()
declare void @f23()
declare void @f24()


; CHECK-LABEL: switch:
; CHECK:  switch  $2,
define void @switch(i32 %arg) {
  switch i32 %arg, label %default [
    i32 10, label %br_10
    i32 11, label %br_11
    i32 12, label %br_12
    i32 13, label %br_13
    i32 14, label %br_14

    i32 20, label %br_20
    i32 21, label %br_21
    i32 22, label %br_22
    i32 23, label %br_23
    i32 24, label %br_24
  ]

br_10:
  call void @f10()
  ret void

br_11:
  call void @f11()
  ret void

br_12:
  call void @f12()
  ret void

br_13:
  call void @f13()
  ret void

br_14:
  call void @f14()
  ret void

br_20:
  call void @f20()
  ret void

br_21:
  call void @f21()
  ret void

br_22:
  call void @f22()
  ret void

br_23:
  call void @f23()
  ret void

br_24:
  call void @f24()
  ret void

default:
  call void @fdefault()
  ret void
}
