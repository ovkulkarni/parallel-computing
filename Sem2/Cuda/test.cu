//
// http://forums.nvidia.com/index.php?showtopic=34309
//
#include <stdio.h>

// called from host, run on device
__global__ void add_arrays_gpu(float *in1,float *in2,float *out)
{
	int idx=threadIdx.x; // flat model
	
	out[idx]=in1[idx]+in2[idx];
}
int main()
{
    cudaDeviceProp c;
    cudaGetDeviceProperties(&c, 0);
    printf("%s\n", c.name);
    printf("%d\n", c.maxThreadsPerBlock);
    printf("%d, %d, %d\n", c.maxThreadsDim[0], c.maxThreadsDim[1], c.maxThreadsDim[2]);
    printf("%d, %d, %d\n", c.maxGridSize[0], c.maxGridSize[1], c.maxGridSize[2]);
}
