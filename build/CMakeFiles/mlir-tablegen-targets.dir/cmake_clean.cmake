file(REMOVE_RECURSE
  "ToyDialect.cpp.inc"
  "ToyDialect.h.inc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/mlir-tablegen-targets.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
