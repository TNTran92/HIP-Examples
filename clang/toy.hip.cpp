
#include <hip/hip_runtime.h>
#include <iostream>
#ifdef __HIP_PLATFORM_AMD__
// Compiled with HIP-Clang
#endif

__global__ void axpy(float a, float* x, float* y) {
  y[threadIdx.x] = a * x[threadIdx.x];
}

int main(int argc, char* argv[]) {
  const int kDataLen = 4;

  float a = 2.0f;
  float host_x[kDataLen] = {1.0f, 2.0f, 3.0f, 4.0f};
  float host_y[kDataLen];

  // Copy input data to device.
  float* device_x;
  float* device_y;
  hipMalloc(&device_x, kDataLen * sizeof(float));
  hipMalloc(&device_y, kDataLen * sizeof(float));
  hipMemcpy(device_x, host_x, kDataLen * sizeof(float),
             hipMemcpyHostToDevice);

  // Launch the kernel.
  axpy<<<1, kDataLen>>>(a, device_x, device_y);

  // Copy output data to host.
  hipDeviceSynchronize();
  hipMemcpy(host_y, device_y, kDataLen * sizeof(float),
             hipMemcpyDeviceToHost);

  // Print the results.
  for (int i = 0; i < kDataLen; ++i) {
    std::cout << "y[" << i << "] = " << host_y[i] << "\n";
  }

  hipDeviceReset();
  return 0;
}