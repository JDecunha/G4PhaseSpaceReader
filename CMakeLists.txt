#CMakeLists file for MicroTrackGenerator

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(G4PhaseSpaceReader VERSION 1.0.0)

###
### Find and include Geant4 libraries
###

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE}) #include the Geant4 CMakeLists

###
### Configure executable and in-project libraries
###

add_executable(G4PhaseSpaceReader G4PhaseSpaceReader.cc)

#locate our project source files and make a library
	file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
	add_library(src STATIC ${sources})
	target_include_directories(src PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/extern/include ${Geant4_INCLUDE_DIR})

#locate the extern source files and make a library
	file(GLOB external ${PROJECT_SOURCE_DIR}/extern/src/*.cc)
	add_library(extern STATIC ${external})
	target_include_directories(extern PUBLIC ${PROJECT_SOURCE_DIR}/extern/include ${Geant4_INCLUDE_DIR})

###
### Link G4PhaseSpaceReader with libraries
###

target_link_libraries(${PROJECT_NAME} src extern ${Geant4_LIBRARIES})

#Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

