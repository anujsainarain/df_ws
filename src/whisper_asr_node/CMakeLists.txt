cmake_minimum_required(VERSION 3.8)
project(whisper_asr_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# Add whisper.cpp as a subdirectory
add_subdirectory(whisper_cpp)

# Add the executable
add_executable(whisper_asr src/whisper_asr.cpp)

# Link whisper.cpp to the executable
target_link_libraries(whisper_asr whisper)

# ROS 2 dependencies for the executable
ament_target_dependencies(whisper_asr rclcpp std_msgs)

install(TARGETS whisper_asr
  DESTINATION lib/${PROJECT_NAME})

ament_package()
