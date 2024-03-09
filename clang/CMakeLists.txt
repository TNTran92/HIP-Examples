#this method follows "Consuming the HIP API in C++ code"
#https://rocm.docs.amd.com/en/latest/conceptual/cmake-packages.html#consuming-the-hip-api-in-c-code

project(clang)

cmake_minimum_required(VERSION 3.21)

message("Is it Window?: " ${CMAKE_HOST_WIN32})
message("Is it Linux?: " ${CMAKE_HOST_UNIX})


# Search for rocm in common locations
list(APPEND CMAKE_PREFIX_PATH /opt/rocm/hip /opt/rocm-6.0.2 /opt/rocm)
list(APPEND HIP_PATH /opt/rocm-6.0.2/llvm/bin/)
# Find HIP.
# The user may override AMDGPU_TARGETS defined in the HIP config file
# to select the AMDGPU archs to compile for.
# ex. set(AMDGPU_TARGETS "gfx803;gfx900;gfx906")
find_package(hip REQUIRED)
find_package(rocThrust REQUIRED)
find_package(rocPRIM REQUIRED)
find_package(MIOpen REQUIRED)
#find_package(hipblas REQUIRED)
#find_package(hipblaslt REQUIRED)
#43find_package(rocSPARSE REQUIRED)


# Find OpenMP.
#find_package(OpenMP REQUIRED)

# Set compiler and linker.
if(NOT WIN32)
  set(CMAKE_CXX_COMPILER ${HIP_HIPCC_EXECUTABLE})
  set(CMAKE_CXX_LINKER   ${HIP_HIPCC_EXECUTABLE})
endif()
message("Current CMAKE_CXX_COMPILER (should show hipcc): " ${CMAKE_CXX_COMPILER})
message("Current CMAKE_CXX_LINKER (should show hipcc): " ${CMAKE_CXX_LINKER})

set(CMAKE_BUILD_TYPE Release)
set(BUILD_CUDA ON)

# Source files.
set(CPP_SOURCES ${CMAKE_SOURCE_DIR}/kernels.hip.cpp ${CMAKE_SOURCE_DIR}/kernels.hip.h ${CMAKE_SOURCE_DIR}/common.hip.cpp ${CMAKE_SOURCE_DIR}/common.hip.h ${CMAKE_SOURCE_DIR}/cpu_ops.hip.cpp ${CMAKE_SOURCE_DIR}/cpu_ops.hip.h ${CMAKE_SOURCE_DIR}/ops.hip.cpp ${CMAKE_SOURCE_DIR}/ops.hip.h ${CMAKE_SOURCE_DIR}/pythonInterface.hip.cpp)
message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# Preparing the executable.
#Add an executable target called "test_openmp_helloworld" to be built from the source files listed in the command invocation.
add_executable(test_clang ${CPP_SOURCES})

# Link Libraries - HIP Device and OpenMP.
#target_compile_options(test_vector_add PRIVATE ${OpenMP_CXX_FLAGS})
target_include_directories(test_clang PRIVATE ${CMAKE_SOURCE_DIR} /opt/rocm-6.0.2/include/hipblas /opt/rocm-6.0.2/include/hipblaslt /opt/rocm-6.0.2/include/hipsparse /opt/rocm-6.0.2/include/hipcub /opt/rocm-6.0.2/include/rocwmma)
target_link_libraries(test_clang PRIVATE hip::device)