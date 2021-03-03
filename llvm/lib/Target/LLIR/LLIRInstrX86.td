//===-- LLIRInstrX86.td - LLIR X86 specific instructions ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the LLIR instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

let Predicates = [IsX86_32] in {

def X86_ATOMIC_XCHG_8_32 : I
  < (outs I8:$dst)
  , (ins I32:$addr, I8:$src)
  , [(set I8:$dst, (atomic_swap_8 I32:$addr, I8:$src))]
  , "x86_xchg.i8\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_XCHG_16_32 : I
  < (outs I16:$dst)
  , (ins I32:$addr, I16:$src)
  , [(set I16:$dst, (atomic_swap_16 I32:$addr, I16:$src))]
  , "x86_xchg.i16\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_XCHG_32_32 : I
  < (outs I32:$dst)
  , (ins I32:$addr, I32:$src)
  , [(set I32:$dst, (atomic_swap_32 I32:$addr, I32:$src))]
  , "x86_xchg.i32\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_8_32 : I
  < (outs I8:$dst)
  , (ins I32:$addr, I8:$val, I8:$ref)
  , [(set I8:$dst, (atomic_cmp_swap_8 I32:$addr, I8:$ref, I8:$val))]
  , "x86_cmpxchg.i8\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_16_32 : I
  < (outs I16:$dst)
  , (ins I32:$addr, I16:$val, I16:$ref)
  , [(set I16:$dst, (atomic_cmp_swap_16 I32:$addr, I16:$ref, I16:$val))]
  , "x86_cmpxchg.i16\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_32_32 : I
  < (outs I32:$dst)
  , (ins I32:$addr, I32:$val, I32:$ref)
  , [(set I32:$dst, (atomic_cmp_swap_32 I32:$addr, I32:$ref, I32:$val))]
  , "x86_cmpxchg.i32\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

// Loads.
def : Pat<(acquiring_load<atomic_load_32>  I32:$ptr), (LD_4_I32_I32 I32:$ptr)>;

// Stores.
def : Pat<(releasing_store<atomic_store_8>  I32:$ptr,  I8:$val), (ST_1_I8_I32  I32:$ptr,  I8:$val)>;
def : Pat<(monotonic_store<atomic_store_8>  I32:$ptr,  I8:$val), (ST_1_I8_I32  I32:$ptr,  I8:$val)>;
def : Pat<(atomic_store_8                   I32:$ptr,  I8:$val), (ST_1_I8_I32  I32:$ptr,  I8:$val)>;
def : Pat<(releasing_store<atomic_store_16> I32:$ptr, I16:$val), (ST_2_I16_I32 I32:$ptr, I16:$val)>;
def : Pat<(monotonic_store<atomic_store_16> I32:$ptr, I16:$val), (ST_2_I16_I32 I32:$ptr, I16:$val)>;
def : Pat<(atomic_store_16                  I32:$ptr, I16:$val), (ST_2_I16_I32 I32:$ptr, I16:$val)>;
def : Pat<(releasing_store<atomic_store_32> I32:$ptr, I32:$val), (ST_4_I32_I32 I32:$ptr, I32:$val)>;
def : Pat<(monotonic_store<atomic_store_32> I32:$ptr, I32:$val), (ST_4_I32_I32 I32:$ptr, I32:$val)>;
def : Pat<(atomic_store_32                  I32:$ptr, I32:$val), (ST_4_I32_I32 I32:$ptr, I32:$val)>;

}


