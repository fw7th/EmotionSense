# Install script for directory: /app/src/detection/3rdparty/ncnn/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/libncnn.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ncnn" TYPE FILE FILES
    "/app/src/detection/3rdparty/ncnn/src/allocator.h"
    "/app/src/detection/3rdparty/ncnn/src/blob.h"
    "/app/src/detection/3rdparty/ncnn/src/command.h"
    "/app/src/detection/3rdparty/ncnn/src/cpu.h"
    "/app/src/detection/3rdparty/ncnn/src/gpu.h"
    "/app/src/detection/3rdparty/ncnn/src/layer.h"
    "/app/src/detection/3rdparty/ncnn/src/layer_type.h"
    "/app/src/detection/3rdparty/ncnn/src/mat.h"
    "/app/src/detection/3rdparty/ncnn/src/modelbin.h"
    "/app/src/detection/3rdparty/ncnn/src/net.h"
    "/app/src/detection/3rdparty/ncnn/src/opencv.h"
    "/app/src/detection/3rdparty/ncnn/src/option.h"
    "/app/src/detection/3rdparty/ncnn/src/paramdict.h"
    "/app/src/detection/3rdparty/ncnn/src/pipeline.h"
    "/app/src/detection/3rdparty/ncnn/src/benchmark.h"
    "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/layer_type_enum.h"
    "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/platform.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn/ncnn.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn/ncnn.cmake"
         "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/CMakeFiles/Export/lib/cmake/ncnn/ncnn.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn/ncnn-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn/ncnn.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn" TYPE FILE FILES "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/CMakeFiles/Export/lib/cmake/ncnn/ncnn.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn" TYPE FILE FILES "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/CMakeFiles/Export/lib/cmake/ncnn/ncnn-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ncnn" TYPE FILE FILES "/app/src/detection/3rdparty/ncnn/build-host-gcc-linux/src/ncnnConfig.cmake")
endif()

