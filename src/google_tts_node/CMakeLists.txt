cmake_minimum_required(VERSION 3.8)
project(google_tts_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(google_tts src/google_tts.cpp)
ament_target_dependencies(google_tts rclcpp std_msgs)

install(TARGETS google_tts
  DESTINATION lib/${PROJECT_NAME})

ament_package()
