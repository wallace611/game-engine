cmake_minimum_required(VERSION 3.15)
project(OpenGL_Env_Test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(FreeGLUT CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_SOURCE_DIR}/code)

add_executable(main 
    main.cpp
    
    engine/engine.cpp

    engine/input/input_mapper.cpp

    engine/object/object.cpp
    engine/object/camera.cpp
    engine/object/scene_object.cpp
    engine/object/collision/collider.cpp
    engine/object/collision/aabb.cpp
    engine/object/collision/sphere.cpp

    engine/rendering/shader_loader.cpp
    engine/rendering/model_drawer.cpp
    engine/rendering/drawer/collider_drawer.cpp
    engine/rendering/drawer/phong_drawer.cpp

    code/scene_initializing.cpp
    code/object/wall.cpp
    code/object/ball.cpp
    code/interface/color_interface.cpp
)

target_link_libraries(main PRIVATE FreeGLUT::freeglut glad::glad glm::glm)

# Link system OpenGL library for Windows
if(WIN32)
    target_link_libraries(main PRIVATE opengl32)
endif()


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/shader")
    add_custom_target(CopyShaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "${CMAKE_BINARY_DIR}/bin/shader"
        COMMENT "Forcing shader directory copy on every build..."
    )

    add_dependencies(main CopyShaders)
endif()


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/textures")
    add_custom_target(CopyTextures ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/textures"
        "${CMAKE_BINARY_DIR}/bin/textures"
        COMMENT "Forcing texture directory copy on every build..."
    )

    add_dependencies(main CopyTextures)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/model")
    add_custom_target(CopyModels ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/model"
        "${CMAKE_BINARY_DIR}/bin/model"
        COMMENT "Forcing model directory copy on every build..."
    )

    add_dependencies(main CopyModels)
endif()