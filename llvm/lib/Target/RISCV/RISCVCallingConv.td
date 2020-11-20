//===-- RISCVCallingConv.td - Calling Conventions RISCV ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the RISCV architecture.
//
//===----------------------------------------------------------------------===//

// The RISC-V calling convention is handled with custom code in
// RISCVISelLowering.cpp (CC_RISCV).

def CSR_ILP32_LP64
    : CalleeSavedRegs<(add X1, X3, X4, X8, X9, (sequence "X%u", 18, 27))>;

def CSR_ILP32F_LP64F
    : CalleeSavedRegs<(add CSR_ILP32_LP64,
                       F8_F, F9_F, (sequence "F%u_F", 18, 27))>;

def CSR_ILP32D_LP64D
    : CalleeSavedRegs<(add CSR_ILP32_LP64,
                       F8_D, F9_D, (sequence "F%u_D", 18, 27))>;

// Needed for implementation of RISCVRegisterInfo::getNoPreservedMask()
def CSR_NoRegs : CalleeSavedRegs<(add)>;

// Interrupt handler needs to save/restore all registers that are used,
// both Caller and Callee saved registers.
def CSR_Interrupt : CalleeSavedRegs<(add X1,
    (sequence "X%u", 3, 9),
    (sequence "X%u", 10, 11),
    (sequence "X%u", 12, 17),
    (sequence "X%u", 18, 27),
    (sequence "X%u", 28, 31))>;

// Same as CSR_Interrupt, but including all 32-bit FP registers.
def CSR_XLEN_F32_Interrupt: CalleeSavedRegs<(add X1,
    (sequence "X%u", 3, 9),
    (sequence "X%u", 10, 11),
    (sequence "X%u", 12, 17),
    (sequence "X%u", 18, 27),
    (sequence "X%u", 28, 31),
    (sequence "F%u_F", 0, 7),
    (sequence "F%u_F", 10, 11),
    (sequence "F%u_F", 12, 17),
    (sequence "F%u_F", 28, 31),
    (sequence "F%u_F", 8, 9),
    (sequence "F%u_F", 18, 27))>;

// Same as CSR_Interrupt, but including all 64-bit FP registers.
def CSR_XLEN_F64_Interrupt: CalleeSavedRegs<(add X1,
    (sequence "X%u", 3, 9),
    (sequence "X%u", 10, 11),
    (sequence "X%u", 12, 17),
    (sequence "X%u", 18, 27),
    (sequence "X%u", 28, 31),
    (sequence "F%u_D", 0, 7),
    (sequence "F%u_D", 10, 11),
    (sequence "F%u_D", 12, 17),
    (sequence "F%u_D", 28, 31),
    (sequence "F%u_D", 8, 9),
    (sequence "F%u_D", 18, 27))>;

// LLIR: OCaml call which does not preserve any registers.
def CSR_LLIR_Caml_Call : CalleeSavedRegs<
  (add
      X1, X3, X4
  )
>;

// LLIR: OCaml-to-C invoke which which does not preserve any regs.
def CSR_LLIR_Caml_Ext_Invoke : CalleeSavedRegs<
  (add
      X1, X3, X4
  )
>;

// LLIR: OCaml-to-C call which respects the C calling convention.
def CSR_LLIR_Caml_Ext_Call_ILP32_LP64 : CalleeSavedRegs<
  (add CSR_ILP32_LP64)
>;
def CSR_LLIR_Caml_Ext_Call_ILP32F_LP64F : CalleeSavedRegs<
  (add CSR_ILP32F_LP64F)
>;
def CSR_LLIR_Caml_Ext_Call_ILP32D_LP64D: CalleeSavedRegs<
  (add CSR_ILP32D_LP64D)
>;

// LLIR: OCaml allocation call, does not preserve 25-28
def CSR_LLIR_Caml_Alloc_ILP32_LP64 : CalleeSavedRegs<
  (add
    X1, X3, X4, X5, X6, X7,
    (sequence "X%u", 10, 25),
    X28, X29, X30, X31
  )
>;
def CSR_LLIR_Caml_Alloc_ILP32F_LP64F : CalleeSavedRegs<
  (add
    CSR_LLIR_Caml_Alloc_ILP32_LP64,
    (sequence "F%u_F", 0, 31)
  )
>;
def CSR_LLIR_Caml_Alloc_ILP32D_LP64D : CalleeSavedRegs<
  (add
    CSR_LLIR_Caml_Alloc_ILP32_LP64,
    (sequence "F%u_D", 0, 31)
  )
>;

// LLIR: OCaml GC trampoline call, preserves all registers.
def CSR_LLIR_Caml_Gc_ILP32_LP64 : CalleeSavedRegs<
  (add
    X1, X3, X4,
    (sequence "X%u", 5, 31)
  )
>;
def CSR_LLIR_Caml_Gc_ILP32F_LP64F : CalleeSavedRegs<
  (add
    CSR_LLIR_Caml_Alloc_ILP32_LP64,
    (sequence "F%u_F", 0, 31)
  )
>;
def CSR_LLIR_Caml_Gc_ILP32D_LP64D : CalleeSavedRegs<
  (add
    CSR_LLIR_Caml_Alloc_ILP32_LP64,
    (sequence "F%u_D", 0, 31)
  )
>;

// LLIR: setjmp, does not preserve any registers.
def CSR_LLIR_Setjmp : CalleeSavedRegs<
  (add
    X1, X3, X4)
>;
