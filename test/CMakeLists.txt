execute_process(
  COMMAND ls -F
  COMMAND grep /
  COMMAND sed -E "s/\(.*\)\//add_subdirectory(\1)/g"
  COMMAND cat > tmp.cmake
)

include(tmp.cmake)