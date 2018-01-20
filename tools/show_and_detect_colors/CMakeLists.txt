cmake_minimum_required(VERSION 2.8.0)
project(image_proc)

find_package(OpenCV REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

add_executable(show_image show_and_detect_colors.cpp)
target_link_libraries(show_image ${OpenCV_LIBS})

