cmake_minimum_required(VERSION 3.0.2)
project(serial_reader)

## 找到catkin构建系统和所需的包
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  serial
)

## 声明catkin包
catkin_package()

## 包含头文件目录
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## 编译可执行文件
add_executable(serial_reader_node src/serial_reader_node.cpp)
target_link_libraries(serial_reader_node ${catkin_LIBRARIES})

