#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "error.h"

int main(int argc, char const *argv[])
{
    cl_uint platformNums;
    /* find the number of OpenCL platforms */
    cl_uint err = clGetPlatformIDs(0, NULL, &platformNums);
    checkError(err, "Finding platforms");
    if (platformNums == 0) {
        printf("Found 0 platforms!\n");
        return EXIT_FAILURE;
    }

    // Create a list of platform IDs
    cl_platform_id platform[platformNums];
    err = clGetPlatformIDs(platformNums, platform, NULL);
    checkError(err, "Getting platforms");

    printf("\nNumber of OpenCL platforms: %d\n", platformNums);
    printf("---------------------------------\n");

    // Investigate each platform
    for (int i = 0; i < platformNums; ++i) {
        cl_char string[100];
        // Print the platform name
        err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, sizeof(string),
                                string, NULL);
        checkError(err, "Getting platform name");
        printf("Platform: %s\n", string);

        // Print out the platform vendor
        err = clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, sizeof(string),
                                string, NULL);
        checkError(err, "Getting platform vendor");
        printf("Vendor: %s\n", string);

        // Print out the platform OpenCL version
        err = clGetPlatformInfo(platform[i], CL_PLATFORM_VERSION,
                                sizeof(string), string, NULL);
        checkError(err, "Getting platform OpenCL version");
        printf("Version: %s\n", string);

        // Count the number of devices in the platform
        cl_uint devicesNum;
        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 0, NULL,
                             &devicesNum);
        checkError(err, "Finding devices");

        // Get devices IDs
        cl_device_id device[devicesNum];
        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, devicesNum,
                             device, NULL);
        checkError(err, "Getting devices");
        printf("Number of devices: %d\n", devicesNum);

        // Investigate each device
        for (int j = 0; j < devicesNum; ++j) {
            printf("\t----------------------------------------------------\n");
            // Get device name
            err = clGetDeviceInfo(device[j], CL_DEVICE_NAME, sizeof(string),
                                  string, NULL);
            checkError(err, "Getting device name");
            printf("\t\tName: %s\n", string);

            // Get device OpenCL version
            err = clGetDeviceInfo(device[j], CL_DEVICE_VERSION, sizeof(string),
                                  string, NULL);
            checkError(err, "Getting device OpenCL C version");
            printf("\t\tHardware Version: %s\n", string);

            clGetDeviceInfo(device[j], CL_DRIVER_VERSION, sizeof(string),
                            string, NULL);
            printf("\t\tDriver version: %s\n", string);

            err = clGetDeviceInfo(device[i], CL_DEVICE_OPENCL_C_VERSION,
                                  sizeof(string), string, NULL);
            checkError(err, "Getting device version");
            printf("\t\tOpenCL C Version: %s\n", string);

            // Get Max. Compute Units
            cl_uint num;
            err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                                  sizeof(cl_uint), &num, NULL);
            checkError(err, "Getting device max compute units");
            printf("\t\tMax. Compute Units: %d\n", num);

            // Get local memory size
            cl_ulong memSize;
            err = clGetDeviceInfo(device[j], CL_DEVICE_LOCAL_MEM_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            checkError(err, "Getting device local memory size");
            printf("\t\tLocal memory size: %llu KB\n", memSize / 1024);

            // Get global memory size
            err = clGetDeviceInfo(device[j], CL_DEVICE_GLOBAL_MEM_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            checkError(err, "Getting device global memory size");
            printf("\t\tGlobal memory size: %llu MB\n",
                   memSize / (1024 * 1024));

            // Get maximum buffer alloc. size
            err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            checkError(err, "Getting device max allocation size");
            printf("\t\tMax Alloc size: %llu MB\n", memSize / (1024 * 1024));

            // Get work-group size information
            size_t size;
            err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,
                                  sizeof(size_t), &size, NULL);
            checkError(err, "Getting device max work-group size");
            printf("\t\tMax Work-group size: %ld\n", size);

            // Find the maximum dimensions of the work-groups
            err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                                  sizeof(cl_uint), &num, NULL);
            checkError(err, "Getting device max work-item dimensions");

            // Get the max. dimensions of the work-groups
            size_t dims[num];
            err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,
                                  sizeof(dims), &dims, NULL);
            checkError(err, "Getting device max work-item sizes");
            printf("\t\tMax Work-group Dims: ( ");
            for (size_t k = 0; k < num; ++k)
                printf("%ld ", dims[k]);
            printf(")\n");

            printf("\t----------------------------------------------------\n");
        }
        printf("\n---------------------------------\n");
    }

    return EXIT_SUCCESS;
}
