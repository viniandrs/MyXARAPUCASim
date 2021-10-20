#include "../include/SteppingAction.h"
#include "../include/Constants.h"
#include "../include/PrimaryGeneratorAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4TrackStatus.hh"

#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"

/* Alteracao 5 5 17 */
#include "G4ios.hh"
#include "G4GeometryTolerance.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PhysicalConstants.hh"

#include <unistd.h>

//Arquivo txt de output
#include <fstream>
using namespace std;
//ofstream OutPut_Step(TXT_FILE);

// Conversao de radianos para graus
#define RAD_TO_DEG 180 / pi
//================================================================================

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

//================================================================================

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    //============================ Collecting information about the Step ============================
    //fEventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4Track *track = aStep->GetTrack();
    G4String particle = track->GetDefinition()->GetParticleName();

    //PreStep volume info
    G4StepPoint *aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume *aPrePV = aPrePoint->GetPhysicalVolume();
    G4String PreVolName = "";
    if (aPrePV)
        PreVolName = aPrePV->GetName();

    //PostStep volume info
    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *aPostPV = aPostPoint->GetPhysicalVolume();
    G4String PostVolName = "";
    if (aPostPV)
        PostVolName = aPostPV->GetName();

    //some another info
    G4int trackID = aStep->GetTrack()->GetTrackID();

    G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    //============================ Building the histograms ============================
    G4ThreeVector Pre_momentum;
    G4ThreeVector Pos_momentum;

    //Alpha spectrum
    if (particle == "alpha")
    {
        //Get the kinetic energy an alpha particle has when it leaves the aluminum
        if (PreVolName == "aluminumAlphaSource" && PostVolName == "Argon")
        {
            G4double kinEnergy = track->GetKineticEnergy();
            analysisManager->FillH1(1, kinEnergy);
        }
    }

    //Counting the photons that entered the Arapuca
    if (particle == "opticalphoton")
    {
        //Checking if the particle passed from the Argon to one of the detector screens
        if ((PreVolName == "Argon" && PostVolName == "detector2") ||
            (PreVolName == "Argon" && PostVolName == "detector1"))
        {
            analysisManager->FillNtupleIColumn(0, 1); //preenche o branch da TTree
            analysisManager->AddNtupleRow(0);
        }
    }

    //Liquid argon scintillation spectrum
    if (particle == "opticalphoton")
    {

        /* If creater is null, this means the track is created by the event generator, i.e. the primary track. 
        If it is not null, creator points to the process which created the track.*/
        const G4VProcess *creator = track->GetCreatorProcess();

        if (creator != NULL)
        {
            G4double photonEnergy = track->GetTotalEnergy()/eV;
            analysisManager->FillH1(0, photonEnergy);
        }
    }
}