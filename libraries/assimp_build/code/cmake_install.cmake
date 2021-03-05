# Install script for directory: D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/Debug/assimpd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/Release/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/MinSizeRel/assimp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/RelWithDebInfo/assimp.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp3.1.1x" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/Debug/assimpd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/Release/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/MinSizeRel/assimp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/RelWithDebInfo/assimp.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/anim.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/ai_assert.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/camera.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/color4.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/color4.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/config.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/defs.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/cfileio.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/light.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/material.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/material.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/matrix3x3.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/matrix3x3.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/matrix4x4.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/matrix4x4.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/mesh.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/postprocess.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/quaternion.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/quaternion.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/scene.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/metadata.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/texture.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/types.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/vector2.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/vector2.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/vector3.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/vector3.inl"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/version.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/cimport.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/importerdesc.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Importer.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/DefaultLogger.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/ProgressHandler.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/IOStream.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/IOSystem.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Logger.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/LogStream.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/NullLogger.hpp"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/cexport.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Exporter.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Compiler/pushpack1.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Compiler/poppack1.h"
    "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/assimp-3.1.1/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/Debug/assimpd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/LakeheadUCourse/comp_proj/learn_opengl/libraries/ASSIMP/code/RelWithDebInfo/assimp.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

