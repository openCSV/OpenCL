#include "cl.h"

const int N = 100;

int A[N], B[N], C[N];

int main()
{
    std::vector<Platform> platforms;
    Platform::get(&platforms);
    Platform platform = platforms[0];
    console_log("Using platform:", platform.getInfo<CL_PLATFORM_NAME>());
    
    std::vector<Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    Device device = devices[0];
    console_log("Using device:", device.getInfo<CL_DEVICE_NAME>());
    
    std::string kernel_source_file = "kernel.cl";
    std::string kernel_code = getKernelCode(kernel_source_file);
    Context context({device});
    Program::Sources sources;
    sources.push_back({kernel_code.c_str(), kernel_code.length()});
    Program program(context, sources);
    if(program.build({device}) != CL_SUCCESS)
    {
        console_log("Error building:", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));
        exit(1);
    }
    console_log("Kernel build successful");
 
    Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof A);
    Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof B);
    Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof C);
    CommandQueue q(context, device);
    q.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof A, A);
    q.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof B, B);

    Kernel kernel = Kernel(program, "poly_multiply");
    kernel.setArg(0, buffer_A);
    kernel.setArg(1, buffer_B);
    kernel.setArg(2, buffer_C);

    q.enqueueNDRangeKernel(kernel, NullRange, NDRange(N), NullRange);
    q.finish();

    q.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof C, C);

    std::cout << "Result : ";
    for(int i=0; i<N; i++)
        std::cout << C[i] << ' ';
    console_log();
    return 0;
}