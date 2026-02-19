cmake_minimum_required(VERSION 3.16)

project(pertemuan5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Lokasi library (macOS / Homebrew)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/opencv")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Untuk Windows (uncomment jika diperlukan)
# list(APPEND CMAKE_PREFIX_PATH "C:\\msys64\\ucrt64")

# Temukan Package
find_package(OpenCV REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

# Target
add_executable(${PROJECT_NAME}
        main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ${OpenCV_LIBS}
)