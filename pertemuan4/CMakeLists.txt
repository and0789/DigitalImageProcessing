
cmake_minimum_required(VERSION 4.2)

project(pertemuan4)

set(CMAKE_CXX_STANDARD 26)

# Path OpenCV (sesuaikan jika perlu)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/opencv")

find_package(OpenCV REQUIRED)

# Tambahkan Inc folder ke include path
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/Inc)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "Src/*.cpp")

# Cek (Optional): Tampilkan file apa saja yang ditemukan (untuk debugging)
message(STATUS "Source files detected: ${SOURCES}")

add_executable(pertemuan4 ${SOURCES})
target_link_libraries(pertemuan4 ${OpenCV_LIBS})