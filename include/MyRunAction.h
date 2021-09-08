#pragma once
#include "G4UserRunAction.hh"

class MyRunAction : public G4UserRunAction
{
public:
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);
};