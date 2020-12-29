cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME practicafinal)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(SOURCES_LOCATION src)
set(HEADERS_LOCATION ../libraries/includes)
set(LIBS_LOCATION    ../libraries/libs)

include_directories(${SOURCES_LOCATION} ${HEADERS_LOCATION})
link_directories(${LIBS_LOCATION})
file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${SOURCES_LOCATION}/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}  opengl32 glu32 glew32 freeglut assimp freeimage)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG binary)