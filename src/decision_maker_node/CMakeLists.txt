cmake_minimum_required(VERSION 3.8)
project(decision_maker_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(decision_maker src/decision_maker.cpp)
ament_target_dependencies(decision_maker rclcpp std_msgs)

install(TARGETS decision_maker
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY scripts/
  DESTINATION lib/${PROJECT_NAME}/scripts
)

ament_package()
