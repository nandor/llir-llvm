; RUN: llc -mtriple=genm -verify-machineinstrs < %s | FileCheck %s


; CHECK-LABEL: _fadd:
define double @fadd(double %a, double %b) {
  %r = fmul double %a, %b
  ret double %r
}
