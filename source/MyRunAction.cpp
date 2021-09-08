#include "../include/MyRunAction.h"
#include "G4Run.hh"

void MyRunAction :: BeginOfRunAction(const G4Run *aRun)
{
    G4cout << "========= Run beggining =========\n Starting run " << aRun->GetRunID() << " with " 
    << aRun->GetNumberOfEventToBeProcessed() << " events being processed" << G4endl;
}

void MyRunAction :: EndOfRunAction(const G4Run *aRun)
{
    G4cout << "========= End of run =========" << G4endl;
}