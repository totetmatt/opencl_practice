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
        std::ifstream kernelFile("ker.cl");
        std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

        cl::Program::Sources sources(1, std::make_pair(src.c_str(),src.length()+1));
        cl::Context context(device);
        cl::Program program(context,sources);
        auto err  =program.build("-cl-std=CL2.0");
   
        char buff[16];
        cl::Buffer memBuf(context,CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,sizeof(buff));
        cl::Kernel kernel(program,"hello",&err);
        kernel.setArg(0,memBuf);

        cl::CommandQueue queue(context,device);
        queue.enqueueTask(kernel);
        queue.enqueueReadBuffer(memBuf,CL_TRUE, 0,sizeof(buff),buff);
        std::cout << err << std::endl;
        std::cout << buff << std::endl;

        return 0;
}
