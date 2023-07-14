# Setup the project
#
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(DMG4ebrem)

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

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../modules_cmake_standalone)

#----------------------------------------------------------------------------
# Set DMG4
#
set (DMG4_DIR /Users/Gajju/dmg4)
set (DMG4_INCLUDE_DIR ${DMG4_DIR}/include)
set (DMG4_LIBRARY_DIR ${DMG4_DIR}/lib)

#----------------------------------------------------------------------------
# Find GSL
#
find_package(GSL REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(${Geant4_USE_FILE})
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${DMG4_INCLUDE_DIR}
  ${DMG4_INCLUDE_DIR}/DMG4
  ${GSL_INCLUDE_DIR}
  )

#----------------------------------------------------------------------------
# Locate sources and libraries for this project
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc ${PROJECT_SOURCE_DIR}/src/*.cc)
link_directories (
  ${DMG4_LIBRARY_DIR}
  )

set(BREMPROJECT-DMG4_SCRIPTS
  cardg.g4mac
  vis.mac
  clean
  )

foreach(_script ${BREMPROJECT-DMG4_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and other libraries
#
add_executable(dmg4brem ${sources})
target_link_libraries(dmg4brem DarkMatter DMG4 UtilsDM ${Geant4_LIBRARIES} ${GSL_LIBRARIES})

# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS dmg4brem DESTINATION bin)
