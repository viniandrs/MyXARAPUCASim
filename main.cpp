#include <iostream>

using namespace std;

#include "include/DetectorConstruction.h"
#include "include/PrimaryGeneratorAction.h"

#include "G4RunManager.hh"
#include "G4ScoringManager.hh"

#include "QBBC.hh"

//Interface classes
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#define G4VIS_USE_OPENGLQT
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

//Calling the User Action classes
#include "include/RunAction.h"
#include "include/EventAction.h"

//Physics configuration
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option2.hh"

int main(int argc, char **argv)
{
    G4RunManager *runManager = new G4RunManager;

    // Physics list setup
    G4VModularPhysicsList *physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option2());

    G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
    opticalPhysics->Configure(kCerenkov, true);
    opticalPhysics->Configure(kScintillation, true);
    physicsList->DumpList();
    physicsList->RegisterPhysics(new G4OpticalPhysics());

    runManager->SetUserInitialization(physicsList);

    // User initialization classes
    runManager->SetUserInitialization(new Detector());
    runManager->SetUserInitialization(physicsList);

    //We need to supply the Run Manager with our User Action classes before initializing
    runManager->SetUserAction(new RunAction());
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new EventAction());

    runManager->Initialize();

    //Initializing the visualization manager
    auto *visManager = new G4VisExecutive();
    visManager->Initialize();

    //User interface
    auto *uiExecutive = new G4UIExecutive(argc, argv, "Qt");

    if (argc == 1)
    {
        uiExecutive->SessionStart();
    }
    else
    {
        auto *uiManager = G4UImanager::GetUIpointer();
        
        //Executing instructions from a macro file
        uiManager->ApplyCommand("/control/execute " + G4String(argv[1]));
        uiExecutive->SessionStart();
    }

    delete uiExecutive;
    delete runManager;
    return 0;
}