rm -rf build
mkdir build
cd build
# Update the absolute path to libtorch (ensure you leave .. at the end)
cmake .. -G Xcode -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch ..
cmake --build .
