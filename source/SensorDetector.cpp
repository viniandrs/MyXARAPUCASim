#include "../include/SensorDetector.h"

#include <iostream>
#include <iomanip>

using namespace std;

#include "../include/MyHit.h"

MySensorDetector::MySensorDetector(const G4String &name) : G4VSensitiveDetector(name)
{
}

MySensorDetector::~MySensorDetector() {}

/*
TouchableHistory has a collection of information about the particle as it has traveled along the physical
volumes.
*/
G4bool MySensorDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    if (aStep->GetTotalEnergyDeposit() != 0)
    {
        auto aHit = new MyHit( aStep );
        aHit->Print();
        delete aHit;
    }

    return true;
}

/*
Now we shall put our sensitive detector into our world through the DetectorConstruction class.
*/
