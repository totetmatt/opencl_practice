__kernel void hello(__global char* data) {
    data[0]='H';
    data[1]='l';
    data[2]='w';
    data[3]='\n';
}