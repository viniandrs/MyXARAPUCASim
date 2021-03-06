#pragma once

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    ~ActionInitialization();
    
    virtual void BuildForMaster() const;
    virtual void Build() const;
};