set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.9)
SET (CMAKE_CXX_FLAGS "-fno-rtti -fPIC")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(LLVMLivenessPass MODULE Liveness.cpp)
set_target_properties(LLVMLivenessPass PROPERTIES COMPILE_FLAGS "-D__GLIBCXX_USE_CXX11_ABI=0 ")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
endif ()
target_link_libraries(LLVMLivenessPass)