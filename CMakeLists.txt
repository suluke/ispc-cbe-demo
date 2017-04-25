cmake_minimum_required(VERSION 3.0.2)
project(cbetest CXX C)

find_package(LLVM CONFIG REQUIRED)

set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${LLVM_LIBRARY_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

if( LLVM_ENABLE_ASSERTIONS )
  # MSVC doesn't like _DEBUG on release builds. See PR 4379.
  if( NOT MSVC )
    add_definitions( -D_DEBUG )
  endif()
  # On non-Debug builds cmake automatically defines NDEBUG, so we
  # explicitly undefine it:
  if( NOT uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
    add_definitions( -UNDEBUG )
    # Also remove /D NDEBUG to avoid MSVC warnings about conflicting defines.
    foreach (flags_var_to_scrub
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS_MINSIZEREL)
      string (REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
        "${flags_var_to_scrub}" "${${flags_var_to_scrub}}")
    endforeach()
  endif()
endif()

add_library(ispc STATIC
  ispc/cbackend.cpp
  ispc/ispc.cpp
  ispc/llvmutil.cpp
  ispc/module.cpp
  ispc/util.cpp
  )
target_include_directories(ispc PRIVATE
  ${LLVM_INCLUDE_DIR}
  )

add_executable(cbetest
  cbetest.cc
  cbe.cc
  )
target_include_directories(cbetest PRIVATE
  ${LLVM_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}
  )
target_link_libraries(cbetest ispc)

if (NOT LLVM_ENABLE_RTTI)
  target_compile_options(ispc PRIVATE -fno-rtti)
  target_compile_options(cbetest PRIVATE -fno-rtti)
endif()
target_compile_options(ispc PRIVATE -std=c++14 -Wall)
target_compile_options(cbetest PRIVATE -std=c++14 -Wall)

function(link_llvm_components TARGET)
  llvm_map_components_to_libnames(llvm_libs
    ${ARGN}
  )
  target_link_libraries(${TARGET} ${llvm_libs})
endfunction()
link_llvm_components(cbetest
  analysis
  codegen
  core
  irreader
  mc
  native
  passes
  scalaropts
  support
  target
  transformutils
  )
