cmake_minimum_required(VERSION 3.26.0)
project(proj01 VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

# place for finding Findglfw3.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# external opengl related libraries 
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


# adding source files to our exectuable programs
add_executable(run01 src/main.cpp src/glad.c src/shader.cpp)

# specify include directories
target_include_directories(run01 PRIVATE include)

# specify library directories
target_link_libraries(run01 ${GLFW3_LIBRARY} OpenGL::GL)

# copy shaders
# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_CURRENT_SOURCE_DIR}/shaders"
	"${CMAKE_CURRENT_BINARY_DIR}/shaders"
)

add_dependencies(run01 CopyShaders)