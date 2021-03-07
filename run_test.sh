cd build/
make -j4
cd ../test
opt -load ../build/libLLVMLivenessPass.so -Liveness $1.ll