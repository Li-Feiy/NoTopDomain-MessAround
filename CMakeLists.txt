cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(MainWindow LANGUAGES CXX)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "S:/Qt/5.15.2/mingw81_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
find_package(Qt5 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS
aux_source_directory(./src srcs)

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs}
    src/main.cpp
    src/Help.h src/Help.cpp src/Help.ui
    src/About.h src/About.cpp src/About.ui
    src/Feedback.h src/Feedback.cpp src/Feedback.ui
    src/MainWindow.cpp src/MainWindow.h src/MainWindow.ui
    src/Game.h src/Game.cpp src/Game.ui
    Res.qrc
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets) # Qt5 Shared Library