let Predicates = [IsX86_64] in {

def X86_ATOMIC_XCHG_8_64 : I
  < (outs I8:$dst)
  , (ins I64:$addr, I8:$src)
  , [(set I8:$dst, (atomic_swap_8 I64:$addr, I8:$src))]
  , "x86_xchg.i8\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_XCHG_16_64 : I
  < (outs I16:$dst)
  , (ins I64:$addr, I16:$src)
  , [(set I16:$dst, (atomic_swap_16 I64:$addr, I16:$src))]
  , "x86_xchg.i16\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_XCHG_32_64 : I
  < (outs I32:$dst)
  , (ins I64:$addr, I32:$src)
  , [(set I32:$dst, (atomic_swap_32 I64:$addr, I32:$src))]
  , "x86_xchg.i32\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_XCHG_64_64 : I
  < (outs I64:$dst)
  , (ins I64:$addr, I64:$src)
  , [(set I64:$dst, (atomic_swap_64 I64:$addr, I64:$src))]
  , "x86_xchg.i64\t$dst, $addr, $src"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_8_64 : I
  < (outs I8:$dst)
  , (ins I64:$addr, I8:$val, I8:$ref)
  , [(set I8:$dst, (atomic_cmp_swap_8 I64:$addr, I8:$ref, I8:$val))]
  , "x86_cmpxchg.i8\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_16_64 : I
  < (outs I16:$dst)
  , (ins I64:$addr, I16:$val, I16:$ref)
  , [(set I16:$dst, (atomic_cmp_swap_16 I64:$addr, I16:$ref, I16:$val))]
  , "x86_cmpxchg.i16\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_32_64 : I
  < (outs I32:$dst)
  , (ins I64:$addr, I32:$val, I32:$ref)
  , [(set I32:$dst, (atomic_cmp_swap_32 I64:$addr, I32:$ref, I32:$val))]
  , "x86_cmpxchg.i32\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

def X86_ATOMIC_CMPXCHG_64_64 : I
  < (outs I64:$dst)
  , (ins I64:$addr, I64:$val, I64:$ref)
  , [(set I64:$dst, (atomic_cmp_swap_64 I64:$addr, I64:$ref, I64:$val))]
  , "x86_cmpxchg.i64\t$dst, $addr, $val, $ref"
  , 0xE1
  >;

// Loads.
def : Pat<(acquiring_load<atomic_load_32>  I64:$ptr), (LD_4_I32_I64 I64:$ptr)>;

// Stores.
def : Pat<(releasing_store<atomic_store_8>  I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(monotonic_store<atomic_store_8>  I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(atomic_store_8                   I64:$ptr,  I8:$val), (ST_1_I8_I64  I64:$ptr,  I8:$val)>;
def : Pat<(releasing_store<atomic_store_16> I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(monotonic_store<atomic_store_16> I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(atomic_store_16                  I64:$ptr, I16:$val), (ST_2_I16_I64 I64:$ptr, I16:$val)>;
def : Pat<(releasing_store<atomic_store_32> I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;
def : Pat<(monotonic_store<atomic_store_32> I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;
def : Pat<(atomic_store_32                  I64:$ptr, I32:$val), (ST_4_I32_I64 I64:$ptr, I32:$val)>;
def : Pat<(monotonic_store<atomic_store_64> I64:$ptr, I64:$val), (ST_8_I64_I64 I64:$ptr, I64:$val)>;
def : Pat<(seq_cst_store<atomic_store_64>   I64:$ptr, I64:$val), (ST_8_I64_I64 I64:$ptr, I64:$val)>;
def : Pat<(atomic_store_64                  I64:$ptr, I64:$val), (ST_8_I64_I64 I64:$ptr, I64:$val)>;

}

let Predicates = [IsX86] in
{
  // Read time stamp counter
  def X86_RDTSC_I64 : I
    < (outs I64:$res)
    , (ins)
    , [(set I64:$res, LLIRrdtsc)]
    , "x86_rdtsc.i64\t$res"
    , 0x15
    >;

  // Barriers.
  def X86_MFENCE : I <(outs), (ins), [(LLIRMFence)], "x86_mfence", 0xE1>;
  def X86_BARRIER : I <(outs), (ins), [(LLIRBarrier)], "x86_barrier", 0xE1>;
}
