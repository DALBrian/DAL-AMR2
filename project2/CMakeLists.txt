cmake_minimum_required(VERSION 3.0.2)
project(project2)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  tf
  tf2
  tf2_ros
)


###################################
## catkin specific configuration ##
###################################

catkin_package(

CATKIN_DEPENDS 
  roscpp
  std_msgs 
)

###########
## Build ##
###########


include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(odom_tf src/odom_tf.cpp)
add_dependencies(odom_tf ${catkin_EXPORTED_TARGETS})


target_link_libraries(odom_tf ${catkin_LIBRARIES})


