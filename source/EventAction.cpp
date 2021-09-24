#include "../include/EventAction.h"
#include "../include/RunAction.h"
#include "../include/Constants.h"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

void EventAction::BeginOfEventAction(const G4Event* anEvent) {

    nOfReflections = 0;
    nOfDetections = 0;

}

void EventAction::EndOfEventAction(const G4Event* anEvent) {

    // Print number of reflections
    if (IMPRIMIR_EVENTOS == false) {//false?
        G4cout << "Number of reflections [Event] = " << nOfReflections << G4endl;
        G4cout << "Number of detections [Event] = " << nOfDetections << G4endl;
    }

    // Print per event (modulo n)
    G4int eventID = anEvent->GetEventID();
    G4RunManager* runManager = G4RunManager::GetRunManager();
    G4int printModulo = runManager->GetPrintProgress();

    //NÃO ENTENDI
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }

}

//================================================================================