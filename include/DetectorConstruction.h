#pragma once

#include "G4VUserDetectorConstruction.hh"

class Detector : public G4VUserDetectorConstruction
{
public:
    Detector();
    ~Detector();

    G4VPhysicalVolume *Construct() override;

protected:
    G4LogicalVolume*  fScoringVolume;  
};
