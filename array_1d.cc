#include <iostream>
#include <CL/cl.hpp>
#include <fstream>
int main()
{
    
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        auto platform = platforms.front();

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL,&devices) ;

        auto device = devices.front();
        std::ifstream kernelFile("array_1d.cl");
        std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

        cl::Program::Sources sources(1, std::make_pair(src.c_str(),src.length()+1));
        cl::Context context(device);
        cl::Program program(context,sources);
        program.build("-cl-std=CL2.0");
   
                // In array
        std::vector<int> vec(1000000000);

        std::fill(vec.begin(),vec.end(),2);
        cl_int err;
        cl::Buffer inBuff (context, CL_MEM_READ_ONLY  | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR ,sizeof(int)*vec.size(),vec.data());
        cl::Buffer outBuff(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)*vec.size());


        cl::Kernel kernel(program,"arrayprocess",&err);
        kernel.setArg(0,inBuff);
        kernel.setArg(1,outBuff);

        cl::CommandQueue queue(context,device);
        queue.enqueueFillBuffer(inBuff,3, sizeof(int) * 10, sizeof(int) * (vec.size()-10));
        queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(vec.size()));
        queue.enqueueReadBuffer(outBuff, CL_FALSE, 0, sizeof(int) * vec.size(), vec.data());
        std::cout << err << std::endl;
        cl::finish();


        std::cout << "end" << std::endl;

        return 0;
}
