; RUN: llc -mtriple=llir -verify-machineinstrs < %s | FileCheck %s

@caml_atom_table = external global [0 x i64], align 8

; CHECK: mov.i64  $2, caml_atom_table+8
define i32 @CMP(i32* nocapture readonly %prog) {
  %1 = getelementptr inbounds [0 x i64], [0 x i64]* @caml_atom_table, i64 0, i64 1
  %2 = ptrtoint i64* %1 to i64
  %3 = icmp eq i64 %2, 0
  %4 = zext i1 %3 to i32
  ret i32 %4
}

@addr = external local_unnamed_addr global i32*, align 8

; Function Attrs: norecurse nounwind readonly ssp uwtable
define i32 @main() local_unnamed_addr #0 {
  %1 = load i64, i64* bitcast (i32** @addr to i64*), align 8
  %2 = trunc i64 %1 to i32
  ret i32 %2
}
