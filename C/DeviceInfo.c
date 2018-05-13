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
    cl_uint num_platforms;
    /* find the number of OpenCL platforms */
    cl_uint err = clGetPlatformIDs(0, NULL, &num_platforms);
    CHECK_ERROR(err, "Finding platforms");
    if (num_platforms == 0) {
        printf("Found 0 platforms!\n");
        return EXIT_FAILURE;
    }

    // Create a list of platform IDs
    cl_platform_id platforms[num_platforms];
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    CHECK_ERROR(err, "Getting platforms");

    printf("\nNumber of OpenCL platforms: %d\n", num_platforms);
    printf("---------------------------------\n");

    // Investigate each platform
    for (int i = 0; i < num_platforms; ++i) {
        cl_char string[100];
        // Print the platform name
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(string),
                                string, NULL);
        CHECK_ERROR(err, "Getting platform name");
        printf("Platform: %s\n", string);

        // Print out the platform vendor
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR,
                                sizeof(string), string, NULL);
        CHECK_ERROR(err, "Getting platform vendor");
        printf("Vendor: %s\n", string);

        // Print out the platform OpenCL version
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION,
                                sizeof(string), string, NULL);
        CHECK_ERROR(err, "Getting platform OpenCL version");
        printf("Version: %s\n", string);

        // Count the number of devices in the platform
        cl_uint num_devices;
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL,
                             &num_devices);
        CHECK_ERROR(err, "Finding devices");

        // Get devices IDs
        cl_device_id devices[num_devices];
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices,
                             devices, NULL);
        CHECK_ERROR(err, "Getting devices");
        printf("Number of devices: %d\n", num_devices);

        // Investigate each device
        for (int j = 0; j < num_devices; ++j) {
            printf("\t----------------------------------------------------\n");
            // Get device name
            err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(string),
                                  string, NULL);
            CHECK_ERROR(err, "Getting device name");
            printf("\t\tName: %s\n", string);

            // Get device OpenCL version
            err = clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(string),
                                  string, NULL);
            CHECK_ERROR(err, "Getting device OpenCL C version");
            printf("\t\tHardware Version: %s\n", string);

            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(string),
                            string, NULL);
            printf("\t\tDriver version: %s\n", string);

            err = clGetDeviceInfo(devices[i], CL_DEVICE_OPENCL_C_VERSION,
                                  sizeof(string), string, NULL);
            CHECK_ERROR(err, "Getting device version");
            printf("\t\tOpenCL C Version: %s\n", string);

            // Get Max. Compute Units
            cl_uint num;
            err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                                  sizeof(cl_uint), &num, NULL);
            CHECK_ERROR(err, "Getting device max compute units");
            printf("\t\tMax. Compute Units: %d\n", num);

            // Get local memory size
            cl_ulong memSize;
            err = clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            CHECK_ERROR(err, "Getting device local memory size");
            printf("\t\tLocal memory size: %llu KB\n", memSize / 1024);

            // Get global memory size
            err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            CHECK_ERROR(err, "Getting device global memory size");
            printf("\t\tGlobal memory size: %llu MB\n",
                   memSize / (1024 * 1024));

            // Get maximum buffer alloc. size
            err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                                  sizeof(cl_ulong), &memSize, NULL);
            CHECK_ERROR(err, "Getting device max allocation size");
            printf("\t\tMax Alloc size: %llu MB\n", memSize / (1024 * 1024));

            // Get work-group size information
            size_t size;
            err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,
                                  sizeof(size_t), &size, NULL);
            CHECK_ERROR(err, "Getting device max work-group size");
            printf("\t\tMax Work-group size: %ld\n", size);

            // Find the maximum dimensions of the work-groups
            err =
                clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                                sizeof(cl_uint), &num, NULL);
            CHECK_ERROR(err, "Getting device max work-item dimensions");

            // Get the max. dimensions of the work-groups
            size_t dims[num];
            err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,
                                  sizeof(dims), &dims, NULL);
            CHECK_ERROR(err, "Getting device max work-item sizes");
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
