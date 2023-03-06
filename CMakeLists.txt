cmake_minimum_required(VERSION 2.8.3)
project(natnet_ros)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++14)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp 
  rospy 
  std_msgs 
  sensor_msgs 
  genmsg 
  pcl_conversions
  pcl_ros
  geometry_msgs 
  tf2 
  tf2_ros
  geometry_msgs
  tf
)

# NATNET library
# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{HOME}/libnatnet)
# find_library(NATNET_LIB NatNetLibShared)

# PCL
# find_package(PCL 1.8 REQUIRED COMPONENTS common io)
# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES test_pkg
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  $ENV{HOME}/libnatnet/include
)


add_executable(${PROJECT_NAME}_node src/main.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
  #  ${NATNET_LIB}
  ${PROJECT_SOURCE_DIR}/NatNet-SDK/lib/libNatNet.so
  # /home/jerry/Software/catkin_natnet/src/natnet_ros/NatNet-SDK/lib/libNatNet.so
  #  ${PCL_LIBRARIES}
 )

