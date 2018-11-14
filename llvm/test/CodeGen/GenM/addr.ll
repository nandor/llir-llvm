; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s

@caml_atom_table = external global [0 x i64], align 8

; CHECK: addr.i64  $2, _caml_atom_table+8
define i32 @CMP(i32* nocapture readonly %prog) {
  %1 = getelementptr inbounds [0 x i64], [0 x i64]* @caml_atom_table, i64 0, i64 1
  %2 = ptrtoint i64* %1 to i64
  %3 = icmp eq i64 %2, 0
  %4 = zext i1 %3 to i32
  ret i32 %4
}
