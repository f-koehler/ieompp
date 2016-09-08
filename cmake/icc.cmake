if(${CMAKE_C_COMPILER} MATCHES "icc.*$")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fp-model precise")
endif()

if(${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model precise")
endif()
