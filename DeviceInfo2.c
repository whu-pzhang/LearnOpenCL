#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int main(int argc, char const *argv[])
{
    cl_uint platformNums;
    /* find the number of OpenCL platforms */
    clGetPlatformIDs(0, NULL, &platformNums);
    if (platformNums == 0) {
        printf("Found 0 platforms!\n");
        return EXIT_FAILURE;
    }

    // Create a list of platform IDs
    cl_platform_id platform[platformNums];
    clGetPlatformIDs(platformNums, platform, NULL);

    printf("\nNumber of OpenCL platforms: %d\n", platformNums);
    printf("\n---------------------------------\n");

    const char *platformAttrName[] = {"Platform", "Vendor", "Version",
                                      "Profile", "Extensions"};
    const cl_platform_info platformAttrType[] = {
        CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS};
    size_t platformAttrCount = sizeof(platformAttrName) / sizeof(char *);

    const char *deviceAttrName[] = {"Name",
                                    "Version",
                                    "Max. Compute units",
                                    "Max Work-group size",
                                    "Max Work-group Dims",
                                    "Local memory size",
                                    "Global memory size",
                                    "Max. Alloc size"};
    // "Max. work-group size",
    // "Max. work-item dimensions"};
    const cl_device_info deviceAttrType[] = {CL_DEVICE_NAME,
                                             CL_DEVICE_OPENCL_C_VERSION,
                                             CL_DEVICE_MAX_COMPUTE_UNITS,
                                             CL_DEVICE_MAX_WORK_GROUP_SIZE,
                                             CL_DEVICE_MAX_WORK_ITEM_SIZES,
                                             CL_DEVICE_LOCAL_MEM_SIZE,
                                             CL_DEVICE_GLOBAL_MEM_SIZE,
                                             CL_DEVICE_MAX_MEM_ALLOC_SIZE};
    // CL_DEVICE_MAX_WORK_GROUP_SIZE,
    // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
    // CL_DEVICE_MAX_WORK_ITEM_SIZES};
    size_t deviceAttrCount = sizeof(deviceAttrName) / sizeof(char *);

    for (int i = 0; i < platformNums; ++i) {
        // get platform attribute value size
        size_t infoSize;
        char *info;
        for (int j = 0; j < platformAttrCount; ++j) {
            clGetPlatformInfo(platform[i], platformAttrType[j], 0, NULL,
                              &infoSize);
            info = (char *)malloc(sizeof(char) * infoSize);
            clGetPlatformInfo(platform[i], platformAttrType[j], infoSize, info,
                              NULL);
            printf("  %d.%d %-11s:%s\n", i + 1, j + 1, platformAttrName[j],
                   info);
            free(info);
        }

        // Count the number of devices in the platform
        cl_uint devicesNum;
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 0, NULL, &devicesNum);

        // Get devices IDs
        cl_device_id device[devicesNum];
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, devicesNum, device,
                       NULL);
        printf("---------------------------------\n");
        printf("Number of devices: %d\n", devicesNum);

        // Investigate each device
        for (int j = 0; j < devicesNum; ++j) {
            printf("\t----------------------------------------------------\n");
            // Get device info
            for (int k = 0; k < deviceAttrCount; ++k) {
                if (k < 2) { // string attributes
                    clGetDeviceInfo(device[j], deviceAttrType[k], 0, NULL,
                                    &infoSize);
                    info = (char *)malloc(sizeof(char) * infoSize);
                    clGetDeviceInfo(device[j], deviceAttrType[k], infoSize,
                                    info, NULL);
                    printf("\t%d.%d.%d %-18s: %s\n", i + 1, j + 1, k + 1,
                           deviceAttrName[k], info);
                    free(info);
                } else { // memory attribute
                    cl_ulong memSize;
                    clGetDeviceInfo(device[j], deviceAttrType[k],
                                    sizeof(cl_ulong), &memSize, NULL);
                    printf("\t%d.%d.%d %-18s: %llu", i + 1, j + 1, k + 1,
                           deviceAttrName[k], memSize);
                    printf(k < 4 ? " \n" : " Bytes\n");
                }
            }

            // Find the maximum dimensions of the work-groups
            cl_uint num;
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                            sizeof(cl_uint), &num, NULL);
            //
            // Get the max. dimensions of the work-groups
            size_t dims[num];
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,
                            sizeof(dims), &dims, NULL);
            printf("\tMax Work-group Dims: ( ");
            for (size_t k = 0; k < num; ++k)
                printf("%ld ", dims[k]);
            printf(")\n");
            //
            printf("\t----------------------------------------------------\n");
        }
        printf("\n---------------------------------\n");
    }

    return EXIT_SUCCESS;
}
