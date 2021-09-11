#pragma once

#include "../include/RunAction.h"
#include "../include/EventAction.h"
#include "../include/PrimaryGeneratorAction.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

extern G4bool flag_generate;

class SteppingAction : public G4UserSteppingAction
{

public:
    SteppingAction(EventAction *eventAction, RunAction *runAction, PrimaryGeneratorAction *PrimaryGeneratorAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step *);

private:
    G4int fEventNumber;
    EventAction *fEventAction;
    RunAction *fRunAction;
    PrimaryGeneratorAction *fPrimaryGeneratorAction;
    G4double contadorPhotons1;
    G4double contadorPhotons2;
    G4double contadorEnergia;
    G4double myEnergy;
    G4double x;
    G4double y;
    G4double z;
    G4int teste;
    G4bool firstStep;
    G4double initial_energy;
    G4double temp_edep;
    G4double temp_ID;
    G4bool isAlpha;

    G4bool firstOfAll = true;
};