cmake_minimum_required(VERSION 3.8)
project(MuonSim)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    main.cc
    DetectorConstruction.cc
    PrimaryGeneratorAction.cc
    EventAction.cc
    SteppingAction.cc
)

add_executable(muonSim ${SOURCES})
target_link_libraries(muonSim ${Geant4_LIBRARIES})

configure_file(run.mac run.mac COPYONLY)
