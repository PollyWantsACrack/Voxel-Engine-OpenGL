# CMakeList.txt : CMake project for OpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGL")

# Add source to this project's executable.
add_executable (OpenGL
"Src/OpenGL.cpp"
"Header Files/OpenGL.h"
"Dependencies/src/glad.c" 
"Dependencies/src/stb.cpp"
"Header Files/Shader.h"
"Header Files/Camera.h"
"Header Files/Texture.h"
"Header Files/stb_image.h"
 "Header Files/Vao.h" "Header Files/UniformBuffer.h" "Header Files/ChunkLoader.h" "Header Files/Block.h" "Header Files/Chunk.h" "Header Files/PhysicsEngine.h" "Header Files/Player.h" "Header Files/Crosshair.h")


include_directories("${CMAKE_SOURCE_DIR}/Header Files")
include_directories("${CMAKE_SOURCE_DIR}/Dependencies/include")

set(GLFW_LIB "${CMAKE_SOURCE_DIR}/Dependencies/libs/glfw3.lib")
target_link_libraries(OpenGL ${GLFW_LIB} opengl32)






if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
