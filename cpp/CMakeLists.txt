cmake_minimum_required( VERSION 2.6 )

project( cg3 )

set( CMAKE_BUILD_TYPE "RELEASE" )

# add_definitions( "-std=c++11 -g -O0" )
add_definitions( "-std=c++11" )

option( USE_RENDERMANAGER_SIGNALS "Enable signals to monitor rendering progress." ON )

if( USE_RENDERMANAGER_SIGNALS )
  add_definitions( -DUSE_RENDERMANAGER_SIGNALS )
endif()

FIND_PACKAGE ( Threads REQUIRED )

include_directories( include include/common/ )

add_library( cg3common src/common/primitive.cpp
                       src/common/primitive_group.cpp
                       src/common/transformation_group.cpp
                       src/common/sphere.cpp
                       src/common/plane.cpp
                       src/common/box.cpp
                       src/common/cylinder.cpp
                       src/common/triangle.cpp
                       src/common/kd_tree.cpp

                       src/common/intersection_info.cpp

                       src/common/material.cpp
                       src/common/phong.cpp
                       src/common/blinn_phong.cpp

                       src/common/light_source.cpp
                       src/common/point_light.cpp
                       src/common/directional_light.cpp
                       src/common/spot_light.cpp
                       src/common/area_light.cpp

                       src/common/renderer.cpp
                       src/common/scene.cpp

                       src/common/camera.cpp

                       src/common/test_scenes.cpp
           )

add_library( cg3manager src/manager/render_manager.cpp
                        src/manager/tiled_image.cpp
                        src/manager/strategy.cpp
                        src/manager/progressive_strategy.cpp
           )

add_library( cg3rt src/raytracer/raytracer.cpp )

add_library( cg3pt src/pathtracer/pathtracer.cpp )

add_executable( cg3raytrace apps/main_rt.cpp )
target_link_libraries( cg3raytrace  cg3rt cg3manager cg3common ${CMAKE_THREAD_LIBS_INIT} )

add_executable( cg3pathtrace apps/main_pt.cpp )
target_link_libraries( cg3pathtrace cg3pt cg3manager cg3common ${CMAKE_THREAD_LIBS_INIT} )

add_executable( linear_diffusion apps/linear_diffusion.cpp )

install( DIRECTORY include/ DESTINATION include )

install( TARGETS cg3common cg3manager cg3rt cg3pt 
         RUNTIME DESTINATION bin 
         ARCHIVE DESTINATION lib64
         INCLUDES DESTINATION include
       )

install( TARGETS cg3raytrace cg3pathtrace RUNTIME DESTINATION bin )
