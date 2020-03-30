add_llvm_loadable_module(LLVMObfuscation
  SimplePass.cpp
  
  )

  add_dependencies(LLVMObfuscation intrinsics_gen)
