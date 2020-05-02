rm -rf build
mkdir build
cd build
cmake .. -G Xcode -DCMAKE_PREFIX_PATH=/Users/cjstein/Code/RONN/plugin/libtorch
cmake --build .
