cmake_minimum_required(VERSION 2.8.8)
project(CPM)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Caffe REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})
add_executable(CPM src/CPM_demo.cpp src/PoseEstimator.cpp)
target_link_libraries(CPM ${Caffe_LIBRARIES})
target_link_libraries(CPM ${OpenCV_LIBS})
