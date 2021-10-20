#include "../include/RunAction.h"
#include "../include/Constants.h"
#include "G4Run.hh"

#include "G4Timer.hh"

//#include "B1Constantes.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include <fstream>

// #include "G4NeutronHPManager.hh"

using namespace std;


RunAction::RunAction()  : G4UserRunAction(), timer(new G4Timer), nOfReflections_Total(0),
nOfDetections_Total(0), TOF_Detections_Total(0) {}

RunAction::~RunAction()
{
    delete timer;
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {

    //-------------------------Root setup--------------------------------//
    G4cout << "========= Run beggining =========\nStarting run " << aRun->GetRunID() << " with " 
    << aRun->GetNumberOfEventToBeProcessed() << " events being processed" << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4cout << "Using " << analysisManager->GetType() << " analysis manager." << G4endl;

    analysisManager->OpenFile("output.root");

    // // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1 ("hEnergy","Energy of scintillation photon",2000,4,16); //0 Liquid Argon scintillation spectrum
    analysisManager->CreateH1 ("hAlpha","Alpha Spectrum",100,-1.0, 6.0); //1
    analysisManager->CreateH1 ("hPhotonsAlpha","Photons/Alpha",250,0,7000); //2
    
    // //Declara ntuples
    analysisManager->CreateNtuple("iPhotons", "Incident Photons");
    analysisManager->CreateNtupleIColumn("photonsDetected"); //0
    analysisManager->CreateNtupleDColumn("alphaEnergy"); //1
    analysisManager->FinishNtuple(0);

    timer->Start();
}

void RunAction::EndOfRunAction(const G4Run* aRun) {

    G4cout << "========= End of run =========" << G4endl;
    
    timer->Stop();

    if (PRINT_FILE == 1)
    {
       ofstream Output;

       Output.open(TXT_FILE, std::ios::app);
       Output<< setprecision(4) << fixed << (G4double)nOfDetections_Total <<endl;
       Output.close();
   }

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}