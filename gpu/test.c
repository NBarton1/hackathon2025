#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(err, msg) \
if (err != CL_SUCCESS) { \
fprintf(stderr, "%s failed with error %d\n", msg, err); \
exit(1); \
}

int main() {
    cl_platform_id platform;
    cl_device_id device;
    cl_int err;
    char name[128];

    // Get platform
    err = clGetPlatformIDs(1, &platform, NULL);
    CHECK(err, "clGetPlatformIDs");

    // Get GPU device
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (err == CL_DEVICE_NOT_FOUND) {
        printf("No GPU device found. Trying CPU instead...\n");
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    }
    CHECK(err, "clGetDeviceIDs");

    // Get and print device name
    err = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, NULL);
    CHECK(err, "clGetDeviceInfo");

    printf("OpenCL device: %s\n", name);

    return 0;
}
