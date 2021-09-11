#pragma once
#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()

class EventAction : public G4UserEventAction
{
public:
    
    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    
public:

    //Data members to hold information

    G4int nOfReflections;
    G4int nOfDetections;
};