__kernel void arrayprocess(__global int* data, __global int* out) {
    out[get_global_id(0)] = data[get_global_id(0)] * 2;
}