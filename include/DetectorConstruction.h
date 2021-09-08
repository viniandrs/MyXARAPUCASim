#pragma once

#include "G4VUserDetectorConstruction.hh"

class Detector : public G4VUserDetectorConstruction
{
public:
    G4VPhysicalVolume *Construct() override;

private://methods

    void DefineMaterials();

};
