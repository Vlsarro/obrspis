cmake_minimum_required (VERSION 3.14)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (obrspis
  LANGUAGES C CXX
  VERSION 0.0.1 
)

if (WIN32)
else()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/unix")
  include(FindLibEdit)

  if (NOT libedit_FOUND)
    message(FATAL_ERROR "libdedit is not found")
  endif()

endif()

add_library(mpc STATIC "mpc/mpc.c" "mpc/mpc.h")

add_executable (obrspis_exe "src/main.cpp"  "src/lisp.cpp" "src/lisp.h")
target_include_directories(obrspis_exe PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/mpc")
if (WIN32)
  target_link_libraries(obrspis_exe PRIVATE mpc)
else()
  target_link_libraries(obrspis_exe PRIVATE mpc ${libedit_LIBRARIES})
endif()

set_property(TARGET obrspis_exe PROPERTY CXX_STANDARD 20)

