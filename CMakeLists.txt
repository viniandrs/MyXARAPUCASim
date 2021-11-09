cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(RafaelaSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(mySim3 main.cpp ${sources} ${headers})
target_link_libraries(mySim3 ${Geant4_LIBRARIES})

add_custom_target(RafaelaSim DEPENDS mySim3)
