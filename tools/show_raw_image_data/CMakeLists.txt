cmake_minimum_required(VERSION 2.8.0)
project(image_proc)

find_package(OpenCV REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

add_executable(show_raw_image_data show_raw_image.cpp)
target_link_libraries(show_raw_image_data ${OpenCV_LIBS})

