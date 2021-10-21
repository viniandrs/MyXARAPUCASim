#include "../include/RunAction.h"
#include "../include/Constants.h"

using namespace std;

RunAction::RunAction(){}

RunAction::~RunAction(){}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    //-------------------------Root setup--------------------------------//
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("output.root");

    // // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1 ("hEnergy","Energy of scintillation photon",2000,4,16); //0 Liquid Argon scintillation spectrum
    analysisManager->CreateH1 ("hAlpha","Alpha Spectrum",100,-1.0, 6.0); //1
    analysisManager->CreateH1 ("hPhotonsAlpha","Photons/Alpha",250,0,7000); //2
    
    // //Declara ntuples
    analysisManager->SetFirstNtupleId(0);
    analysisManager->CreateNtuple("iPhotons", "Incident Photons");//NTuple 0
    analysisManager->CreateNtupleIColumn("photonsDetected"); //0
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("alphasOutAluminum", "Alpha particles in lAr");//NTuple 1
    analysisManager->CreateNtupleIColumn("trackID");//0
    analysisManager->CreateNtupleIColumn("photonsGenerated");//1
    analysisManager->FinishNtuple(1);
    
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}