CXXFLAGS="-g -O2 -pg" ./configure
make clean
make
cd s51.src
make
mv s51 s51p
