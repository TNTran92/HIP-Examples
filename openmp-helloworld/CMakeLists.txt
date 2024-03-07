#this method follows "Consuming the HIP API in C++ code"
#https://rocm.docs.amd.com/en/latest/conceptual/cmake-packages.html#consuming-the-hip-api-in-c-code

project(openmp_helloworld)

cmake_minimum_required(VERSION 3.16)

message("Is it Window?: " ${CMAKE_HOST_WIN32})
message("Is it Linux?: " ${CMAKE_HOST_UNIX})


# Search for rocm in common locations
list(APPEND CMAKE_PREFIX_PATH /opt/rocm/hip /opt/rocm-6.0.2 /opt/rocm)

# Find HIP.
# The user may override AMDGPU_TARGETS defined in the HIP config file
# to select the AMDGPU archs to compile for.
# ex. set(AMDGPU_TARGETS "gfx803;gfx900;gfx906")
find_package(hip REQUIRED)

# Find OpenMP.
find_package(OpenMP REQUIRED)

# Set compiler and linker.
if(NOT WIN32)
  set(CMAKE_CXX_COMPILER ${HIP_HIPCC_EXECUTABLE})
  set(CMAKE_CXX_LINKER   ${HIP_HIPCC_EXECUTABLE})
endif()
message("Current CMAKE_CXX_COMPILER (should show hipcc): " ${CMAKE_CXX_COMPILER})
message("Current CMAKE_CXX_LINKER (should show hipcc): " ${CMAKE_CXX_LINKER})

set(CMAKE_BUILD_TYPE Release)

# Source files.
set(CPP_SOURCES ${CMAKE_SOURCE_DIR}/openmp_helloworld.cpp)
message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# Preparing the executable.
#Add an executable target called "test_openmp_helloworld" to be built from the source files listed in the command invocation.
add_executable(test_openmp_helloworld ${CPP_SOURCES})

# Link Libraries - HIP Device and OpenMP.
target_compile_options(test_openmp_helloworld PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(test_openmp_helloworld PRIVATE hip::device ${OpenMP_CXX_FLAGS})
