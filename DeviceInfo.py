#!/usr/bin/env python3
# coding=utf-8
import pyopencl as cl

# Python3版 OpenCL DeviceInfo

platforms = cl.get_platforms()

print("Number of OpenCL platforms:", len(platforms))

for p in platforms:
    print("Platform: {}".format(p.name))
    print("Vendor: {}".format(p.vendor))
    print("Version: {}".format(p.version))

    devices = p.get_devices()
    print("Number of devices:", len(devices))

    for d in devices:
        print("-" * 50)
        print("Name:", d.name)
        print("Version:", d.opencl_c_version)
        print("Max. Compute Units:", d.max_compute_units)
        print("Local Memory Size:", d.local_mem_size / 1024, "KB")
        print("Global Memory Size:", d.global_mem_size / (1024 * 1024), "MB")
        print("Max Alloc Size:", d.max_mem_alloc_size / (1024 * 1024), "MB")
        print("Max Work-group Total Size:", d.max_work_group_size)

        # Find the maximum dimensions of the work-groups
        dim = d.max_work_item_sizes
        print("Max Work-group Dims:(", dim[0],
              " ".join(map(str, dim[1:])), ")")

        print("-" * 50)
