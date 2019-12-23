set(LLVM_TARGET_DEFINITIONS GenM.td)

tablegen(LLVM GenMGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM GenMGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM GenMGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM GenMGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM GenMGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM GenMGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(GenMCommonTableGen)

add_llvm_target(GenMCodeGen
  GenMArgumentMove.cpp
  GenMAsmPrinter.cpp
  GenMFrameLowering.cpp
  GenMInstrInfo.cpp
  GenMISelIDAGToDAG.cpp
  GenMISelLowering.cpp
  GenMMachineFunctionInfo.cpp
  GenMMCInstLower.cpp
  GenMRegisterInfo.cpp
  GenMRegisterNumbering.cpp
  GenMSubtarget.cpp
  GenMTargetMachine.cpp
  GenMTargetObjectFile.cpp
)

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
