cmake_minimum_required(VERSION 3.15)
project(OpenGL_Env_Test)

set(CMAKE_CXX_STANDARD 17)

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
    engine/object/light_object.cpp

    engine/rendering/shader_loader.cpp
    engine/rendering/model_drawer.cpp

    code/object/cube.cpp

    code/rendering/phong_drawer.cpp
    code/rendering/skybox_drawer.cpp
)

target_link_libraries(main PRIVATE FreeGLUT::freeglut glad::glad glm::glm)

# Link system OpenGL library for Windows
if(WIN32)
    target_link_libraries(main PRIVATE opengl32)
endif()


# Check if the 'shader' directory exists in the source folder
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/shader")
    add_custom_target(CopyShaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "${CMAKE_CURRENT_BINARY_DIR}/shader"
        COMMENT "Forcing shader directory copy on every build..."
    )

    add_dependencies(main CopyShaders)
endif()


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/textures")
    add_custom_target(CopyTextures ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/textures"
        "${CMAKE_CURRENT_BINARY_DIR}/textures"
        COMMENT "Forcing texture directory copy on every build..."
    )

    add_dependencies(main CopyTextures)
endif()