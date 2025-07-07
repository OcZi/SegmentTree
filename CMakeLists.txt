cmake_minimum_required(VERSION 3.30)
project(segment_tree)

set(CMAKE_CXX_STANDARD 20)

# Include the command that downloads libraries
include(FetchContent)

# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(${libName})
endfunction()

# add raylib support
set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
    message(STATUS "Getting ${LIB1} from Github")
    include_dependency(${LIB1} https://github.com/raysan5/raylib.git 5.5)
else()
    message(STATUS "Using local ${LIB1}")
endif()

add_executable(segment_tree main.cpp
        src/segment_tree.cpp
        src/segment_tree.h
        src/node.cpp
        src/node.h
        src/visualizer.h
        src/visualizer.cpp)

# Apuntamos a la carpeta 'src' principal, donde están tanto raylib.h como raygui.h
target_include_directories(segment_tree PRIVATE
        ${raylib_SOURCE_DIR}/src
)

# Vinculamos la librería raylib a nuestro proyecto
target_link_libraries(segment_tree PRIVATE raylib)