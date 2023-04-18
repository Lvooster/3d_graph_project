cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(EngProgramm)

set(CMAKE_CXX_STANDARD 11)			 
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_definitions(-DSFML_STATIC)

include_directories("ext32/SFML/include/")
link_directories("ext32/SFML/lib/")
include_directories(headers sources resource)

add_executable(EngProgramm
main.cpp
headers/camera.h
headers/interface.h
headers/matrix.h
headers/object.h
headers/scene.h
sources/camera.cpp
sources/interface.cpp
sources/object.cpp
sources/scene.cpp
resource/calibri.ttf
)

target_link_libraries("${PROJECT_NAME}" sfml-graphics-s-d
						  sfml-window-s-d
						  sfml-system-s-d
						  opengl32
						  freetype
						  winmm
						  gdi32
						  sfml-audio-s-d
						  openal32
						  flac
						  vorbisenc
						  vorbisfile
						  vorbis
						  ogg)