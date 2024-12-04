# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.11)

# Set the C++ standard to C++14 and ensure it is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the name of project
project(Car_Crasher)

#### Setup Directories ####
# Set the main output directory for the compiled binaries
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

# Set the output directory for runtime binaries (e.g., executables and DLLs)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
# Add the SFML library by pointing to its subdirectory
add_subdirectory("lib/sfml")

# Specify the SFML include directory for header files
set(SFML_INCS "lib/sfml/include")

# Link the SFML libraries from the specified directory
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# Define a custom target to copy the 'res' (resources) folder to the appropriate output directory
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # For Visual Studio: copy resources into Debug/Release-specific directories
    add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
            -E copy_directory
            "${PROJECT_SOURCE_DIR}/res"
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
    )
else()
    # For CLion (or other generators): copy resources into the root build directory
    add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
            -E copy_directory
            "${PROJECT_SOURCE_DIR}/res"
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
    )
endif()


# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

## ECM Library
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" SYSTEM INTERFACE ${SFML_INCS} "${CMAKE_SOURCE_DIR}/lib_maths")
target_link_libraries(lib_ecm PRIVATE lib_maths)

## Car Crasher
file(GLOB_RECURSE SOURCES car_crasher/*.cpp car_crasher/*.h)
add_executable(CAR_CRASHER ${SOURCES})
target_include_directories(CAR_CRASHER PRIVATE ${SFML_INCS})
target_link_libraries(CAR_CRASHER sfml-graphics lib_ecm)

# Set the working directory for the Visual Studio debugger when running CAR_CRASHER
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(CAR_CRASHER
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
    )
endif()

# Make sure that the 'copy_resources' target is executed before building Tile Engine
add_dependencies(CAR_CRASHER copy_resources)