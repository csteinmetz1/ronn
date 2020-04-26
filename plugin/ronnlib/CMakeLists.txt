cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ronnlib)

include_directories("/usr/local/include" "/usr/local/opt/llvm/include")
link_directories("/usr/local/lib" "/usr/local/opt/llvm/lib")

find_package(Torch REQUIRED)

add_executable(ronnlib ronnlib.cpp)
target_link_libraries(ronnlib "${TORCH_LIBRARIES}")
set_property(TARGET ronnlib PROPERTY CXX_STANDARD 14)