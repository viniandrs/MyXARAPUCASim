/* ========================================================================
    VINIOSSI'S GUIDE TO GEANT 4 - Part 6
    -----------------------------------------------------------------------
    Sensitive detectors
   ======================================================================== */

#pragma once

#include "G4VSensitiveDetector.hh"

class MySensorDetector : public G4VSensitiveDetector
{
public:
    MySensorDetector(const G4String &name);
    ~MySensorDetector();

    /*
    A step is a  ...    .
    */
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;
};
