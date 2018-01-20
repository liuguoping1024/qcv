#
# CMake listfile to specify the build process, see:
# http://www.cmake.org/cmake/help/documentation.html
#
project(qcv)
cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_LIBRARY_PATH /opt/local/lib ${CMAKE_LIBRARY_PATH})
SET(CMAKE_CXX_FLAGS "$ENV{CXX_FLAGS} -O3 -Wall -std=c++11")

# Check for polluted source tree.
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt OR
    EXISTS ${CMAKE_SOURCE_DIR}/CMakeFiles)
    message(FATAL_ERROR
        "Source directory is polluted:"
        "\n  * remove CMakeCache.txt"
        "\n  * remove CMakeFiles directory")
endif()

# Suppress in-source builds.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation is not allowed within the source directory:"
        "\n  * mkdir build"
        "\n  * cd build"
        "\n  * Unix-like: cmake -G \"Unix Makefiles\" .."
        "\n  * Windows: cmake -G \"Visual Studio 10\" ..")
endif()

# Adjust CMake's module path.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

# Suppress MSVC CRT warnings.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(/Za)
endif()

# Add libqcv library.
file(GLOB_RECURSE LIBQCV_FILES
    "./src/*.cpp"
    "./src/*.h"
    "./src/*.cc"
    "./src/*.hh"
)
if(WIN32)
    file(GLOB LIBQCV_WIN32_FILES
        "./core/lib/win32/*.c"
        "./core/lib/win32/*.h"
    )
    set(LIBQCV_FILES ${LIBQCV_FILES} ${LIBQCV_WIN32_FILES})
    include_directories(SYSTEM "./core/lib/win32/")
endif()

# OpenCV classes
find_package(OpenCV)
if(OpenCV_FOUND)
    file(GLOB_RECURSE LIBQCV_OPENCV_FILES
        "./opencv/*.cpp"
        "./opencv/*.h"
    )
    set(LIBQCV_FILES ${LIBQCV_FILES} ${LIBQCV_OPENCV_FILES})
    include_directories(${OpenCV_INCLUDE_DIRS})
    include_directories("./opencv/")
endif()

include_directories("./src/")
add_library(libqcv ${LIBQCV_FILES})
set_target_properties(libqcv PROPERTIES PREFIX "")

#find_package(Iconv)
#if(ICONV_FOUND)
#    include_directories(${ICONV_INCLUDE_DIR})
#    target_link_libraries(libzxing ${ICONV_LIBRARIES})
#else()
#    add_definitions(-DNO_ICONV=1)
#endif()

# Add OpenCV cli executable
if(OpenCV_FOUND)
    file(GLOB_RECURSE OPENCV_QCV_FILES
        "./qcv-opencv-tools/*.cpp"
        "./qcv-opencv-tools/*.h"
    )
    add_executable(qcv-opencv-tools ${OPENCV_QCV_FILES})
    target_link_libraries(qcv-opencv-tools libqcv ${OpenCV_LIBRARIES})
endif()

# Add cli executable.
file(GLOB_RECURSE QCV_FILES
    "./qcv-tools/*.cpp"
    "./qcv-tools/*.h"
)

#file(GLOB_RECURSE QCV_FILES
#    "./test/*.cpp"
#    "./test/*.h"
#)

add_executable(qcv-tools ${QCV_FILES})
target_link_libraries(qcv-tools libqcv)

#install(TARGETS qcv libqcv
#	LIBRARY DESTINATION lib
#	RUNTIME DESTINATION bin
#	ARCHIVE DESTINATION lib)
#install(DIRECTORY core/src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

# Add testrunner executable.
#find_package(CPPUNIT)
#if(CPPUNIT_FOUND)
#    file(GLOB_RECURSE TESTRUNNER_FILES
#        "./core/tests/src/*.cpp"
#        "./core/tests/src/*.h"
#    )
#    add_executable(testrunner ${TESTRUNNER_FILES})
#    include_directories(${CPPUNIT_INCLUDE_DIR})
#    target_link_libraries(testrunner libqcv  ${CPPUNIT_LIBRARIES})
#else()
#    message(WARNING "Not building testrunner, because CppUnit is missing")
#endif()
