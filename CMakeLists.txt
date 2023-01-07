cmake_minimum_required(VERSION 3.11)
project(conway_gol)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS graphics REQUIRED)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)

target_include_directories(${PROJECT_NAME} PRIVATE
	src
)
target_sources(${PROJECT_NAME} PRIVATE

	src/main.cpp
	src/Grid.hpp
	src/Grid.cpp

)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()

if(CMAKE_GENERATOR MATCHES "^(Visual Studio)")
  target_compile_options(${PROJECT_NAME} INTERFACE /MP)
endif()
