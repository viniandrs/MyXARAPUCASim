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
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step *);

};