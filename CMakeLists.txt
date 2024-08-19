cmake_minimum_required(VERSION 3.5.0)
project(Milo VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)


include(C:/Users/owcar/vcpkg/scripts/buildsystems/vcpkg.cmake)

file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(Milo ${SOURCES} ${HEADERS})

target_link_libraries(Milo PUBLIC GLEW::GLEW)
target_link_libraries(Milo PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(Milo PUBLIC glfw)
target_link_libraries(Milo PRIVATE glad::glad)