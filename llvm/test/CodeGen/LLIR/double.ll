; RUN: llc -mtriple=llir_x86_64 -verify-machineinstrs < %s | FileCheck %s


; CHECK-LABEL: fadd:
define double @fadd(double %a, double %b) {
  %r = fmul double %a, %b
  ret double %r
}
