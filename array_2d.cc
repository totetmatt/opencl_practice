#include <iostream>
#include <CL/cl.hpp>
#include <fstream>
#include <array>
int main()
{
    
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        for(auto p : platforms) {
                std::cout << p.getInfo<CL_PLATFORM_NAME>() << std::endl;;
        }
        auto platform = platforms.front();

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU,&devices) ;

        auto device = devices.front();
        std::ifstream kernelFile("array_2d.cl");
        std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

        cl::Program::Sources sources(1, std::make_pair(src.c_str(),src.length()+1));
        cl::Context context(device);
        cl::Program program(context,sources);
        program.build();
   
                // In array      
        const int numRows = 3;
        const int numCols = 2;
        const int count = numRows * numCols;
        std::array<std::array<int,numCols>,numRows> arr= {{{1,1},{2,2},{3,3}}};
  
        cl::Buffer buf(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * count, arr.data());

        cl::Kernel kernel(program,"arrayprocess");
        kernel.setArg(0,buf);

        cl::CommandQueue queue(context,device);

        queue.enqueueNDRangeKernel(kernel,cl::NullRange, cl::NDRange(2,3),cl::NDRange(1,1));
        queue.enqueueReadBuffer(buf, CL_TRUE, 0, sizeof(int) *count, arr.data());
        


        std::cout << "end" << std::endl;

        return 0;
}
