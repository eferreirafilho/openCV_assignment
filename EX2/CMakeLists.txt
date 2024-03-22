cmake_minimum_required(VERSION 2.8)
project( tp2ex2 )
find_package( OpenCV REQUIRED )
add_executable( tp2ex2 tp2ex2.cpp )
target_link_libraries( tp2ex2 ${OpenCV_LIBS} )

