rm -rf build
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/Users/cjstein/Code/DISNET/plugin/libtorch ..
cmake --build . --config Release
cd build
./disnetlib