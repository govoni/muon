#  $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MUON)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
 
### including ROOT Headers
EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

### standard G4
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh, ${PROJECT_SOURCE_DIR}/include/*.h, ${PROJECT_SOURCE_DIR}/include/*.icc)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(muon muon.cc ${sources} ${headers})

### including ROOT libraries
EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})


### standard G4
target_link_libraries(muon ${Geant4_LIBRARIES})

target_link_libraries(muon ${ROOT_LD_FLAGS})

### suppress warnings
if(MSVC)
    ### need to find proper option
else()
    target_compile_options(muon PRIVATE -w)
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build muon. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SOME_SCRIPTS
  run.png
  gui.mac
  icons.mac
  init_vis.mac
  vis.mac
  shoot_positron.mac
  )


#set(CMAKE_BUILD_TYPE Debug)

foreach(_script ${SOME_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/utils/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS muon DESTINATION bin)
