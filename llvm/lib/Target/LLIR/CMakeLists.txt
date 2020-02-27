set(LLVM_TARGET_DEFINITIONS LLIR.td)

tablegen(LLVM LLIRGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM LLIRGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM LLIRGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM LLIRLLIRCCodeEmitter.inc -gen-emitter)
tablegen(LLVM LLIRGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM LLIRGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(LLIRCommonTableGen)

add_llvm_target(LLIRCodeGen
  LLIRArgumentMove.cpp
  LLIRAsmPrinter.cpp
  LLIRFrameLowering.cpp
  LLIRInstrInfo.cpp
  LLIRISelIDAGToDAG.cpp
  LLIRISelLowering.cpp
  LLIRMachineFunctionInfo.cpp
  LLIRMCInstLower.cpp
  LLIRRegisterInfo.cpp
  LLIRRegisterNumbering.cpp
  LLIRSubtarget.cpp
  LLIRTargetMachine.cpp
  LLIRTargetObjectFile.cpp
)

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
