#include <iostream>
#include <CL/cl.hpp>
#include <fstream>
#include <vector>
int main()
{
    
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        for(auto p : platforms) {
                std::cout << p.getInfo<CL_PLATFORM_NAME>() << std::endl;;
        }
        auto platform = platforms.front();

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL,&devices) ;

        auto device = devices.front();
        std::ifstream kernelFile("numerical_reduction.cl");
        std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

        cl::Program::Sources sources(1, std::make_pair(src.c_str(),src.length()+1));
        cl::Context context(device);
        cl::Program program(context,sources);
        program.build();
   
        std::vector<int> vec(1024);
        for(int i = 0;i < vec.size(); ++i) {
                vec[i] = i;
        }
        

        cl::Kernel kernel(program,"arrayprocess");
        auto workGroupSize =  kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
        std::cout << "workGroupSize : " << workGroupSize << std::endl;
        auto numWorkGroups = vec.size() / workGroupSize;
        std::cout << numWorkGroups << std::endl;

        cl::Buffer buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int)*vec.size(), vec.data());
        cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * numWorkGroups);


        kernel.setArg(0,buf);
        kernel.setArg(1,sizeof(int)* workGroupSize, nullptr);
        kernel.setArg(2,outBuf);

        std::vector<int> outVec(numWorkGroups);

        cl::CommandQueue queue(context,device);
        queue.enqueueNDRangeKernel(kernel,cl::NullRange, cl::NDRange(vec.size()),cl::NDRange(workGroupSize));
        queue.enqueueReadBuffer(outBuf, CL_TRUE,0,sizeof(int)*outVec.size(), outVec.data());
        std::cout << "end" << std::endl;
        for(int i: outVec) {
                std::cout << i << " " << std::endl;
        }
        return 0;
}
