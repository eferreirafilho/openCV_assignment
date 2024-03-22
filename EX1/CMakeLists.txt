cmake_minimum_required(VERSION 2.8)
project( tp2ex1 )
find_package( OpenCV REQUIRED )
add_executable( tp2ex1 tp2ex1.cpp )
target_link_libraries( tp2ex1 ${OpenCV_LIBS} )

