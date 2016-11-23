set(CMAKE_C_FLAGS_QUICK "-O0" 
    CACHE STRING "Flags used by the C compiler to speed up builds."
    FORCE)
set(CMAKE_CXX_FLAGS_QUICK "-O0"
    CACHE STRING "Flags used by the C++ compiler to speed up builds."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_QUICK ""
    CACHE STRING "Flags used for linking binaries during quick builds."
    FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_QUICK ""
    CACHE STRING "Flags used for linking shared libraries during quick builds."
    FORCE)

MARK_AS_ADVANCED(
    CMAKE_C_FLAGS_QUICK
    CMAKE_CXX_FLAGS_QUICK
    CMAKE_EXE_LINKER_FLAGS_QUICK
    CMAKE_SHARED_LINKER_FLAGS_QUICK)
