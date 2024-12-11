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
add_executable(CAR_CRASHER WIN32 ${SOURCES})
target_include_directories(CAR_CRASHER PRIVATE ${SFML_INCS} "${CMAKE_CURRENT_SOURCE_DIR}/car_crasher")
target_link_libraries(CAR_CRASHER PRIVATE sfml-graphics sfml-audio sfml-window sfml-system sfml-main lib_ecm)


# Copy the OpenAL32.dll file to the output directory to get audio working
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # For Visual Studio: copy to Debug/Release-specific directories
    add_custom_command(TARGET CAR_CRASHER POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x64/openal32.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/"
    )
else()
    # For CLion (or other non-Visual Studio generators): copy to runtime directory
    add_custom_command(TARGET CAR_CRASHER POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x64/openal32.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
    )
endif()


# Set the working directory for the Visual Studio debugger when running CAR_CRASHER
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(CAR_CRASHER
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
    )
endif()

# Make sure that the 'copy_resources' target is executed before building Tile Engine
add_dependencies(CAR_CRASHER copy_resources)


## Installer Stuff
#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")
#Installer.exe/zip built into /installer
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")
#Install component for runtime dlls and stuff
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) #stop this happening on non-release builds
INCLUDE(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin/" CONFIGURATIONS Release COMPONENT SYSTEM_CMPT)
#Install component for resources
install(DIRECTORY "${PROJECT_SOURCE_DIR}/res" DESTINATION "bin/" CONFIGURATIONS Release COMPONENT RESOURCES_CMPT)
#We disabled SFML's INSTALL component as it brings along junk, so we gotta make one ourselves
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" DESTINATION "bin" CONFIGURATIONS Release COMPONENT SFML_CMPT FILES_MATCHING PATTERN "*.dll" )
#Main Install target
install(TARGETS CAR_CRASHER DESTINATION "bin" CONFIGURATIONS Release COMPONENT CAR_CRASHER_CMPT)

#Tell CPack about the libs and resources install components
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} RESOURCES_CMPT SFML_CMPT SYSTEM_CMPT CAR_CRASHER_CMPT)
set(CPACK_COMPONENT_CAR_CRASHER_CMPT_DEPENDS RESOURCES_CMPT SFML_CMPT SYSTEM_CMPT)
set(CPACK_COMPONENT_RESOURCES_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_SFML_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_SYSTEM_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_CAR_CRASHER_CMPT_DISPLAY_NAME "Car Crasher")

IF(WIN32)
    set(CPACK_GENERATOR NSIS)
    set(CPACK_NSIS_COMPONENT_INSTALL ON)
    set(CPACK_NSIS_HELP_LINK "https://github.com/jules946/set09121_games_engineering_car_crasher")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/jules946/set09121_games_engineering_car_crasher")
    set(CPACK_NSIS_CONTACT "https://github.com/jules946/set09121_games_engineering_car_crasher")
ELSE()
    set(CPACK_GENERATOR ZIP)
ENDIF(WIN32)


set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Car Crasher")
set(CPACK_PACKAGE_VENDOR "Julius, Alessio, and Luke")

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)