; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

define i32 @main() {
  ret i32 42
}

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
