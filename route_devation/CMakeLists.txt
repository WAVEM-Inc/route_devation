cmake_minimum_required(VERSION 3.8)
project(route_devation)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(route_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(routedevation_msgs REQUIRED)
find_package(rclcpp REQUIRED)
include_directories(
  include
  include/${PROJECT_NAME}
  #${EXTERNAL_INCLUDE_DIRS}
)

set(source_set
  src/main.cpp 
  src/route_devation.cpp
  src/calc_distance.cpp
)

set(dependencies 
  rclcpp
  sensor_msgs
  route_msgs
  nav_msgs
  routedevation_msgs
)

set(node_name
  route_devation_node
)

add_executable(${node_name}
  ${source_set}
)

ament_target_dependencies(${node_name}
  ${dependencies}
)

install(TARGETS
  ${node_name}
  DESTINATION lib/${PROJECT_NAME}
)
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
