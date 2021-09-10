#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"

class Detector : public G4VUserDetectorConstruction
{
public:
    Detector(/*G4double RIndex*/);
    ~Detector();

    G4VPhysicalVolume *Construct() override;

protected:
    G4LogicalVolume*  fScoringVolume;  
};
