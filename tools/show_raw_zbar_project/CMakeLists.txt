cmake_minimum_required(VERSION 2.8.0)
project(zar_proc)

SET(CMAKE_CXX_FLAGS "$ENV{CXX_FLAGS} -O3 -Wall -std=c++11")
SET(CMAKE_C_FLAGS "$ENV{C_FLAGS} -O3")

file(GLOB_RECURSE LIBZBAR_FILES                                                                                                                                                                             
    "./include/*.h"
    "./zbar/*.h"
	"zbar/config.c"
	"zbar/error.c"
	"zbar/symbol.c"
	"zbar/image.c"
	"zbar/convert.c"
	"zbar/processor.c"
	"zbar/processor/lock.c"
	"zbar/refcnt.c"
	"zbar/window.c"
	"zbar/video.c"
	"zbar/img_scanner.c"
	"zbar/scanner.c"
	"zbar/decoder.c"
	"zbar/decoder/ean.c"
	"zbar/decoder/code128.c"
	"zbar/decoder/code39.c"
	"zbar/decoder/i25.c"
	"zbar/decoder/qr_finder.c"
	"zbar/qrcode/qrdec.c"
	"zbar/qrcode/qrdectxt.c"
	"zbar/qrcode/rs.c"
	"zbar/qrcode/isaac.c"
	"zbar/qrcode/bch15_5.c"
	"zbar/qrcode/binarize.c"
	"zbar/qrcode/util.c"
	"zbar/processor/posix.c"
	"zbar/video/null.c"
	"zbar/processor/null.c"
	"zbar/window/null.c"
)

add_definitions(-DHAVE_CONFIG_H -DNDEBUG)

include_directories("./include/" "./zbar/")


add_library(libzbar ${LIBZBAR_FILES})
set_target_properties(libzbar PROPERTIES PREFIX "")


find_package(OpenCV REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

add_executable(show_image zbar_cv.cpp raw_image.c)
target_link_libraries(show_image libzbar ${OpenCV_LIBS} pthread)


