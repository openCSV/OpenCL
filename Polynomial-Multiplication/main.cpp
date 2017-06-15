#include "cl.h"

const int N = 1000;

int A[N], B[N], C[N << 1];

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::vector<Platform> platforms;
    Platform::get(&platforms);
    Platform platform = platforms[0];
    console_log("Using platform:", platform.getInfo<CL_PLATFORM_NAME>());
    
    std::vector<Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    Device device = devices[0];
    console_log("Using device:", device.getInfo<CL_DEVICE_NAME>());
    
    std::string kernel_source_file = "simple_multiply.cl";
    std::string kernel_code = getKernelCode(kernel_source_file.c_str());
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

    for(int i=0; i<N; i++)
        std::cin >> A[i];
    for(int i=0; i<N; i++)
        std::cin >> B[i];
 
    Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof A);
    Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof B);
    Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof C);
    CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);
    q.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof A, A);
    q.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof B, B);

    Kernel kernel = Kernel(program, "poly_multiply");
    kernel.setArg(0, buffer_A);
    kernel.setArg(1, buffer_B);
    kernel.setArg(2, buffer_C);
    kernel.setArg(3, N);

    Event event;
    q.enqueueNDRangeKernel(kernel, NullRange, NDRange(N << 1), NullRange, NULL, &event);
    q.finish();

    q.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof C, C);

    if(checkOutput(C, N << 1))
        console_log("Correct result");
    else
        console_log("Incorrect result");

    cl_ulong time_start, time_end;
	time_start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
	time_end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
	cl_ulong time_taken = time_end - time_start;
	console_log("Time taken(in ns):", time_taken);

    return 0;
}