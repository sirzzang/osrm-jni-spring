cmake_minimum_required(VERSION 2.8.8)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'. Please delete them.")
endif()

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(osrm-example C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
  message(STATUS "Building on a 64 bit system")
else()
  message(STATUS "Building on a 32 bit system")
endif()

if(WIN32 AND MSVC_VERSION LESS 1900)
  message(FATAL_ERROR "Building with Microsoft compiler needs Latest Visual Studio 2015 (Community or better)")
endif()

link_directories(${LibOSRM_LIBRARY_DIRS})
# add_executable(osrm-example osrmJNI.cpp) # executable 파일 및 소스 코드 이름 변경
add_library(osrmjni-example SHARED osrmJNI.cpp) # shared option
find_package(LibOSRM REQUIRED)

target_link_libraries(osrmjni-example ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES}) # library 이름 변경
include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS})
include_directories(/usr/lib/jvm/java-11-openjdk-amd64/include /usr/lib/jvm/java-11-openjdk-amd64/include/linux) # jni.h 라이브러리 헤더 추가
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")
