#pragma once

#include "../include/RunAction.h"
#include "../include/PrimaryGeneratorAction.h"

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

class SteppingAction : public G4UserSteppingAction
{

public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step *);

};