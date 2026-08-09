file(REMOVE_RECURSE
  "ToyDialect.cpp.inc"
  "ToyDialect.h.inc"
  "ToyOps.cpp.inc"
  "ToyOps.h.inc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/vt_gen.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
