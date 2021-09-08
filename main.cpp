#include <iostream>

using namespace std;

#include "include/DetectorConstruction.h"
#include "include/MyGenerator.h"

#include "G4RunManager.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "QBBC.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#define G4VIS_USE_OPENGLX
#define G4VIS_USE_OPENGLQT
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4OpticalPhysics.hh"

//Calling the User Action classes
#include "include/MyRunAction.h"
#include "include/MyEventAction.h"

int main(int argc, char **argv)
{
    //Creating our run manager
    G4RunManager *runManager = new G4RunManager;

    // Physics list
    G4VModularPhysicsList *physicsList = new QBBC;
    physicsList->SetVerboseLevel(1);

    physicsList->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physicsList);

    // User initialization classes
    runManager->SetUserAction(new MyPrimaryGenerator());
    runManager->SetUserInitialization(new Detector());
    runManager->SetUserInitialization(physicsList);

    //We need to supply the Run Manager with our User Action classes before initializing
    runManager->SetUserAction(new MyRunAction());
    runManager->SetUserAction(new MyEventAction());

    runManager->Initialize();

    auto *visManager = new G4VisExecutive();
    visManager->Initialize();

    auto *uiExecutive = new G4UIExecutive(argc, argv, "Qt");

    if (argc == 1)
    {
        uiExecutive->SessionStart();
    }
    else
    {
        auto *uiManager = G4UImanager::GetUIpointer();
        uiManager->ApplyCommand("/control/execute " + G4String(argv[1]));
        uiExecutive->SessionStart();
    }

    delete uiExecutive;
    delete runManager;
    return 0;
}