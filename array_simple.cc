#include <iostream>
#include <vector>
int main()
{
        std::vector<int> vec(1000000000);
        std::fill(vec.begin(),vec.end(),2);
        for(int i=0;i<vec.size();i++){
                vec[i] *=2;
        }
        std::cout << "end" << std::endl;
        return 0;
}
