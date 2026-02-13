cmake_minimum_required(4.2)

project(Praktikum0)

set(CMAKE_CXX_STANDARD 26)

# Sesuaikan dengan lokasi opencv yang di install
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/opencv")

# untuk sistem operasi windows
# set(CMAKE_PREFIX_PATH "C:\\msys64\\ucrt64\\bin")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Praktikum0 main.cpp)
target_link_libraries(Praktikum0 ${OpenCV_LIBS})