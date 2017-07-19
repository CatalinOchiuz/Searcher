## Docs

## This won't have proper error handling initially(ex: handling sanitizers conflicts)
## This is still work in progress: don't have an idea yet how to make the experience painless

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# opt_ifndef("Enable ASan"  BOOL  OFF  CXXBASICS_ASAN)
# if(CMAKE_BUILD_TYPE STREQUAL "ASan")
#   if(CXXBASICS_MSAN OR CXXBASICS_TSAN)
#     cberror("AddressSanitizer is not compatible with MemorySanitizer or ThreadSanitizer")
#   endif()

# if(CMAKE_BUILD_TYPE STREQUAL "ASan") -> disable "FASTER_LINKERS"
  macro(__cxxbasics_add_asan  compiler)
    if("${compiler}" STREQUAL "${CMAKE_C_COMPILER}")
      include(CheckCCompilerFlag)

      set(CMAKE_REQUIRED_FLAGS "-Werror -faddress-sanitizer")
      check_c_compiler_flag("-faddress-sanitizer" __cxxbasics_address_sanitizer)

      set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=address")
      check_c_compiler_flag("-fsanitize=address" __cxxbasics_sanitize_address)
    elseif("${compiler}" STREQUAL "${CMAKE_CXX_COMPILER}")
      include(CheckCXXCompilerFlag)

      set(CMAKE_REQUIRED_FLAGS "-Werror -faddress-sanitizer")
      check_cxx_compiler_flag("-faddress-sanitizer" __cxxbasics_address_sanitizer)

      set(CMAKE_REQUIRED_FLAGS "-Werror -fsanitize=address")
      check_cxx_compiler_flag("-fsanitize=address" __cxxbasics_sanitize_address)
    else()
      cberror("Could not obtain CMAKE_C_COMPILER nor CMAKE_CXX_COMPILER")
    endif()

    unset(CMAKE_REQUIRED_FLAGS)

    if(__cxxbasics_sanitize_address)
      set(__cxxbasics_sanitizer_flag "-fsanitize=address")
    elseif(__cxxbasics_address_sanitizer)
      set(__cxxbasics_sanitizer_flag "-faddress-sanitizer")
    endif()

    if("${compiler}" STREQUAL "${CMAKE_C_COMPILER}")
      set(CMAKE_C_FLAGS_ASAN "-O1 -g ${__cxxbasics_sanitizer_flag} -fno-omit-frame-pointer -fno-optimize-sibling-calls"
          CACHE STRING "Flags used by the C compiler during ASan builds."
          FORCE)
    elseif("${compiler}" STREQUAL "${CMAKE_CXX_COMPILER}")
      set(CMAKE_CXX_FLAGS_ASAN "-O1 -g ${__cxxbasics_sanitizer_flag} -fno-omit-frame-pointer -fno-optimize-sibling-calls"
          CACHE STRING "Flags used by the C++ compiler during ASan builds."
          FORCE)
    endif()
    set(CMAKE_EXE_LINKER_FLAGS_ASAN "${__cxxbasics_sanitizer_flag}"
        CACHE STRING "Flags used for linking binaries during ASan builds."
        FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_ASAN "${__cxxbasics_sanitizer_flag}"
        CACHE STRING "Flags used by the shared libraries linker during ASan builds."
        FORCE)
    mark_as_advanced(CMAKE_C_FLAGS_ASAN
                     CMAKE_CXX_FLAGS_ASAN
                     CMAKE_EXE_LINKER_FLAGS_ASAN
                     CMAKE_SHARED_LINKER_FLAGS_ASAN)
  endmacro(__cxxbasics_add_asan  compiler)

  __cxxbasics_add_asan("${CMAKE_C_COMPILER}")
  __cxxbasics_add_asan("${CMAKE_CXX_COMPILER}")
# endif()
