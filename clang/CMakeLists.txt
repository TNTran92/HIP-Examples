#this method follows "Consuming the HIP API in C++ code"
#https://rocm.docs.amd.com/en/latest/conceptual/cmake-packages.html#consuming-the-hip-api-in-c-code

project(clang)

cmake_minimum_required(VERSION 3.21)

message("Is it Window?: " ${CMAKE_HOST_WIN32})
message("Is it Linux?: " ${CMAKE_HOST_UNIX})

if(NOT DEFINED HIP_PATH)
    if(NOT DEFINED ENV{HIP_PATH})
        set(HIP_PATH "/opt/rocm/hip" CACHE PATH "Path to which HIP has been installed")
    else()
        set(HIP_PATH $ENV{HIP_PATH} CACHE PATH "Path to which HIP has been installed")
    endif()
endif()
message("HIP_PATH: " ${HIP_PATH})
set(CMAKE_MODULE_PATH "${HIP_PATH}/cmake" ${CMAKE_MODULE_PATH})

find_package(HIP REQUIRED)
if (HIP_FOUND)
  message(STATUS "Found HIP: " ${HIP_VERSION})
else()
  message(FATAL_ERROR "Could not find HIP")
endif()

#find_package(FindThreads REQUIRED)
find_package(rocThrust REQUIRED)
#find_package(rocPRIM REQUIRED)
find_package(MIOpen REQUIRED)
#find_package(hipblas REQUIRED)
#find_package(hipblaslt REQUIRED)
#find_package(rocSPARSE REQUIRED)

# Search for rocm in common locations
list(APPEND CMAKE_PREFIX_PATH /opt/rocm/hip /opt/rocm-6.0.2 /opt/rocm)
list(APPEND HIP_PATH /opt/rocm-6.0.2/llvm/bin/)
# Find HIP.
# The user may override AMDGPU_TARGETS defined in the HIP config file
# to select the AMDGPU archs to compile for.
# ex. set(AMDGPU_TARGETS "gfx803;gfx900;gfx906")

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
# all file
#set(CPP_SOURCES ${CMAKE_SOURCE_DIR}/kernels.hip.cpp ${CMAKE_SOURCE_DIR}/kernels.hip.h ${CMAKE_SOURCE_DIR}/common.hip.cpp ${CMAKE_SOURCE_DIR}/common.hip.h ${CMAKE_SOURCE_DIR}/cpu_ops.hip.cpp ${CMAKE_SOURCE_DIR}/cpu_ops.hip.h ${CMAKE_SOURCE_DIR}/ops.hip.cpp ${CMAKE_SOURCE_DIR}/ops.hip.h ${CMAKE_SOURCE_DIR}/pythonInterface.hip.cpp)
set(CPP_SOURCES ${CMAKE_SOURCE_DIR}/cpu_ops.hip.cpp ${CMAKE_SOURCE_DIR}/cpu_ops.hip.h)
message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# Preparing the executable.
#Add an executable target called "test_openmp_helloworld" to be built from the source files listed in the command invocation.
add_executable(test_clang ${CPP_SOURCES})

# Link Libraries - HIP Device and OpenMP.
#target_compile_options(test_vector_add PRIVATE ${OpenMP_CXX_FLAGS})
target_include_directories(test_clang PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include /opt/rocm-6.0.2/include/hipblas /opt/rocm-6.0.2/include/hipblaslt /opt/rocm-6.0.2/include/hipsparse /opt/rocm-6.0.2/include/hipcub /opt/rocm-6.0.2/include/rocwmma)
target_link_libraries(test_clang PRIVATE hip::device)