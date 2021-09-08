#   Define an environment variable G4INCLUDES that points to 
# your installation path, where the includes are. It is usually
# something like: /path/to/geant4-install/include/Geant4
g++ main.cpp \
    source/DetectorConstruction.cpp \
    source/MyGenerator.cpp \
    source/SensorDetector.cpp \
    source/MyRunAction.cpp \
    source/MyEventAction.cpp \
    source/MyHit.cpp \
    `geant4-config --libs` \
    -I${G4INCLUDES} \
    -o simulacao1

