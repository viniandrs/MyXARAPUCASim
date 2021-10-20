#pragma once
#include "G4UserRunAction.hh"
#include "G4NeutronHPManager.hh"
#include "G4Timer.hh"
#include "g4root.hh"

using namespace std;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

  void BeginOfRunAction(const G4Run *aRun);
  void EndOfRunAction(const G4Run *aRun);

  G4NeutronHPManager *GetInstance();

private:
  G4int nOfReflections_Total;
  G4int nOfDetections_Total;
  // Soma do tempo de voo de todos os fotons detectados
  G4double TOF_Detections_Total;

  G4Timer *timer;
};