// kernel:  vadd
//
// Purpose: Compute the elementwise sum c = a+b
//
// input: a and b float vectors of length count
//
// output: c float vector of length count holding the sum a + b
__kernel void vadd(__global const float *a, __global const float *b,
                   __global float *c, const unsigned len)
{
    int gid = get_global_id(0);
    if (gid < len)
        c[gid] = a[gid] + b[gid];
}
