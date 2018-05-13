#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int main(int argc, char const *argv[])
{
    cl_uint num_platforms;
    /* find the number of OpenCL platforms */
    clGetPlatformIDs(0, NULL, &num_platforms);
    if (num_platforms == 0) {
        printf("Found 0 platforms!\n");
        return EXIT_FAILURE;
    }

    // Create a list of platform IDs
    cl_platform_id platform[num_platforms];
    clGetPlatformIDs(num_platforms, platform, NULL);

    printf("\nNumber of OpenCL platforms: %d\n", num_platforms);
    printf("\n---------------------------------\n");

    const char *platform_attribute_name[] = {"Platform", "Vendor", "Version",
                                      "Profile", "Extensions"};
    const cl_platform_info platform_attribute_type[] = {
        CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS};
    size_t num_platform_attributes = sizeof(platform_attribute_name) / sizeof(char *);

    const char *device_attribure_name[] = {"Name",
                                    "Version",
                                    "Max. Compute units",
                                    "Max Work-group size",
                                    "Max Work-group Dims",
                                    "Local memory size",
                                    "Global memory size",
                                    "Max. Alloc size"};
    // "Max. work-group size",
    // "Max. work-item dimensions"};
    const cl_device_info device_attribure_type[] = {CL_DEVICE_NAME,
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
    size_t num_device_attribures =
        sizeof(device_attribure_name) / sizeof(char *);

    for (int i = 0; i < num_platforms; ++i) {
        // get platform attribute value size
        size_t infoSize;
        char *info;
        for (int j = 0; j < num_platform_attributes; ++j) {
            clGetPlatformInfo(platform[i], platform_attribute_type[j], 0, NULL,
                              &infoSize);
            info = (char *)malloc(sizeof(char) * infoSize);
            clGetPlatformInfo(platform[i], platform_attribute_type[j], infoSize, info,
                              NULL);
            printf("  %d.%d %-11s:%s\n", i + 1, j + 1, platform_attribute_name[j],
                   info);
            free(info);
        }

        // Count the number of devices in the platform
        cl_uint num_devices;
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);

        // Get devices IDs
        cl_device_id device[num_devices];
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, num_devices, device,
                       NULL);
        printf("---------------------------------\n");
        printf("Number of devices: %d\n", num_devices);

        // Investigate each device
        for (int j = 0; j < num_devices; ++j) {
            printf("\t----------------------------------------------------\n");
            // Get device info
            for (int k = 0; k < num_device_attribures; ++k) {
                if (k < 2) { // string attributes
                    clGetDeviceInfo(device[j], device_attribure_type[k], 0, NULL,
                                    &infoSize);
                    info = (char *)malloc(sizeof(char) * infoSize);
                    clGetDeviceInfo(device[j], device_attribure_type[k], infoSize,
                                    info, NULL);
                    printf("\t%d.%d.%d %-18s: %s\n", i + 1, j + 1, k + 1,
                           device_attribure_name[k], info);
                    free(info);
                } else { // memory attribute
                    cl_ulong memSize;
                    clGetDeviceInfo(device[j], device_attribure_type[k],
                                    sizeof(cl_ulong), &memSize, NULL);
                    printf("\t%d.%d.%d %-18s: %llu", i + 1, j + 1, k + 1,
                           device_attribure_name[k], memSize);
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
