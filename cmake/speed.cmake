set(
    CMAKE_C_FLAGS_SPEED "-O3 -march=native -DNDEBUG -flto"
    CACHE STRING "Flags used by the compiler during builds optimized for speed."
    FORCE
)

set(
    CMAKE_CXX_FLAGS_SPEED "-O3 -march=native -DNDEBUG -flto"
    CACHE STRING "Flags used by the compiler during builds optimized for speed."
    FORCE
)

set(
    CMAKE_EXE_LINKER_FLAGS_SPEED ""
    CACHE STRING "Flags used by the linker during builds of dynamic libraries optimized for speed."
    FORCE
)

set(
    CMAKE_SHARED_LINKER_FLAGS_SPEED ""
    CACHE STRING "Flags used by the linker during builds of static libraries optimized for speed."
    FORCE
)

MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_SPEED
    CMAKE_C_FLAGS_SPEED
    CMAKE_EXE_LINKER_FLAGS_SPEED
    CMAKE_SHARED_LINKER_FLAGS_SPEED
)

SET(
    CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"
    CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS and CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel Speed"
    FORCE
)
