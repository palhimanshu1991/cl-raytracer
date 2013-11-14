CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(raytracer)

INCLUDE_DIRECTORIES(
	/usr/local/include/bullet
	/opt/AMDAPP/include
	src
	lib/OCL-Library
)

FILE(GLOB sources src/*.cpp src/*/*.cpp lib/OCL-Library/ocl.cpp)
# */

# We use C++11 for this project.

SET(CMAKE_CXX_FLAGS "-g -Wall -Wno-reorder -Wno-narrowing -std=c++11")

SET(CMAKE_EXE_LINKER_FLAGS "-L/usr/lib/x86_64-linux-gnu -L/usr/local/lib -L/opt/AMDAPP/lib/x86_64/")

ADD_DEFINITIONS(-DBT_USE_DOUBLE_PRECISION)

ADD_EXECUTABLE(raytracer ${sources})

TARGET_LINK_LIBRARIES(
	raytracer

	pthread
	#GL GLU
	SDL
	BulletDynamics BulletCollision LinearMath
	OpenCL
)
