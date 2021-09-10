#include "../include/RunAction.h"
#include "../include/Constants.h"
#include "G4Run.hh"

#include "G4Timer.hh"

//#include "B1Constantes.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "g4root.hh"
#include <fstream>

// #include "G4NeutronHPManager.hh"

using namespace std;


RunAction::RunAction() : G4UserRunAction(), timer(new G4Timer), nOfReflections_Total(0),
nOfDetections_Total(0), TOF_Detections_Total(0) {}

RunAction::~RunAction() {
    delete timer;
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {


    G4cout << "========= Run beggining =========\n Starting run " << aRun->GetRunID() << " with " 
    << aRun->GetNumberOfEventToBeProcessed() << " events being processed" << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4cout << "Using " << analysisManager->GetType() << " analysis manager." << G4endl;

    timer->Start();

    analysisManager->OpenFile("Output");
    analysisManager->SetVerboseLevel(1);


    // // Indica o id do primeiro histograma criado -- default = 0
    analysisManager->SetFirstHistoId(1);
    // // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1 ("h1","Energy of scintillation photon",2000,0,20); //1
    analysisManager->CreateH1 ("h2","Alpha Spectrum",100,-1.0, 6.0); //2
    analysisManager->CreateH1 ("h3","Photons/Alpha",250,0,7000); //3
    // // Indica o id da primeira ntuple criada -- default = 0
     analysisManager->SetFirstNtupleId(1);
    // //Declara ntuples (descobrir oque é)
    analysisManager->CreateNtuple("t1", "data");
    analysisManager->CreateNtupleDColumn("detPhotons1");
    analysisManager->CreateNtupleDColumn("detPhotons2");
    analysisManager->CreateNtupleDColumn("detPhotons");
    analysisManager->CreateNtupleDColumn("alphaEnergy");
    analysisManager->CreateNtupleDColumn("axisX");
    analysisManager->CreateNtupleDColumn("axisY");
    analysisManager->CreateNtupleDColumn("axisZ");
    analysisManager->FinishNtuple();

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

    delete G4AnalysisManager::Instance();

}