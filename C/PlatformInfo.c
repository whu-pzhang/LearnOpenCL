#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int main(void)
{
    const char *attributeNames[5] = {"Name", "Vendor", "Version", "Profile",
                                     "Extensions"};
    const cl_platform_info attributeTypes[5] = {
        CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS};
    const int attributeCount = sizeof(attributeNames) / sizeof(char *);

    // get platform count
    cl_uint platformCount;
    clGetPlatformIDs(0, NULL, &platformCount);
    cl_platform_id platforms[platformCount];
    // get all platforms
    clGetPlatformIDs(platformCount, platforms, NULL);

    // for each platform print all attributes
    size_t infoSize;
    for (int i = 0; i < platformCount; i++) {
        printf("\n %d. Platform \n", i + 1);
        for (int j = 0; j < attributeCount; j++) {
            // get platform attribute value size
            clGetPlatformInfo(platforms[i], attributeTypes[j], 0, NULL,
                              &infoSize);
            char *info = (char *)malloc(infoSize);
            // get platform attribute value
            clGetPlatformInfo(platforms[i], attributeTypes[j], infoSize, info,
                              NULL);

            printf("  %d.%d %-11s: %s\n", i + 1, j + 1, attributeNames[j],
                   info);
            free(info);
        }
        printf("\n");
    }

    return 0;
}
